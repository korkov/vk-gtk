#ifndef VK_1317483823
#define VK_1317483823

#include <vector>
#include <string>
#include <boost/shared_ptr.hpp>
#include "vk_commands.hxx"


namespace vk
{
  struct session_t;

  struct notifier_t
  {
    notifier_t(const session_t& ses);
    void show(const std::vector<message_t>& messages);

  private:
    struct impl;
    boost::shared_ptr<impl> m_impl;
  };
}

#endif  // VK_1317483823
