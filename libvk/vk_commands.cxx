#include "vk_commands.hxx"

#include <sstream>
#include <boost/foreach.hpp>
#include <boost/algorithm/string.hpp>
#include <boost/property_tree/ptree.hpp>
#include <boost/property_tree/json_parser.hpp>

#include "vk_session.hxx"
#include "vk_common.hxx"

namespace vk
{
  std::vector<message_t> parse_messages(const std::string& in_)
  {
    const std::string in = boost::replace_all_copy(in_, "\\/", "/");

    std::vector<message_t> ret;
    std::stringstream stream;
    stream << in;

    using namespace boost::property_tree;
    ptree pt;

    try
      {
        json_parser::read_json(stream, pt);
      }
    catch (const std::exception& err)
      {
        VK_LOG("json parser cannot read stream:" << err.what());
        VK_LOG(in);

        return ret;
      }

    try
      {
        BOOST_FOREACH(ptree::value_type& v, pt.get_child("response"))
          {
            try
              {
                ptree& pt = v.second;
                message_t mess;
                mess.mid = pt.get("mid", 0);
                mess.read_state = pt.get("read_state", 1);
                mess.uid = pt.get("uid", 0);
                mess.date = pt.get("date", 0);
                mess.title = pt.get<std::string>("title");
                mess.body = pt.get<std::string>("body");
                ret.push_back(mess);
              }
            catch (const std::exception& err)
              {
              }
          }
      }
    catch (const std::exception& err)
      {
        VK_LOG(err.what());
        VK_LOG(in);
      }

    return ret;
  }

  profile_t parse_profile(const std::string& in_)
  {
    const std::string in = boost::replace_all_copy(in_, "\\/", "/");

    profile_t ret;
    std::stringstream stream;
    stream << in;

    using namespace boost::property_tree;
    ptree pt;

    try
      {
        json_parser::read_json(stream, pt);
      }
    catch (const std::exception& err)
      {
        VK_LOG("json parser cannot read stream:" << err.what());
        VK_LOG(in);

        return ret;
      }

    try
      {
        BOOST_FOREACH(ptree::value_type& v, pt.get_child("response"))
          {
            try
              {
                ptree& pt = v.second;

                ret.uid = pt.get("uid", 0);
                ret.first_name = pt.get<std::string>("first_name");
                ret.last_name = pt.get<std::string>("last_name");
                ret.nickname = pt.get<std::string>("nickname");
                ret.photo = pt.get<std::string>("photo");
              }
            catch (const std::exception& err)
              {
                VK_LOG(err.what());
              }
          }
      }
    catch (const std::exception& err)
      {
        ret = profile_t();
        VK_LOG(err.what());
        VK_LOG(in);
      }
    return ret;
  }
}

namespace vk
{
  namespace commands
  {
    std::vector<message_t> get_unread_messages(const session_t& ses)
    {
      const std::string content = ses.cmd("messages.get?filters=1");
      return parse_messages(content);
    }

    void mark_as_read(const session_t& ses, const message_t& mess)
    {
      ses.cmd(VK_STR("messages.markAsRead?mids="<<mess.mid));
    }

    profile_t get_profile(const session_t& ses, unsigned uid)
    {
      const std::string content = ses.cmd(
        VK_STR("getProfiles?uids="<<uid<<
               "&fields=uid,first_name,last_name,nickname,photo"<<"&name_case=gen")
      );
      return parse_profile(content);
    }
    
    std::string get_char_page_url(unsigned uid)
    {
      return VK_STR("http://vk.com/im?sel="<<uid);
    }
  }
}
