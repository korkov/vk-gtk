#ifndef VK_1317497723
#define VK_1317497723

#include <string>

namespace browsers
{
  enum browser_t
    {
      browser_firefox,
      browser_chrome,
      browser_opera,
      browser_konqueror,
      browser_ie, // :)
    };

  bool tab_opened(browser_t br, const std::string& url_part);
}

#endif  // VK_1317497723
