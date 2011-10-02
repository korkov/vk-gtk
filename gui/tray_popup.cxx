#include "tray_popup.hxx"
#include "widgets.hxx"
#include <gtk/gtk.h>

using vk::tray_popup_t;

struct tray_popup_t::impl
{
  impl(const widgets_t& widgets)
    : m_menu(widgets.get("tray_menu"))
  {
    g_signal_connect(widgets.get("settings_tray_menuitem"), "activate", G_CALLBACK(settings_cb), this);
    g_signal_connect(widgets.get("exit_tray_menuitem"), "activate", G_CALLBACK(exit_cb), this);
    g_signal_connect(widgets.get("auth_tray_menuitem"), "toggled", G_CALLBACK(auth_cb), this);
  }

  void popup(uint button, uint time)
  {
    gtk_menu_popup(GTK_MENU(m_menu), NULL, NULL, NULL, NULL, button, time);
  }

  boost::signal<void()> settings_sig;  
  boost::signal<void()> exit_sig;
  boost::signal<void(bool)> auth_sig;

private:
  GtkWidget* m_menu;

  static void exit_cb(GtkWidget* item, impl* pimpl) { pimpl->exit_sig(); }
  static void settings_cb(GtkWidget* item, impl* pimpl) { pimpl->settings_sig(); }
  static void auth_cb(GtkCheckMenuItem* item, impl* pimpl) { pimpl->auth_sig(gtk_check_menu_item_get_active(item)); }
};

tray_popup_t::tray_popup_t(const widgets_t& widgets)
  : m_impl(new impl(widgets))
  , settings_sig(m_impl->settings_sig)
  , exit_sig(m_impl->exit_sig)
  , auth_sig(m_impl->auth_sig)
{
}

void tray_popup_t::popup(uint button, uint time)
{
  m_impl->popup(button, time);
}
