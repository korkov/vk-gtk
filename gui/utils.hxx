#ifndef VK_1317315491
#define VK_1317315491

#include <string>

namespace vk
{
  namespace utils
  {
    std::string base_name(const std::string& str);
    bool not_empty_file_exists(const std::string& path);
    std::string get_app_dir();
    void show_warning(const std::string& what);
  }
}

#endif  // VK_1317315491
