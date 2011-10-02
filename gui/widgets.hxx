#ifndef VK_1316721126
#define VK_1316721126

#include <boost/shared_ptr.hpp>
#include <string>
#include <gtk/gtk.h>

namespace vk
{
  struct widgets_t
  {
    widgets_t(const std::string& file);
    GtkWidget* get(const char* name) const;

  private:
    struct impl;
    boost::shared_ptr<impl> m_impl;
  };
}

#endif  // VK_1316721126
