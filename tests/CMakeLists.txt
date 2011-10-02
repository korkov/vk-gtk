add_executable (tests tests.cxx)

set(Boost_USE_STATIC_LIBS ON)
find_package (Boost COMPONENTS unit_test_framework signals REQUIRED)
include_directories(${Boost_INCLUDE_DIRS})

include_directories (${CMAKE_CURRENT_SOURCE_DIR}/../libvk)
link_directories (${CMAKE_CURRENT_SOURCE_DIR}/../libvk)
target_link_libraries (tests ${Boost_LIBRARIES} vk)
