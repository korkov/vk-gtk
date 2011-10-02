#include "notifier.hxx"

#include <sys/stat.h>
#include <sys/types.h>
#include <fstream>
#include <libnotify/notify.h>
#include <boost/foreach.hpp>
#include <boost/algorithm/string.hpp>

#include "utils.hxx"
#include "vk_http.hxx"
#include "vk_common.hxx"
#include "vk_session.hxx"
#include "browsers.hxx"

using vk::notifier_t;
namespace utils = vk::utils;

namespace
{
  std::string store_photo(const std::string& url)
  {
    const std::string base = utils::base_name(url);
    const std::string avatars = utils::get_app_dir() + "/avatars";
    const std::string path = avatars + "/" + base;

    if (!utils::not_empty_file_exists(avatars))
      if (::mkdir(avatars.c_str(), 0700) != 0)
        throw VK_ERROR_NUM(_("cannot make directory")<<": "<<avatars);

    if (utils::not_empty_file_exists(path))
      return path;

    std::ofstream fout(path.c_str());
    if (!fout)
      throw VK_ERROR_NUM(_("cannot write file")<<": "<<path);

    vk::http_t http(url);
    fout << http.perform();
    fout.close();
    return path;
  }
}

namespace vk
{
  struct notify_t
  {
    typedef notify_t impl;

    notify_t(const session_t& ses, const message_t& mess)
      : m_ses(ses)
      , m_mess(mess)
    {
    }

    void show()
    {
      const profile_t prof = commands::get_profile(m_ses, m_mess.uid);
      if (prof.uid == 0)
        throw VK_ERROR(_("cannot get profile for uid") << ": " << m_mess.uid);

      const std::string title = VK_STR(
        _("Message from") << " " << prof.first_name << " " << prof.last_name);

      const std::string icon = store_photo(prof.photo);

      show_notify(title, m_mess.body, icon);
    }

  private:
    const session_t m_ses;
    const message_t m_mess;

    static void open_action_cb(
      NotifyNotification* notification,
      char* action, impl* pimpl
    )
    {
      return pimpl->open_action();
    }

    static void read_action_cb(
      NotifyNotification* notification,
      char* action, impl* pimpl
    )
    {
      return pimpl->read_action();
    }

    void open_action()
    {
      const std::string cmd =
        VK_STR("xdg-open "<<commands::get_char_page_url(m_mess.uid));
      ::system(cmd.c_str());
      delete this;
    }

    void read_action()
    {
      commands::mark_as_read(m_ses, m_mess);
      delete this;
    }

    void show_notify(const std::string& title, const std::string& body, const std::string& icon)
    {
      NotifyNotification* notify =
        notify_notification_new(
          title.c_str(), body.c_str(), icon.c_str()
#ifndef NOTIFY_CHECK_VERSION
          , NULL
#endif
        );

      notify_notification_add_action(
        notify, "open_acion_id",
        "Open", (NotifyActionCallback)open_action_cb,
        this, NULL);

      notify_notification_add_action(
        notify, "read_acion_id",
        "Mark as read", (NotifyActionCallback)read_action_cb,
        this, NULL);

      notify_notification_set_timeout(notify, 5000);
      notify_notification_set_category(notify, "vk");
      GError* error = NULL;
      notify_notification_show(notify, &error);
    }
  };
}


struct notifier_t::impl
{
  impl(const session_t& ses)
    : m_ses(ses)
  {
    ::notify_init("vk-gtk");
  }

  void show(const std::vector<message_t>& messages)
  {
    if (browsers::tab_opened(browsers::browser_firefox, "vkontakte.ru"))
      return;

    BOOST_FOREACH(const message_t& mess, messages)
      {
        notify_t* notify = new notify_t(m_ses, mess);
#warning сделать через shared_from_this
        notify->show();
      }
  }
  private:
  const session_t& m_ses;
};

notifier_t::notifier_t(const session_t& ses)
  : m_impl(new impl(ses))
{
}

void notifier_t::show(const std::vector<message_t>& messages)
{
  return m_impl->show(messages);
}
