#ifndef VK_1316647638
#define VK_1316647638

#include <string>
#include <boost/shared_ptr.hpp>

namespace vk
{
  namespace scope
  {
    const int messages = 4096;
  }

  struct app_t
  {
    app_t(int client_id, int scope);
    std::string get_url() const;

  private:
    struct impl;
    boost::shared_ptr<impl> m_impl;
  };
}

#endif  // VK_1316647638
