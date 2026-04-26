set(_example_proto "msgs/stringmsg.proto")
set(_example_proto_src "${CMAKE_BINARY_DIR}/msgs/stringmsg.pb.cc")
set(_example_proto_hdr "${CMAKE_BINARY_DIR}/msgs/stringmsg.pb.h")

if(TARGET protobuf::protoc)
  set(_example_protoc_cmd protobuf::protoc)
elseif(PROTOBUF_PROTOC_EXECUTABLE)
  set(_example_protoc_cmd "${PROTOBUF_PROTOC_EXECUTABLE}")
else()
  message(FATAL_ERROR
    "Could not find protoc executable (protobuf::protoc / PROTOBUF_PROTOC_EXECUTABLE)")
endif()

add_custom_command(
  OUTPUT "${_example_proto_src}" "${_example_proto_hdr}"
  COMMAND "${CMAKE_COMMAND}" -E make_directory "${CMAKE_BINARY_DIR}/msgs"
  COMMAND "${_example_protoc_cmd}"
    --cpp_out=${CMAKE_BINARY_DIR}
    --proto_path=.
    "${_example_proto}"
  WORKING_DIRECTORY "${CMAKE_CURRENT_SOURCE_DIR}/.."
  DEPENDS "${CMAKE_CURRENT_SOURCE_DIR}/stringmsg.proto"
  COMMENT "Generating example protobuf ${_example_proto}"
  VERBATIM
)

add_custom_target(protobuf_compilation
  DEPENDS "${_example_proto_src}" "${_example_proto_hdr}")

set(PROTO_SRC "${_example_proto_src}" PARENT_SCOPE)
set(PROTO_HEADER "${_example_proto_hdr}" PARENT_SCOPE)
