#ifndef VK_1316643114
#define VK_1316643114

#include <string>
#include <vector>
#include <boost/shared_ptr.hpp>
#include <boost/signal.hpp>

namespace vk
{
  struct session_t
  {
    session_t();
    std::string cmd(const std::string& cmd) const;

    bool active() const;
    void set_token(const std::string& token);

  private:
    struct impl;
    boost::shared_ptr<impl> m_impl;

  public:
    boost::signal<void (bool)>& activated_sig;
  };

  std::string get_auth_str(const std::string in);
}

#endif  // VK_1316643114
