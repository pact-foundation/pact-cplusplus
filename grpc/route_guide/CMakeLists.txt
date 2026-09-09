cmake_minimum_required(VERSION 3.22)

project(RouteGuide LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

find_package(absl CONFIG REQUIRED)
find_package(Protobuf CONFIG REQUIRED)
find_package(gRPC CONFIG REQUIRED)

set(route_guide_proto "${CMAKE_CURRENT_SOURCE_DIR}/route_guide.proto")
set(route_guide_proto_src "${CMAKE_CURRENT_BINARY_DIR}/route_guide.pb.cc")
set(route_guide_proto_hdr "${CMAKE_CURRENT_BINARY_DIR}/route_guide.pb.h")
set(route_guide_grpc_src "${CMAKE_CURRENT_BINARY_DIR}/route_guide.grpc.pb.cc")
set(route_guide_grpc_hdr "${CMAKE_CURRENT_BINARY_DIR}/route_guide.grpc.pb.h")

add_custom_command(
  OUTPUT
    "${route_guide_proto_src}"
    "${route_guide_proto_hdr}"
    "${route_guide_grpc_src}"
    "${route_guide_grpc_hdr}"
  COMMAND protobuf::protoc
  ARGS
    --proto_path "${CMAKE_CURRENT_SOURCE_DIR}"
    --cpp_out "${CMAKE_CURRENT_BINARY_DIR}"
    --grpc_out "${CMAKE_CURRENT_BINARY_DIR}"
    --plugin=protoc-gen-grpc=$<TARGET_FILE:gRPC::grpc_cpp_plugin>
    "${route_guide_proto}"
  DEPENDS "${route_guide_proto}"
  VERBATIM)

add_library(route_guide_proto
  "${route_guide_proto_src}"
  "${route_guide_proto_hdr}"
  "${route_guide_grpc_src}"
  "${route_guide_grpc_hdr}")
target_include_directories(route_guide_proto PUBLIC "${CMAKE_CURRENT_BINARY_DIR}")
target_link_libraries(route_guide_proto PUBLIC gRPC::grpc++ protobuf::libprotobuf)

add_library(route_guide_core
  helper.cc
  route_guide_client_lib.cc
  route_guide_service.cc)
target_include_directories(route_guide_core PUBLIC
  "${CMAKE_CURRENT_SOURCE_DIR}"
  "${CMAKE_CURRENT_BINARY_DIR}")
target_link_libraries(route_guide_core PUBLIC route_guide_proto)

add_executable(route_guide_client route_guide_client.cc)
target_link_libraries(route_guide_client PRIVATE
  route_guide_core
  absl::flags_parse
  absl::log_initialize)

add_executable(route_guide_server route_guide_server.cc)
target_link_libraries(route_guide_server PRIVATE
  route_guide_core
  absl::flags_parse
  absl::log_initialize)

option(RouteGuideBuildPactTests "Build RouteGuide Pact tests" OFF)
if(RouteGuideBuildPactTests)
  if(NOT PACT_FFI_ROOT)
    message(FATAL_ERROR "PACT_FFI_ROOT is required to build Pact tests")
  endif()
  if(NOT EXISTS "${PACT_FFI_ROOT}/include/pact.h")
    message(FATAL_ERROR
      "PACT_FFI_ROOT does not contain include/pact.h: ${PACT_FFI_ROOT}")
  endif()
  if(APPLE)
    file(GLOB route_guide_pact_ffi_runtime
      "${PACT_FFI_ROOT}/lib/libpact_ffi-macos-*.dylib")
  elseif(WIN32)
    file(GLOB route_guide_pact_ffi_runtime
      "${PACT_FFI_ROOT}/lib/pact_ffi-windows-*.dll.lib")
  else()
    file(GLOB route_guide_pact_ffi_runtime
      "${PACT_FFI_ROOT}/lib/libpact_ffi-linux-*.so")
  endif()
  if(NOT route_guide_pact_ffi_runtime)
    message(FATAL_ERROR
      "PACT_FFI_ROOT does not contain a Pact FFI shared library for ${CMAKE_SYSTEM_NAME}: ${PACT_FFI_ROOT}/lib")
  endif()

  set(PactBuildTests OFF CACHE BOOL "" FORCE)
  set(PactBuildUniversal OFF CACHE BOOL "" FORCE)
  add_subdirectory("${CMAKE_CURRENT_SOURCE_DIR}/../../consumer"
                   "${CMAKE_CURRENT_BINARY_DIR}/pact-consumer")
  add_subdirectory("${CMAKE_CURRENT_SOURCE_DIR}/../../verifier"
                   "${CMAKE_CURRENT_BINARY_DIR}/pact-verifier")

  set(route_guide_pact_dir "${CMAKE_CURRENT_BINARY_DIR}/pacts")
  set(route_guide_pact_file
      "${route_guide_pact_dir}/route-guide-consumer-route-guide-provider.json")

  add_executable(route_guide_consumer_test route_guide_consumer_test.cc)
  target_compile_definitions(route_guide_consumer_test PRIVATE
    ROUTE_GUIDE_PACT_DIR="${route_guide_pact_dir}"
    ROUTE_GUIDE_PROTO_FILE="${route_guide_proto}")
  target_include_directories(route_guide_consumer_test PRIVATE
    "${CMAKE_CURRENT_SOURCE_DIR}/../../consumer/include"
    "${PACT_FFI_ROOT}/include"
    "${Boost_ROOT}")
  target_link_libraries(route_guide_consumer_test PRIVATE
    route_guide_core pact-cpp-consumer)

  add_executable(route_guide_provider_test route_guide_provider_test.cc)
  target_compile_definitions(route_guide_provider_test PRIVATE
    ROUTE_GUIDE_DATABASE="${CMAKE_CURRENT_SOURCE_DIR}/route_guide_db.json"
    ROUTE_GUIDE_PACT_FILE="${route_guide_pact_file}")
  target_include_directories(route_guide_provider_test PRIVATE
    "${CMAKE_CURRENT_SOURCE_DIR}/../../verifier/include"
    "${PACT_FFI_ROOT}/include")
  target_link_libraries(route_guide_provider_test PRIVATE
    route_guide_core pact-cpp-verifier)

  enable_testing()
  add_test(NAME route_guide_consumer COMMAND route_guide_consumer_test)
  add_test(NAME route_guide_provider COMMAND route_guide_provider_test)
  set_tests_properties(route_guide_consumer PROPERTIES TIMEOUT 300)
  set_tests_properties(route_guide_provider PROPERTIES
    DEPENDS route_guide_consumer
    TIMEOUT 300)
  if(NOT WIN32)
    set_tests_properties(route_guide_consumer route_guide_provider PROPERTIES
      ENVIRONMENT
        "DYLD_LIBRARY_PATH=${PACT_FFI_ROOT}/lib:$ENV{DYLD_LIBRARY_PATH};LD_LIBRARY_PATH=${PACT_FFI_ROOT}/lib:$ENV{LD_LIBRARY_PATH}")
  endif()
endif()
