set(Boost_USE_STATIC_LIBS ON)
find_package(Boost COMPONENTS signals REQUIRED)
include_directories(${Boost_INCLUDE_DIRS})

add_library(vk vk_session.cxx vk_http.cxx vk_app.cxx vk_commands.cxx)
target_link_libraries(vk curl)
