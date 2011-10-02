#ifndef VK_1316727886
#define VK_1316727886

#include <boost/shared_ptr.hpp>
#include <boost/signal.hpp>
#include <string>
#include <gtk/gtk.h>

namespace vk
{
  class tray_t
  {
  public:
    tray_t();
    void set_pixbuf(GdkPixbuf* pixbuf);
    void set_tooltip(const std::string& text);

  private:
    class impl;
    boost::shared_ptr<impl> pimpl;

  public:
    boost::signal<void ()>& activated;

    typedef unsigned int uint;
    boost::signal<void (uint button, uint time)>& popup_menu_activated;
  };
}

#endif  // VK_1316727886
