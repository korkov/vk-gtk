#ifndef VK_1317912695
#define VK_1317912695

#include <string>
#include <boost/shared_ptr.hpp>

namespace vk
{
  struct gst_sound_t
  {
  public:
    gst_sound_t();

    void play_file(const std::string& filename);
    void set_volume(double v);

  private:
    struct impl;
    boost::shared_ptr<impl> m_impl;
  };
}

#endif  // VK_1317912695
