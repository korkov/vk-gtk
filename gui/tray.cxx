#include "tray.hxx"

#include <boost/bind.hpp>
#include <gtk/gtk.h>
#include "vk_common.hxx"

using vk::tray_t;

struct tray_t::impl
{
  /// signals
  boost::signal<void ()> activated;
  boost::signal<void (uint button, uint act_time)> popup_menu_activated;

  impl()
  {
    m_icon = gtk_status_icon_new();
    g_signal_connect(m_icon, "activate", G_CALLBACK(activate_cb), this);
    g_signal_connect(m_icon, "popup-menu", G_CALLBACK(popup_menu_cb), this);
  }

  void set_pixbuf(GdkPixbuf* pixbuf)
  {
    gtk_status_icon_set_from_pixbuf(m_icon, pixbuf);
  }

  void set_tooltip(const std::string& text)
  {
    g_object_set(m_icon, "tooltip-text", text.c_str(), NULL);
  }

private:
  GtkStatusIcon* m_icon;

  /// static callbacks
  static void activate_cb(GtkStatusIcon* icon, impl* pimpl) { pimpl->activated(); }
  static void popup_menu_cb(
    GtkStatusIcon* status_icon,
    guint button, guint activate_time, impl* pimpl)
  {
    pimpl->popup_menu_activated(button, activate_time);
  }
};


tray_t::tray_t()
  : pimpl(new impl())
  , activated(pimpl->activated)
  , popup_menu_activated(pimpl->popup_menu_activated)
{
}

void tray_t::set_pixbuf(GdkPixbuf* pixbuf)
{
  pimpl->set_pixbuf(pixbuf);
}

void tray_t::set_tooltip(const std::string& text)
{
  pimpl->set_tooltip(text);
}
