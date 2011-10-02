#ifndef VK_1317160021
#define VK_1317160021

#include <string>
#include <vector>
#include <boost/shared_ptr.hpp>
#include <boost/signal.hpp>
#include "vk_commands.hxx"

namespace vk
{
  struct session_t;

  struct messages_checker_t
  {
    messages_checker_t(const session_t& ses);
    void set_active(bool active);
    void set_timeout(int timeout);

  private:
    struct impl;
    boost::shared_ptr<impl> m_impl;

  public:
    boost::signal<void (std::vector<vk::message_t>)>& received_sig;
    boost::signal<void (bool)>& have_unread_sig;
  };
}

#endif  // VK_1317160021
