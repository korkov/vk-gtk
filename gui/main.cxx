#include <sys/stat.h>
#include <sys/types.h>

#include <boost/bind.hpp>

#include "vk_common.hxx"

#include "auth.hxx"
#include "notifier.hxx"
#include "messages_checker.hxx"
#include "vk_session.hxx"
#include "settings_window.hxx"
#include "tray.hxx"
#include "tray_popup.hxx"
#include "utils.hxx"
#include "widgets.hxx"

using namespace vk;

namespace
{
  GdkPixbuf* vk_login_icon()
  {
    static GdkPixbuf* ret = gdk_pixbuf_new_from_file(PKGDATA"/vk.png", NULL);
    return ret;
  }

  GdkPixbuf* vk_logout_icon()
  {
    static GdkPixbuf* ret = gdk_pixbuf_new_from_file(PKGDATA"/vk-logout.png", NULL);
    return ret;
  }

  GdkPixbuf* vk_message_icon()
  {
    static GdkPixbuf* ret = gdk_pixbuf_new_from_file(PKGDATA"/vk-message.png", NULL);
    return ret;
  }

  void auth_changed(tray_t* tray, bool on)
  {
    GdkPixbuf* cur_icon = on ? vk_login_icon() : vk_logout_icon();
    tray->set_pixbuf(cur_icon);
  }

  void set_tray_message_icon(tray_t* tray, bool on)
  {
    GdkPixbuf* cur_icon = on ? vk_message_icon() : vk_login_icon();
    tray->set_pixbuf(cur_icon);
  }
}

int main(int argc, char **argv)
{
  textdomain("vk-gtk");

  try
    {
      gtk_init(&argc, &argv);

      const std::string app_dir = utils::get_app_dir();
      if (!utils::not_empty_file_exists(app_dir))
        if (::mkdir(app_dir.c_str(), 0700) != 0)
          throw VK_ERROR_NUM(_("cannot make directory")<<": "<<app_dir);

      widgets_t widgets(PKGDATA"/vk-gtk.glade");

      settings_window_t settings(conf_t(), widgets);

      tray_t tray;
      tray.set_pixbuf(vk_logout_icon());

      session_t session;
      notifier_t notifier(session);

      messages_checker_t checker(session);
      checker.set_timeout(5);
      checker.received_sig.connect(boost::bind(&notifier_t::show, &notifier, _1));
      checker.have_unread_sig.connect(boost::bind(set_tray_message_icon, &tray, _1));

      session.activated_sig.connect(boost::bind(auth_changed, &tray, _1));
      session.activated_sig.connect(boost::bind(&messages_checker_t::set_active, &checker, _1));

      auth_t auth(2475380, 4096);
      auth.authorized_sig.connect(boost::bind(&session_t::set_token, &session, _1));

      tray_popup_t tray_popup(widgets);
      tray.popup_menu_activated.connect(boost::bind(&tray_popup_t::popup, &tray_popup, _1, _2));

      tray_popup.settings_sig.connect(boost::bind(&settings_window_t::show, &settings));
      tray_popup.exit_sig.connect(gtk_main_quit);
      tray_popup.auth_sig.connect(boost::bind(&auth_t::login, &auth, _1));

      auth.login(true);
      gtk_main();
    }
  catch (const std::exception& err)
    {
      utils::show_warning(err.what());
      return 1;
    }

  return 0;
}
