cmake_minimum_required(VERSION 3.22.1 FATAL_ERROR)

project(aerozen-zenoh-echo)

# Find Zenoh
find_package(zenohc QUIET)
find_package(zenohcxx QUIET)

# If both zenohc and zenohcpp packages are not found then
# look for these packages installed by ROS 2 zenoh_cpp_vendor package
if (NOT zenohc_FOUND AND NOT zenohcxx_FOUND)
  find_package(zenoh_cpp_vendor QUIET REQUIRED)
endif ()

add_executable(zenoh_echo zenoh_echo.cc)
target_link_libraries(zenoh_echo
  zenohcxx::zenohc
  aerozen::core
)
