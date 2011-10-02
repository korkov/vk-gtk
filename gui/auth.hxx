#ifndef VK_1316731632
#define VK_1316731632

#include <string>
#include <boost/shared_ptr.hpp>
#include <boost/signal.hpp>

namespace vk
{
  struct auth_t
  {
    auth_t(int client_id, int scope);
    void login(bool on);

  private:
    struct impl;
    boost::shared_ptr<impl> m_impl;

  public:
    boost::signal<void (std::string)>& authorized_sig;
  };
}

#endif  // VK_1316731632
