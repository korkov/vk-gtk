#include "gst_sound.hxx"

#include <gst/gst.h>

#include "vk_common.hxx"

using vk::gst_sound_t;

namespace
{
  struct gst_initializer_t
  {
    gst_initializer_t()
    {
      int argc = 0;
      char **argv = NULL;

      if (gst_init_check(&argc, &argv, NULL) == FALSE)
        throw VK_ERROR("gst_init_check failed");
    }
    ~gst_initializer_t()
    {
      gst_deinit();
    }
  };
}

struct gst_sound_t::impl
{
  impl()
    : m_play(gst_element_factory_make("playbin", "play"))
    , m_sink(gst_element_factory_make("autoaudiosink", "sink"))
    , m_volume(1.0)
  {
    if (m_play == NULL)
      throw VK_ERROR(_("unable to create Gstreamer playbin"));
    if (m_sink == NULL)
      throw VK_ERROR(_("unable to create GStreamer audiosink"));
    g_object_set(G_OBJECT(m_play), "audio-sink", m_sink, NULL);
  }

  void play_file(const std::string& filename)
  {
    gst_element_set_state(m_sink, GST_STATE_NULL);
    gst_element_set_state(m_play, GST_STATE_NULL);
    const std::string uri = VK_STR("file://"<<filename);
    g_object_set(G_OBJECT(m_play), "uri", uri.c_str(), "volume", m_volume, NULL);
    gst_element_set_state(m_play, GST_STATE_PLAYING);
  }

  void set_volume(const double v)
  {
    if (v > 1.0)
      m_volume = 1.0;
    else if (v < 0)
      m_volume = 0;
    else
      m_volume = v;
  }

private:
  gst_initializer_t m_gst;
  GstElement* m_play;
  GstElement* m_sink;
  double m_volume;
};

gst_sound_t::gst_sound_t()
: m_impl(new impl())
{
}

void gst_sound_t::play_file(const std::string& filename)
{
  return m_impl->play_file(filename);
}

void gst_sound_t::set_volume(double v)
{
  return m_impl->set_volume(v);
}
