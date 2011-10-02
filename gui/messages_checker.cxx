#include "messages_checker.hxx"

#include <gtk/gtk.h>

#include <boost/foreach.hpp>

#include "vk_session.hxx"
#include "vk_commands.hxx"
#include "vk_common.hxx"

using vk::messages_checker_t;

struct messages_checker_t::impl
{
  impl(const session_t& ses)
    : m_ses(ses)
    , m_timer_id(0)
    , m_timeout(0)
  {
  }

  void set_timeout(int timeout)
  {
    m_timeout = timeout;
    set_active(m_ses.active());
  }

  void set_active(bool active)
  {
    if (m_timer_id != 0)
      {
        g_source_remove(m_timer_id);
        m_timer_id = 0;
      }

    if (active && (m_timeout != 0) && (m_ses.active()))
      {
        m_timer_id = g_timeout_add(m_timeout * 1000, (GSourceFunc)update_cb, this);
        update();
      }
  }

 private:
  const session_t& m_ses;
  guint m_timer_id;
  guint m_timeout;
  std::vector<unsigned> m_shown;
public:
  boost::signal<void (std::vector<message_t>)> received_sig;
  boost::signal<void (bool)> have_unread_sig;

private:
  static gboolean update_cb(impl* pimpl) { return pimpl->update(); }

  bool already_shown(unsigned mid) const
  {
    return std::find(m_shown.begin(), m_shown.end(), mid) != m_shown.end();
  }

  gboolean update()
  {
    std::vector<message_t> new_messages;
    std::vector<unsigned> shown;

    std::vector<message_t> unread = commands::get_unread_messages(m_ses);

    BOOST_FOREACH(const message_t& mess, unread)
      {
        if (!already_shown(mess.mid))
          new_messages.push_back(mess);
        shown.push_back(mess.mid);
      }

    m_shown = shown;

    if (!new_messages.empty())
      received_sig(new_messages);

    have_unread_sig(!unread.empty());

    return TRUE;
  }

};

messages_checker_t::messages_checker_t(const session_t& ses)
  : m_impl(new impl(ses))
  , received_sig(m_impl->received_sig)
  , have_unread_sig(m_impl->have_unread_sig)
{
}

void messages_checker_t::set_timeout(int timeout)
{
  return m_impl->set_timeout(timeout);
}

void messages_checker_t::set_active(bool active)
{
  return m_impl->set_active(active);
}
