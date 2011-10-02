#include "auth.hxx"

#include <fstream>
#include <gtk/gtk.h>
#include <webkit/webkit.h>

#include "vk_app.hxx"
#include "vk_session.hxx"
#include "vk_commands.hxx"
#include "vk_common.hxx"
#include "utils.hxx"

using vk::auth_t;

namespace
{
  std::string token_file()
  {
    return vk::utils::get_app_dir() + "/token";
  }
}

struct auth_t::impl
{
  impl(int client_id, int scope)
    : m_app(client_id, scope)
    , m_window(gtk_window_new(GTK_WINDOW_TOPLEVEL))
    , m_webkit(webkit_web_view_new())
  {
    gtk_window_set_title(GTK_WINDOW(m_window), _("VK Gtk"));
    gtk_window_set_icon(GTK_WINDOW(m_window), gdk_pixbuf_new_from_file(PKGDATA"/vk.png", NULL));
    gtk_window_set_default_size(GTK_WINDOW(m_window), 300, 200);
    gtk_window_set_position(GTK_WINDOW(m_window), GTK_WIN_POS_CENTER_ALWAYS);
    g_signal_connect(m_window, "delete-event", G_CALLBACK(gtk_widget_hide), m_window);
    g_signal_connect(m_webkit, "load-finished", G_CALLBACK(loaded_cb), this);
    gtk_container_add(GTK_CONTAINER(m_window), m_webkit);
  }

  void login(bool on)
  {
    if (on)
      {
        std::ifstream fin(token_file().c_str());
        std::string token;
        fin >> token;
        if (commands::check_token(token))
          {
            authorized_sig(token);
            return;
          }
      }
    else
      {
        ::unlink(token_file().c_str());
      }

    const std::string url = on ? m_app.get_url() : "http://api.vk.com/oauth/logout?client_id=2475380";
    webkit_web_view_open((WebKitWebView*)m_webkit, url.c_str());
  }

  boost::signal<void (std::string)> authorized_sig;

private:
  app_t m_app;
  GtkWidget* m_window;
  GtkWidget* m_webkit;

  void loaded()
  {
    const char* url = webkit_web_view_get_uri((WebKitWebView*)m_webkit);
    if (url == NULL)
      return;

    const std::string token = get_auth_str(url);
    if (!token.empty())
      {
        gtk_widget_hide(m_window);

        std::ofstream fout(token_file().c_str());
        fout << token;
        fout.close();

        authorized_sig(token);
      }
    else if (std::string(url).find("success=1") != std::string::npos)
      {
        gtk_widget_hide(m_window);
        authorized_sig("");
      }
    else
      {
        gtk_widget_show_all(m_window);
      }
  }

  static void loaded_cb(WebKitWebView* view, WebKitWebFrame* frame, impl* pimpl)
  {
    pimpl->loaded();
  }
};

auth_t::auth_t(int client_id, int scope)
  : m_impl(new impl(client_id, scope))
  , authorized_sig(m_impl->authorized_sig)
{
}

void auth_t::login(bool on)
{
  return m_impl->login(on);
}
