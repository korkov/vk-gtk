#include "browsers.hxx"

#include <vector>
#include <sstream>
#include <fstream>
#include <glib.h>
#include <sys/types.h>
#include <dirent.h>
#include <unistd.h>
#include <libgen.h>

#include <boost/foreach.hpp>
#include <boost/property_tree/ptree.hpp>
#include <boost/property_tree/json_parser.hpp>

#include "vk_common.hxx"

namespace browsers_private
{
  std::vector<int> get_pids(const std::string& process)
  {
    std::vector<int> ret;

    gchar* std_output = NULL;
    gchar* std_errors = NULL;
    gint status = 0;
    GError *error = NULL;


    const std::string cmd = std::string("pidof ")+process;
    gboolean spawn_ok = g_spawn_command_line_sync(
      cmd.c_str(),
      &std_output,
      &std_errors,
      &status,
      &error
    );
    if (!spawn_ok)
      throw VK_ERROR_NUM(_("cannot exec")<<" '"<<cmd<<"'");

    std::stringstream stream;
    stream << std_output;

    while (stream)
      {
        int pid = 0;
        if (stream >> pid)
          ret.push_back(pid);
      }

    return ret;
  }

  std::string dir_name(const std::string& str)
  {
    char tmp[PATH_MAX];
    strncpy(tmp, str.c_str(), sizeof(tmp));
    tmp[sizeof(tmp)-1] = '\0';
    return ::dirname(tmp);
  }


  bool tabs_contain_url(const std::vector<std::string>& tabs, const std::string& url_part)
  {
//     VK_LOG("opened tabs:");
//     BOOST_FOREACH(const std::string& tab, tabs)
//       VK_LOG("  "<<tab);

    BOOST_FOREACH(const std::string& tab, tabs)
      if (tab.find(url_part) != std::string::npos)
        return true;

    return false;
  }

  std::string read_link(const std::string& link)
  {
    char path[PATH_MAX];
    const ssize_t size = ::readlink(link.c_str(), path, sizeof(path));
    path[sizeof(path)-1] = '\0';

    if (size == -1)
      throw VK_ERROR_NUM(_("cannot read link")<<" "<<link);

    return path;
  }

  std::vector<std::string> opened_files(int pid)
  {
    const std::string path = VK_STR("/proc/"<<pid<<"/fd");

    struct dir_holder_t {
      dir_holder_t(DIR* dir_): dir(dir_) {}
      ~dir_holder_t() { if (dir) ::closedir(dir); }
      DIR* dir;
    };

    dir_holder_t dir_holder(opendir(path.c_str()));

    if (dir_holder.dir == NULL)
      throw VK_ERROR_NUM(_("cannot read")<<" "<<path);

    std::vector<std::string> ret;
    while (true)
      {
        struct dirent* dirp = readdir(dir_holder.dir);
        if (dirp == NULL)
          break;

        if (dirp->d_name == NULL ||
            std::string(dirp->d_name) == "" ||
            std::string(dirp->d_name) == "." ||
            std::string(dirp->d_name) == "..")
          continue;

        std::string linkname = path + "/" + dirp->d_name;
        try
          {
            ret.push_back(read_link(linkname));
          }
        catch (const std::exception& err)
          {
            VK_LOG(err.what());
          }
      }


    return ret;
  }

  namespace firefox
  {
    std::string search_profile_dir(int pid)
    {
      const std::string search_file = "extensions.sqlite";

      const std::vector<std::string> files = opened_files(pid);
      BOOST_FOREACH(const std::string& file, files)
        if (file.find(search_file) != std::string::npos)
          return dir_name(file);

      throw VK_ERROR(_("cannot find")<<" "<<search_file);
      return "";
    }

    std::map<int, std::string> profile_dirs;

    std::string get_profile_dir(int pid)
    {
      if (profile_dirs[pid].empty())
        profile_dirs[pid] = search_profile_dir(pid);
      return profile_dirs[pid];
    }

    std::string get_session_file(const std::string& profile_dir)
    {
      return profile_dir + "/sessionstore.js";
    }

    std::vector<std::string> get_tabs_my(const std::string& session_file)
    {
      std::vector<std::string> ret;

      std::ifstream fin(session_file.c_str());
      std::stringstream stream;
      stream << fin.rdbuf();

      std::string content = stream.str();
      size_t pos = content.find("\"tabs\":");

      size_t counter = 0;
      while (true)
        {
          pos = content.find_first_of("[]", pos);
          if (pos == std::string::npos)
            break;
          if (content[pos] == '[')
            counter++;
          if (content[pos] == ']')
            counter--;
          if (counter == 0)
            break;
          pos++;
        }

      content = content.substr(0, pos);

      pos = 0;
      while (true)
        {
          const std::string url_token = "\"url\":\"";
          pos = content.find(url_token, pos);
          if (pos == std::string::npos)
            break;
          pos += url_token.size();

          const size_t url_end = content.find("\"", pos);
          ret.push_back(content.substr(pos, url_end-pos));
        }

      return ret;
    }


    std::vector<std::string> get_tabs_json(const std::string& session_file)
    {
      std::vector<std::string> ret;

      using namespace boost::property_tree;
      ptree pt;

      try
        {
          json_parser::read_json(session_file, pt);
        }
      catch (const std::exception& err)
        {
          VK_LOG("json parser: " << err.what());
          return ret;
        }

      try
        {
          BOOST_FOREACH(ptree::value_type& v, pt.get_child("windows"))
            {
              try
                {
                  ptree& pt = v.second;
                  BOOST_FOREACH(ptree::value_type& v, pt.get_child("tabs"))
                    {
                      try
                        {
                          ptree& pt = v.second;
                          BOOST_FOREACH(ptree::value_type& v, pt.get_child("entries"))
                            {
                              ptree& pt = v.second;
                              ret.push_back(pt.get<std::string>("url"));
                            }
                        }
                      catch (const std::exception& err)
                        {
                          VK_LOG("json session parser(1): "<<err.what());
                        }
                    }
                }
              catch (const std::exception& err)
                {
                  VK_LOG("json session parser(2): "<<err.what());
                }
            }
        }
      catch (const std::exception& err)
        {
          VK_LOG("json session parser(3): "<<err.what());
        }

      return ret;
    }

    bool tab_opened(const std::string& url_part)
    {
      const std::vector<int> pids = get_pids("firefox-bin");
      BOOST_FOREACH(int pid, pids)
        if (tabs_contain_url(get_tabs_my(get_session_file(get_profile_dir(pid))), url_part))
          return true;

      return false;
    }
  }

  namespace chrome
  {
    bool tab_opened(const std::string& url_part)
    {
      //#warning TODO
      return false;
    }
  }

  namespace opera
  {
    bool tab_opened(const std::string& url_part)
    {
      //#warning TODO
      return false;
    }
  }

  namespace konqueror
  {
    bool tab_opened(const std::string& url_part)
    {
      //#warning TODO
      return false;
    }
  }

  namespace ie
  {
    bool tab_opened(const std::string& url_part)
    {
      //#warning TODO
      return false;
    }
  }
}

namespace browsers
{
  bool tab_opened(browser_t br, const std::string& url_part)
  {
    using namespace browsers_private;
    switch (br)
      {
      case browser_firefox: return firefox::tab_opened(url_part);
      case browser_chrome: return chrome::tab_opened(url_part);
      case browser_opera: return opera::tab_opened(url_part);
      case browser_konqueror: return konqueror::tab_opened(url_part);
      case browser_ie: return ie::tab_opened(url_part);
      };
    VK_ASSERT(!"unknown browser");
    return false;
  }  
}
