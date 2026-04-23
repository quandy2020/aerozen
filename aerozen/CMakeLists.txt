# Collect source files into the "sources" variable and unit test files into the
# "gtest_sources" variable.
aerozen_get_libsources_and_unittests(sources gtest_sources)

# Create the library target.
aerozen_create_core_library(SOURCES ${sources} CXX_STANDARD 17)

# Link the libraries that we always need.
target_link_libraries(${PROJECT_LIBRARY_TARGET_NAME} zenohcxx::zenohc)
if(TARGET UUID::UUID)
  target_link_libraries(${PROJECT_LIBRARY_TARGET_NAME} UUID::UUID)
endif()
target_include_directories(${PROJECT_LIBRARY_TARGET_NAME}
  PRIVATE
    ${PROJECT_SOURCE_DIR})
target_include_directories(${PROJECT_LIBRARY_TARGET_NAME}
  SYSTEM PUBLIC
    $<TARGET_PROPERTY:protobuf::libprotobuf,INTERFACE_INCLUDE_DIRECTORIES>)

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
add_subdirectory(cmd)
