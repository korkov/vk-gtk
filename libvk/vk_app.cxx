#include "vk_app.hxx"
#include "vk_common.hxx"
#include "vk_http.hxx"

using vk::app_t;

namespace
{
  const std::string vk_auth_url_format =
    "http://api.vk.com/oauth/authorize?"
    "client_id=%s&scope=%s&redirect_uri=http://api.vk.com/blank.html&"
    "display=wap&response_type=token";
}

struct app_t::impl
{
  impl(int client_id, int scope)
    : m_client_id(client_id)
    , m_scope(scope)
  {
  }

  std::string get_url() const
  {
    boost::format url(vk_auth_url_format);
    url % m_client_id % m_scope;
    return url.str();
  }


private:
  const int m_client_id;
  const int m_scope;
};

app_t::app_t(int client_id, int scope)
  : m_impl(new impl(client_id, scope))
{
}

std::string app_t::get_url() const
{
  return m_impl->get_url();
}
