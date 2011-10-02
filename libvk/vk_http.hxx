#ifndef VK_1316643860
#define VK_1316643860

#include <string>
#include <boost/shared_ptr.hpp>

namespace vk
{
  struct http_t
  {
    http_t(const std::string& url = "");
    void set_url(const std::string& url);
    std::string perform();

    void clear();
  private:
    struct impl;
    boost::shared_ptr<impl> m_impl;
  };
}

#endif  // VK_1316643860
