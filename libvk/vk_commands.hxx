#ifndef VK_1316734770
#define VK_1316734770

#include <time.h>
#include <string>
#include <vector>

namespace vk
{
  struct session_t;

  struct message_t
  {
    message_t(): mid(0) {}

    unsigned mid;
    bool read_state;
    unsigned uid;
    time_t date;
    std::string title;
    std::string body;
  };

  struct profile_t
  {
    profile_t(): uid(0) {}

    unsigned uid;
    std::string first_name;
    std::string last_name;
    std::string nickname;
    std::string photo;
  };

  namespace commands
  {
    std::vector<message_t> get_unread_messages(const session_t& ses);
    void mark_as_read(const session_t& ses, const message_t& mess);
    profile_t get_profile(const session_t& ses, unsigned uid);
    std::string get_char_page_url(unsigned uid);
  }
}

namespace vk
{
  std::vector<message_t> parse_messages(const std::string& in);
}

#endif  // VK_1316734770
