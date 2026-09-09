cmake_minimum_required(VERSION 3.16)

# set the project name
project(pact-verifier-tests)

find_package(GTest REQUIRED)
find_package(CURL REQUIRED)

add_executable(${PROJECT_NAME} src/verifier_test.cpp src/state_server_test.cpp)
target_include_directories(pact-verifier-tests PRIVATE ../include "${PACT_FFI_INCLUDE}")

target_link_libraries(pact-verifier-tests PRIVATE
    pact-cpp-verifier
    GTest::gtest
    GTest::gtest_main
    CURL::libcurl
    nlohmann_json::nlohmann_json)

if(WIN32)
    target_link_libraries(pact-verifier-tests PRIVATE ws2_32 userenv crypt32 secur32 dnsapi ncrypt ntdll pdh netapi32 iphlpapi powrprof psapi)
endif()

add_custom_command(TARGET pact-verifier-tests POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E copy_directory
        "${CMAKE_CURRENT_SOURCE_DIR}/pacts"
        "$<TARGET_FILE_DIR:pact-verifier-tests>/pacts")

# pact-cpp-verifier links against the DLL's import lib, which records the DLL's
# original export name "pact_ffi.dll", so the copy must be renamed to match -
# copying it as pact_ffi-<platform>.dll causes the loader to fail with
# STATUS_DLL_NOT_FOUND at runtime.
if(WIN32 AND DEFINED PACT_FFI_DLL AND EXISTS "${PACT_FFI_DLL}")
    add_custom_command(TARGET pact-verifier-tests POST_BUILD
        COMMAND ${CMAKE_COMMAND} -E copy_if_different
            "${PACT_FFI_DLL}"
            "$<TARGET_FILE_DIR:pact-verifier-tests>/pact_ffi.dll")
endif()

# Include test
add_test(NAME verifier
         WORKING_DIRECTORY $<TARGET_FILE_DIR:pact-verifier-tests>
         COMMAND ${PROJECT_NAME})

if(NOT WIN32)
    set_tests_properties(verifier PROPERTIES
        ENVIRONMENT "DYLD_LIBRARY_PATH=${PACT_FFI_LIB_DIR}:$ENV{DYLD_LIBRARY_PATH};LD_LIBRARY_PATH=${PACT_FFI_LIB_DIR}:$ENV{LD_LIBRARY_PATH}")
endif()

# Without an explicit timeout ctest computes an effectively infinite one, so a hung
# test (e.g. a stalled state change server on Windows CI) runs until the whole CI
# job times out.
set_tests_properties(verifier PROPERTIES TIMEOUT 300)
