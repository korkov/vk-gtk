#ifndef VK_1316644539
#define VK_1316644539

#include <string>
#include <vector>
#include <stdexcept>
#include <map>
#include <assert.h>
#include <strings.h>
#include <errno.h>
#include <sstream>
#include <libintl.h>
#include <boost/shared_ptr.hpp>
#include <iostream>
#include <boost/format.hpp>

#define _(WHAT) gettext(WHAT)
#define VK_STR(WHAT) ({ std::stringstream e; e << WHAT; e.str(); })
#define VK_LOG(WHAT) do { std::cout << VK_STR(WHAT) << std::endl; } while (0)
#define VK_LOG_NUM(WHAT) VK_LOG(WHAT << " - " << strerror(errno))
#define VK_ERROR(WHAT) (std::runtime_error(VK_STR(WHAT)))
#define VK_ERROR_NUM(WHAT) (std::runtime_error(VK_STR(WHAT << " - " << strerror(errno))))

#define VK_ASSERT(COND)                                                 \
  do                                                                    \
    {                                                                   \
      const std::string mess = VK_STR(__FILE__<<":"<<__LINE__<<": "<<_("ERROR") << ": '" << #COND << "' " << _("failed")); \
      if (!(COND)) VK_LOG(mess);                                        \
      assert(COND);                                                     \
      if (!(COND)) throw VK_ERROR(mess);                                \
    } while (0)

#endif  // VK_1316644539
