cmake_minimum_required(VERSION 3.22.1 FATAL_ERROR)

project(aerozen-examples)

# Use package mode only when examples are configured standalone.
if(CMAKE_SOURCE_DIR STREQUAL CMAKE_CURRENT_SOURCE_DIR)
  find_package(aerozen QUIET REQUIRED OPTIONAL_COMPONENTS log)
endif()

if(TARGET aerozen::core)
  set(AEROZEN_EXAMPLE_CORE_TARGET aerozen::core)
elseif(TARGET aerozen::aerozen)
  set(AEROZEN_EXAMPLE_CORE_TARGET aerozen::aerozen)
elseif(TARGET aerozen)
  set(AEROZEN_EXAMPLE_CORE_TARGET aerozen)
else()
  message(FATAL_ERROR "Could not resolve aerozen core target for examples")
endif()

if (EXISTS "${CMAKE_CURRENT_SOURCE_DIR}/msgs/")
  # Message generation. Only required when using custom Protobuf messages.
  find_package(Protobuf REQUIRED)
  add_subdirectory(msgs)
  set_source_files_properties(${PROTO_SRC} ${PROTO_HEADER} PROPERTIES GENERATED TRUE)
endif()

include_directories(${CMAKE_BINARY_DIR})
include_directories(${CMAKE_SOURCE_DIR})
include_directories(${CMAKE_SOURCE_DIR}/aerozen)
include_directories(${CMAKE_BINARY_DIR}/aerozen/generated)

# Generate examples
if (NOT MSVC AND EXISTS "${CMAKE_CURRENT_SOURCE_DIR}/publisher_c.cpp")
  add_executable(publisher_c publisher_c.cpp)
  target_link_libraries(publisher_c ${AEROZEN_EXAMPLE_CORE_TARGET})
endif()
if (NOT MSVC AND EXISTS "${CMAKE_CURRENT_SOURCE_DIR}/publisher_c_fast.cpp")
  add_executable(publisher_c_fast publisher_c_fast.cpp)
  target_link_libraries(publisher_c_fast ${AEROZEN_EXAMPLE_CORE_TARGET})
endif()

if (EXISTS "${CMAKE_CURRENT_SOURCE_DIR}/subscriber_c.cpp")
  add_executable(subscriber_c subscriber_c.cpp)
  target_link_libraries(subscriber_c ${AEROZEN_EXAMPLE_CORE_TARGET})
endif()

if (NOT MSVC AND EXISTS "${CMAKE_CURRENT_SOURCE_DIR}/subscriber_c_slow.cpp")
  add_executable(subscriber_c_slow subscriber_c_slow.cpp)
  target_link_libraries(subscriber_c_slow ${AEROZEN_EXAMPLE_CORE_TARGET})
endif()

if (EXISTS "${CMAKE_CURRENT_SOURCE_DIR}/subscriber_stats.cpp")
  add_executable(subscriber_stats subscriber_stats.cpp)
  target_link_libraries(subscriber_stats ${AEROZEN_EXAMPLE_CORE_TARGET})
endif()

if (EXISTS "${CMAKE_CURRENT_SOURCE_DIR}/publisher.cpp")
  add_executable(publisher publisher.cpp)
  target_link_libraries(publisher ${AEROZEN_EXAMPLE_CORE_TARGET})
endif()

if (EXISTS "${CMAKE_CURRENT_SOURCE_DIR}/subscriber.cpp")
  add_executable(subscriber subscriber.cpp)
  target_link_libraries(subscriber ${AEROZEN_EXAMPLE_CORE_TARGET})
endif()

if (EXISTS "${CMAKE_CURRENT_SOURCE_DIR}/inproc.cpp")
  add_executable(inproc inproc.cpp)
  target_link_libraries(inproc ${AEROZEN_EXAMPLE_CORE_TARGET})
endif()

if (EXISTS "${CMAKE_CURRENT_SOURCE_DIR}/subscriber_generic.cpp")
  add_executable(subscriber_generic subscriber_generic.cpp)
  target_link_libraries(subscriber_generic
    ${AEROZEN_EXAMPLE_CORE_TARGET})
endif()

if (EXISTS "${CMAKE_CURRENT_SOURCE_DIR}/responser.cpp")
  add_executable(responser responser.cpp)
  target_link_libraries(responser ${AEROZEN_EXAMPLE_CORE_TARGET})
endif()

if (EXISTS "${CMAKE_CURRENT_SOURCE_DIR}/requester.cpp")
  add_executable(requester requester.cpp)
  target_link_libraries(requester ${AEROZEN_EXAMPLE_CORE_TARGET})
endif()

if (EXISTS "${CMAKE_CURRENT_SOURCE_DIR}/requester_async.cpp")
  add_executable(requester_async requester_async.cpp)
  target_link_libraries(requester_async
    ${AEROZEN_EXAMPLE_CORE_TARGET})
endif()

if (EXISTS "${CMAKE_CURRENT_SOURCE_DIR}/responser_oneway.cpp")
  add_executable(responser_oneway responser_oneway.cpp)
  target_link_libraries(responser_oneway
    ${AEROZEN_EXAMPLE_CORE_TARGET})
endif()

if (EXISTS "${CMAKE_CURRENT_SOURCE_DIR}/requester_oneway.cpp")
  add_executable(requester_oneway requester_oneway.cpp)
  target_link_libraries(requester_oneway
    ${AEROZEN_EXAMPLE_CORE_TARGET})
endif()

if (EXISTS "${CMAKE_CURRENT_SOURCE_DIR}/requester_raw.cpp")
  add_executable(requester_raw requester_raw.cpp)
  target_link_libraries(requester_raw
    ${AEROZEN_EXAMPLE_CORE_TARGET})
endif()

if (EXISTS "${CMAKE_CURRENT_SOURCE_DIR}/responser_no_input.cpp")
  add_executable(responser_no_input responser_no_input.cpp)
  target_link_libraries(responser_no_input
    ${AEROZEN_EXAMPLE_CORE_TARGET})
endif()

if (EXISTS "${CMAKE_CURRENT_SOURCE_DIR}/requester_no_input.cpp")
  add_executable(requester_no_input requester_no_input.cpp)
  target_link_libraries(requester_no_input
    ${AEROZEN_EXAMPLE_CORE_TARGET})
endif()

if (EXISTS "${CMAKE_CURRENT_SOURCE_DIR}/requester_async_no_input.cpp")
  add_executable(requester_async_no_input requester_async_no_input.cpp)
  target_link_libraries(requester_async_no_input
    ${AEROZEN_EXAMPLE_CORE_TARGET})
endif()

# FIXME(chapulina) Linking failure on Windows:
# msgs\stringmsg.pb.cc : fatal error LNK1107: invalid or corrupt file: cannot read at 0x3028
if (NOT MSVC AND TARGET protobuf_compilation AND
    EXISTS "${CMAKE_CURRENT_SOURCE_DIR}/publisher_custom_msg.cpp")
  add_executable(publisher_custom_msg
    publisher_custom_msg.cpp
    ${PROTO_SRC})
  target_link_libraries(publisher_custom_msg
    ${AEROZEN_EXAMPLE_CORE_TARGET})
  add_dependencies(publisher_custom_msg protobuf_compilation)
endif()

# FIXME(chapulina) Linking failure on Windows:
# msgs\stringmsg.pb.cc : fatal error LNK1107: invalid or corrupt file: cannot read at 0x3028
if (NOT MSVC AND TARGET protobuf_compilation AND
    EXISTS "${CMAKE_CURRENT_SOURCE_DIR}/subscriber_custom_msg.cpp")
  add_executable(subscriber_custom_msg
    subscriber_custom_msg.cpp
    ${PROTO_SRC})
  target_link_libraries(subscriber_custom_msg
    ${AEROZEN_EXAMPLE_CORE_TARGET})
  add_dependencies(subscriber_custom_msg protobuf_compilation)
endif()

if (EXISTS "${CMAKE_CURRENT_SOURCE_DIR}/publisher_raw.cpp")
  add_executable(publisher_raw publisher_raw.cpp)
  target_link_libraries(publisher_raw
    ${AEROZEN_EXAMPLE_CORE_TARGET}
  )
endif()

if (EXISTS "${CMAKE_CURRENT_SOURCE_DIR}/subscriber_raw.cpp")
  add_executable(subscriber_raw subscriber_raw.cpp)
  target_link_libraries(subscriber_raw
    ${AEROZEN_EXAMPLE_CORE_TARGET}
  )
endif()

# Logging examples.
if (TARGET aerozen::log)

  if (EXISTS "${CMAKE_CURRENT_SOURCE_DIR}/record.cpp")
    add_executable(log_record record.cpp)
    target_link_libraries(log_record
      aerozen::log
    )
  endif()

  if (EXISTS "${CMAKE_CURRENT_SOURCE_DIR}/playback.cpp")
    add_executable(log_playback playback.cpp)
    target_link_libraries(log_playback
      aerozen::log
    )
  endif()

  if (EXISTS "${CMAKE_CURRENT_SOURCE_DIR}/custom_query.cpp")
    add_executable(log_custom_query custom_query.cpp)
    target_link_libraries(log_custom_query
      aerozen::log
    )
  endif()

endif()


if (MSVC)
  # Suppress Protobuf message generation warnings.
  set (CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} /wd4018 /wd4100 /wd4127 /wd4244 /wd4267 /wd4512")

  # Suppress the "decorated name length exceed" warning (inside the STL).
  set (CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} /wd4503")

  add_definitions(-DBUILDING_STATIC_LIBS -DWIN32_LEAN_AND_MEAN)

  # Don't pull in the Windows min/max macros
  add_definitions(-DNOMINMAX)

endif()
