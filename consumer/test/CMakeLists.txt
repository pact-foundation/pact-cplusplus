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
    target_link_libraries(pact-consumer-tests PRIVATE ws2_32 userenv crypt32 secur32 dnsapi ncrypt ntdll)
endif()

add_custom_command(TARGET pact-consumer-tests POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E copy_if_different
        "${CMAKE_CURRENT_SOURCE_DIR}/example.jpg"
        "$<TARGET_FILE_DIR:pact-consumer-tests>")

# Include test
add_test(NAME consumer
         WORKING_DIRECTORY $<TARGET_FILE_DIR:pact-consumer-tests>
         COMMAND ${PROJECT_NAME})
