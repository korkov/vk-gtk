#ifndef VK_1316721014
#define VK_1316721014

#include "vk_common.hxx"

namespace vk
{
  struct widgets_t;
  struct conf_t {};

  struct settings_window_t
  {
    settings_window_t(const conf_t& conf, const widgets_t& widgets);
    void show();

  private:
    struct impl;
    boost::shared_ptr<impl> m_impl;
  };
}

#endif  // VK_1316721014
