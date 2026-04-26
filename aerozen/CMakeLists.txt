# Collect source files into the "sources" variable and unit test files into the
# "gtest_sources" variable.
aerozen_get_libsources_and_unittests(sources gtest_sources)

# Generate config header expected by sources.
file(MAKE_DIRECTORY "${PROJECT_BINARY_DIR}/include/aerozen")
configure_file(
  "${CMAKE_CURRENT_SOURCE_DIR}/config.hpp.in"
  "${PROJECT_BINARY_DIR}/include/aerozen/config.hpp")

# Generate protobuf sources/headers from local proto files.
file(GLOB aerozen_proto_files "${CMAKE_CURRENT_SOURCE_DIR}/proto/*.proto")
set(aerozen_proto_srcs)
set(aerozen_proto_hdrs)
set(aerozen_proto_gen_dir "${CMAKE_CURRENT_BINARY_DIR}/generated")

if(TARGET protobuf::protoc)
  set(_aerozen_protoc_cmd protobuf::protoc)
endif()
if(NOT _aerozen_protoc_cmd AND PROTOBUF_PROTOC_EXECUTABLE)
  set(_aerozen_protoc_cmd "${PROTOBUF_PROTOC_EXECUTABLE}")
endif()
if(NOT _aerozen_protoc_cmd)
  message(FATAL_ERROR
    "Could not find protoc executable (protobuf::protoc / PROTOBUF_PROTOC_EXECUTABLE)")
endif()

foreach(_proto ${aerozen_proto_files})
  file(RELATIVE_PATH _proto_rel "${PROJECT_SOURCE_DIR}" "${_proto}")
  get_filename_component(_proto_dir "${_proto_rel}" DIRECTORY)
  get_filename_component(_proto_we "${_proto_rel}" NAME_WE)
  set(_out_cc "${aerozen_proto_gen_dir}/${_proto_dir}/${_proto_we}.pb.cc")
  set(_out_h  "${aerozen_proto_gen_dir}/${_proto_dir}/${_proto_we}.pb.h")

  add_custom_command(
    OUTPUT "${_out_cc}" "${_out_h}"
    COMMAND "${CMAKE_COMMAND}" -E make_directory "${aerozen_proto_gen_dir}/${_proto_dir}"
    COMMAND "${_aerozen_protoc_cmd}"
      --cpp_out=${aerozen_proto_gen_dir}
      --proto_path=${PROJECT_SOURCE_DIR}
      "${_proto_rel}"
    DEPENDS "${_proto}"
    COMMENT "Generating protobuf ${_proto_rel}"
    VERBATIM
  )

  list(APPEND aerozen_proto_srcs "${_out_cc}")
  list(APPEND aerozen_proto_hdrs "${_out_h}")
endforeach()

list(APPEND sources ${aerozen_proto_srcs})
list(APPEND sources
  "${CMAKE_CURRENT_SOURCE_DIR}/utils/environment.cpp"
  "${CMAKE_CURRENT_SOURCE_DIR}/installation_directories.cpp")

# Create the library target.
aerozen_create_core_library(SOURCES ${sources} CXX_STANDARD 17)

# Link the libraries that we always need.
if(HAVE_ZENOH AND TARGET zenohcxx::zenohc)
  target_link_libraries(${PROJECT_LIBRARY_TARGET_NAME} zenohcxx::zenohc)
endif()
if(TARGET UUID::UUID)
  target_link_libraries(${PROJECT_LIBRARY_TARGET_NAME} UUID::UUID)
endif()
target_include_directories(${PROJECT_LIBRARY_TARGET_NAME}
  PRIVATE
    ${PROJECT_SOURCE_DIR}
    ${CMAKE_CURRENT_BINARY_DIR}
    ${aerozen_proto_gen_dir})
target_include_directories(${PROJECT_LIBRARY_TARGET_NAME}
  SYSTEM PUBLIC
    $<TARGET_PROPERTY:protobuf::libprotobuf,INTERFACE_INCLUDE_DIRECTORIES>)
include_directories("${aerozen_proto_gen_dir}")

if(TARGET protobuf::libprotobuf)
  target_link_libraries(${PROJECT_LIBRARY_TARGET_NAME} protobuf::libprotobuf)
endif()
if(TARGET cppzmq::cppzmq)
  target_link_libraries(${PROJECT_LIBRARY_TARGET_NAME} cppzmq::cppzmq)
endif()
if(TARGET ZeroMQ::ZeroMQ)
  target_link_libraries(${PROJECT_LIBRARY_TARGET_NAME} ZeroMQ::ZeroMQ)
endif()
if(DEFINED ZeroMQ_TARGET AND TARGET ${ZeroMQ_TARGET})
  target_link_libraries(${PROJECT_LIBRARY_TARGET_NAME} ${ZeroMQ_TARGET})
endif()
if(ZeroMQ_LIBRARIES)
  target_link_libraries(${PROJECT_LIBRARY_TARGET_NAME} ${ZeroMQ_LIBRARIES})
endif()
target_link_libraries(${PROJECT_LIBRARY_TARGET_NAME} zmq)

# Build the unit tests.
set(unit_test_lib_deps ${EXTRA_TEST_LIB_DEPS})
if(TARGET test_config)
  list(APPEND unit_test_lib_deps test_config)
endif()

aerozen_build_tests(TYPE UNIT SOURCES ${gtest_sources}
  TEST_LIST test_list
  LIB_DEPS ${unit_test_lib_deps})

foreach(test ${test_list})
  set(_env_vars)
  list(APPEND _env_vars "AEROZEN_IP=127.0.0.1")
  list(APPEND _env_vars "AEROZEN_VERBOSE=1")
  set_property(TEST ${test} PROPERTY ENVIRONMENT "${_env_vars}")
endforeach()

# Command line support.
if(EXISTS "${CMAKE_CURRENT_SOURCE_DIR}/cmd/CMakeLists.txt")
  add_subdirectory(cmd)
endif()
