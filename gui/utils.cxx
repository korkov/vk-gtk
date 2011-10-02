#include "utils.hxx"

#include <libnotify/notify.h>
#include <fstream>
#include <libgen.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>
#include <stdlib.h>
#include <string.h>

#include <gtk/gtk.h>

namespace vk
{
  namespace utils
  {
    std::string base_name(const std::string& str)
    {
      char tmp[PATH_MAX] = "";
      ::strncpy(tmp, str.c_str(), sizeof(tmp));
      tmp[sizeof(tmp)-1] = '\0';
      return ::basename(tmp);
    }

    bool not_empty_file_exists(const std::string& path)
    {
      struct ::stat st;
      return (stat(path.c_str(),& st) == 0) && (st.st_size != 0);
    }

    std::string get_app_dir()
    {
      const char* home_env = ::getenv("HOME");
      const std::string home = home_env ? home_env : "/tmp";;
      return home + "/.vk-gtk";
    }

    void show_warning(const std::string& mess)
    {
      GtkWidget* dialog = gtk_message_dialog_new (
        NULL,
        GTK_DIALOG_DESTROY_WITH_PARENT,
        GTK_MESSAGE_ERROR,
        GTK_BUTTONS_CLOSE,
        mess.c_str()
      );

      gtk_window_set_title(GTK_WINDOW(dialog), "VK-GTK");
      gtk_window_set_skip_taskbar_hint(GTK_WINDOW(dialog), false);
      gtk_window_set_skip_pager_hint(GTK_WINDOW(dialog), false);
      gtk_window_set_keep_above(GTK_WINDOW(dialog), true);
      gtk_dialog_run (GTK_DIALOG (dialog));
      gtk_widget_destroy (dialog);
    }
  }
}
