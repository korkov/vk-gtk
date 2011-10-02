#define BOOST_TEST_MODULE vk_tests
#include <boost/test/unit_test.hpp>
#include "vk_http.hxx"
#include "vk_app.hxx"
#include "vk_common.hxx"
#include "vk_session.hxx"

using namespace vk;

BOOST_AUTO_TEST_CASE( test_http_t )
{
  http_t req("http://ya.ru");
  std::string content = req.perform();
  BOOST_CHECK(content.size() != 0);

  content = req.perform();
  BOOST_CHECK(content.size() != 0);
}

BOOST_AUTO_TEST_CASE( test_get_auth_str )
{
  const std::string in1 = "access_token=422fdddddddddddddddddddddddddddddddddd7422f8ac225a183ff91d830f7&expires_in=86400&user_id=4dd22";
  const std::string in2 = "422fdddddddddddddddddddddddddddddddddd7422f8ac225a183ff91d830f7&expires_in=86400&user_id=4dd22";
  const std::string in3 = "access_token=422fdddddddddddddddddddddddddddddddddd7422f8ac225a183ff91d830f7";
  const std::string in4 = "422fdddddddddddddddddddddddddddddddddd7422f8ac225a183ff91d830f7";
  const std::string res = "422fdddddddddddddddddddddddddddddddddd7422f8ac225a183ff91d830f7";
  BOOST_CHECK(get_auth_str(in1) == res);
  BOOST_CHECK(get_auth_str(in2) == "");
  BOOST_CHECK(get_auth_str(in3) == "");
  BOOST_CHECK(get_auth_str(in4) == "");
}

BOOST_AUTO_TEST_CASE( test_app_t )
{
#warning write tests for session_t and app_t
}

const char json_messages[] = "{"
  "\"response\":[4820,"
  "{\"mid\":9587,\"date\":1316730850,\"out\":0,\"uid\":45816,\"read_state\":1,\"title\":\"...\",\"body\":\"можешь поставить\"},"
  "{\"mid\":9584,\"date\":1316717797,\"out\":0,\"uid\":25965382,\"read_state\":1,\"title\":\"...\",\"body\":\"я старался))\"}"
  "]}";

#include "vk_commands.hxx"

BOOST_AUTO_TEST_CASE( test_parse_messages )
{
  std::vector<message_t> messages = parse_messages(json_messages);
  BOOST_CHECK(messages.size() == 2);
  const message_t& mess = messages[1];
  BOOST_CHECK(mess.mid == 9584);
  BOOST_CHECK(mess.title == "...");
  BOOST_CHECK(mess.uid == 25965382);
}
