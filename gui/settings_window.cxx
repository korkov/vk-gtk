#include "settings_window.hxx"
#include "vk_common.hxx"
#include "widgets.hxx"

using vk::settings_window_t;

struct settings_window_t::impl
{
  impl(const conf_t& conf, const widgets_t& widgets)
    : m_conf(conf)
    , m_widgets(widgets)
    , m_window(widgets.get("settings_window"))
  {
    g_signal_connect(m_window, "delete-event", G_CALLBACK(gtk_widget_hide), m_window);
    g_signal_connect(widgets.get("settings_apply_button"), "clicked", G_CALLBACK(apply_clicked_cb), this);
  }

  void show()
  {
    gtk_widget_show_all(m_window);
  }

private:
  conf_t m_conf;
  widgets_t m_widgets;
  GtkWidget* m_window;

  void apply()
  {
    gtk_widget_hide(m_window);
  }

  static void apply_clicked_cb(GtkWidget* button, impl* pimpl) { pimpl->apply(); }
};

settings_window_t::settings_window_t(const conf_t& conf, const widgets_t& widgets)
  : m_impl(new impl(conf, widgets))
{
}

void settings_window_t::show()
{
  return m_impl->show();
}
