#include "vk_http.hxx"
#include "vk_common.hxx"
#include <boost/noncopyable.hpp>
#include <curl/curl.h>

using vk::http_t;

struct curl_hander_t: boost::noncopyable
{
  curl_hander_t()
    : m_handle(curl_easy_init())
  {
  }

  ~curl_hander_t()
  {
    curl_easy_cleanup(m_handle);
  }

  void reset()
  {
    if (m_handle)
      curl_easy_cleanup(m_handle);
    m_handle = curl_easy_init();
    VK_ASSERT(m_handle != NULL);
  }

  CURL* get() const
  {
    return m_handle;
  }

private:
  CURL* m_handle;
};


struct http_t::impl
{
  impl(const std::string& url)
    : m_url(url)
  {
  }

  void set_url(const std::string& url)
  {
    m_url = url;
  }

  std::string perform()
  {
    VK_ASSERT(m_url.size());

    std::string content;

    curl_easy_setopt(m_handle.get(), CURLOPT_URL, m_url.c_str());
    curl_easy_setopt(m_handle.get(), CURLOPT_PROXY, "");
    curl_easy_setopt(m_handle.get(), CURLOPT_WRITEFUNCTION, curl_write_func);
    curl_easy_setopt(m_handle.get(), CURLOPT_WRITEDATA, &content);
    curl_easy_setopt(m_handle.get(), CURLOPT_FOLLOWLOCATION, 1);

    const CURLcode result = curl_easy_perform(m_handle.get());
    if (result != CURLE_OK)
      throw VK_ERROR(curl_easy_strerror(result));

    const int reply = 200;
    curl_easy_getinfo(m_handle.get(), CURLINFO_RESPONSE_CODE, &reply);
    return content;
  }

  void clear()
  {
    m_handle.reset();
  }

private:
  curl_hander_t m_handle;
  std::string m_url;

  static size_t curl_write_func(char* data, size_t size, size_t nmemb, std::string* buf)
  {
    buf->append(data, size * nmemb);
    return size * nmemb;
  }
};

http_t::http_t(const std::string& url)
  : m_impl(new impl(url))
{
}

void http_t::set_url(const std::string& url)
{
  return m_impl->set_url(url);
}

std::string http_t::perform()
{
  return m_impl->perform();
}

void http_t::clear()
{
  return m_impl->clear();
}
