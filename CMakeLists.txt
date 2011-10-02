cmake_minimum_required (VERSION 2.6)

option(ENABLE_TESTS "Build tests." OFF)

project (vk-gtk)
set(PKGDATADIR ${CMAKE_INSTALL_PREFIX}/share/vk-gtk)
add_definitions(-Wall)

add_subdirectory(libvk)
add_subdirectory(gui) 
add_subdirectory(data)

if (ENABLE_TESTS)
  add_subdirectory(tests)
  enable_testing()
  add_test(tests ${CMAKE_CURRENT_BINARY_DIR}/tests)
endif(ENABLE_TESTS)

install(FILES po/ru/vk-gtk.mo DESTINATION ${CMAKE_INSTALL_PREFIX}/share/locale/ru/LC_MESSAGES)

set(CPACK_GENERATOR "DEB")
set(CPACK_DEBIAN_PACKAGE_MAINTAINER "Alexander Korkov")
set(CPACK_PACKAGE_VERSION "0.3")
set(CPACK_INSTALL_PREFIX "/usr")
set(CPACK_PACKAGE_DESCRIPTION_SUMMARY "notifications for vk.com (vkontakte.ru)")
set(CPACK_PACKAGE_FILE_NAME "vk-gtk-0.3")

include(CPack)
