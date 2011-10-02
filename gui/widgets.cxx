#include "widgets.hxx"
#include "vk_common.hxx"

using vk::widgets_t;

struct widgets_t::impl
{
  impl(const std::string& file)
    : m_builder(gtk_builder_new())
  {
    GError* error = NULL;
    if (!gtk_builder_add_from_file(m_builder, file.c_str(), &error))
      throw VK_ERROR(_("cannot load builder file") << ": " << error->message);
  }

  GtkWidget* get(const char *name) const
  {
    GtkWidget* ret = GTK_WIDGET(gtk_builder_get_object(m_builder, name));
    if (ret == NULL)
      throw VK_ERROR(_("cannot find widget") << ": '" << name << "'");
    return ret;
  }

private:
  GtkBuilder* m_builder;
};

widgets_t::widgets_t(const std::string& file)
  : m_impl(new impl(file))
{
}

GtkWidget* widgets_t::get(const char *name) const
{
  return m_impl->get(name);
}
