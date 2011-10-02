#ifndef VK_1316728269
#define VK_1316728269

#include <boost/signal.hpp>

namespace vk
{
  struct widgets_t;

  struct tray_popup_t
  {
    tray_popup_t(const widgets_t& widgets);
    void popup(uint button, uint time);

  private:
    struct impl;
    boost::shared_ptr<impl> m_impl;

  public:
    boost::signal<void()>& settings_sig;  
    boost::signal<void()>& exit_sig;
    boost::signal<void(bool)>& auth_sig;
  };
}

#endif  // VK_1316728269
