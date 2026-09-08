cmake_minimum_required(VERSION 3.16)

# set the project name
project(pact-consumer-tests)

find_package(GTest REQUIRED)
find_package(CURL REQUIRED)

add_executable(${PROJECT_NAME} src/consumer_test.cpp src/todo.cpp src/matchers_tests.cpp src/multipart_parser.cpp)
target_include_directories(pact-consumer-tests PRIVATE ../include "${PACT_FFI_INCLUDE}")

target_link_libraries(pact-consumer-tests PRIVATE
    pact-cpp-consumer
    GTest::gtest
    GTest::gtest_main
    CURL::libcurl
    nlohmann_json::nlohmann_json)

if(WIN32)
    target_link_libraries(pact-consumer-tests PRIVATE ws2_32 userenv crypt32 secur32 dnsapi ncrypt ntdll pdh netapi32 iphlpapi powrprof psapi)
endif()

add_custom_command(TARGET pact-consumer-tests POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E copy_if_different
        "${CMAKE_CURRENT_SOURCE_DIR}/example.jpg"
        "$<TARGET_FILE_DIR:pact-consumer-tests>")

# pact-cpp-consumer links against the DLL's import lib, so the actual pact_ffi.dll
# needs to sit next to the test binary for the loader to find it at runtime.
if(WIN32 AND DEFINED PACT_FFI_DLL AND EXISTS "${PACT_FFI_DLL}")
    add_custom_command(TARGET pact-consumer-tests POST_BUILD
        COMMAND ${CMAKE_COMMAND} -E copy_if_different
            "${PACT_FFI_DLL}"
            "$<TARGET_FILE_DIR:pact-consumer-tests>")
endif()

# Include test
add_test(NAME consumer
         WORKING_DIRECTORY $<TARGET_FILE_DIR:pact-consumer-tests>
         COMMAND ${PROJECT_NAME})

if(NOT WIN32)
    set_tests_properties(consumer PROPERTIES
        ENVIRONMENT "DYLD_LIBRARY_PATH=${PACT_FFI_LIB_DIR}:$ENV{DYLD_LIBRARY_PATH};LD_LIBRARY_PATH=${PACT_FFI_LIB_DIR}:$ENV{LD_LIBRARY_PATH}")
endif()

# Without an explicit timeout ctest computes an effectively infinite one, so a hung
# test (e.g. a stalled mock server on Windows CI) runs until the whole CI job times out.
set_tests_properties(consumer PROPERTIES TIMEOUT 300)
