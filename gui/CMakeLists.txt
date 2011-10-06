add_executable(vk-gtk main.cxx widgets.cxx settings_window.cxx
tray.cxx tray_popup.cxx auth.cxx notifier.cxx utils.cxx
messages_checker.cxx browsers.cxx gst_sound.cxx)

#find_package(GTK2 2.10 REQUIRED gtk glade)
#include_directories(${GTK2_INCLUDE_DIRS})

execute_process(
  COMMAND "pkg-config" "--cflags" "webkit-1.0" "gtk+-2.0" "libglade-2.0" "gstreamer-0.10"
  OUTPUT_VARIABLE PKG_CONFIG_INCLUDE_DIRS
  )
execute_process(
  COMMAND "pkg-config" "--libs" "webkit-1.0" "gtk+-2.0" "libglade-2.0" "gstreamer-0.10"
  OUTPUT_VARIABLE PKG_CONFIG_LIBRARIES_
  )
string(REGEX REPLACE "^[:blank:]*(.+)  \n" "\\1" PKG_CONFIG_LIBRARIES "${PKG_CONFIG_LIBRARIES_}")

add_definitions(${PKG_CONFIG_INCLUDE_DIRS})
add_definitions(-DPKGDATA="${PKGDATADIR}")

include_directories(${CMAKE_CURRENT_SOURCE_DIR}/../libvk)
link_directories(${CMAKE_CURRENT_SOURCE_DIR}/../libvk)

set(Boost_USE_STATIC_LIBS ON)
find_package(Boost COMPONENTS signals REQUIRED)
include_directories(${Boost_INCLUDE_DIRS})

target_link_libraries(vk-gtk ${PKG_CONFIG_LIBRARIES} ${Boost_LIBRARIES} vk notify)

install(TARGETS vk-gtk DESTINATION ${CMAKE_INSTALL_PREFIX}/bin)
