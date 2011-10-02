#include "vk_session.hxx"
#include "vk_common.hxx"
#include "vk_http.hxx"
#include <string.h>

using vk::session_t;

namespace
{
  const std::string vk_url = "https://api.vk.com/method/";
}

namespace vk
{
  std::string get_auth_str(const std::string in)
  {
    const char begin_id[] = "en=";
    const char end_id[] = "&ex";

    const size_t begin = in.find(begin_id);
    const size_t end = in.find(end_id);

    if ((begin == std::string::npos) ||
        (end == std::string::npos))
      return "";

    const size_t auth_pos = begin + ::strlen(begin_id);
    return in.substr(auth_pos, (end - auth_pos));
  }
}


struct session_t::impl
{
  bool active() const
  {
    return !m_token.empty();
  }

  void set_token(const std::string& token)
  {
    m_token = token;
    activated_sig(!token.empty());
  }
  
  std::string cmd(const std::string& str) const
  {
    http_t req;
    req.set_url(vk_url + str + "&access_token=" + m_token);
    return req.perform();
  }

private:
  std::string m_token;
public:
  boost::signal<void (bool)> activated_sig;
};

session_t::session_t()
  : m_impl(new impl())
  , activated_sig(m_impl->activated_sig)
{
}

std::string session_t::cmd(const std::string& str) const
{
  return m_impl->cmd(str);
}

bool session_t::active() const
{
  return m_impl->active();
}

void session_t::set_token(const std::string& token)
{
  return m_impl->set_token(token);
}
