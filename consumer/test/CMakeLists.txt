cmake_minimum_required(VERSION 3.16)

# set the project name
project(pact-consumer-tests)

# Initialize Conan
INCLUDE(${CMAKE_BINARY_DIR}/conanbuildinfo.cmake)
CONAN_BASIC_SETUP()

ADD_EXECUTABLE(${PROJECT_NAME} src/consumer_test.cpp src/todo.cpp src/matchers_tests.cpp src/multipart_parser.cpp)
target_include_directories(pact-consumer-tests PRIVATE ../include)

if(WIN32)
    TARGET_LINK_LIBRARIES(pact-consumer-tests ${CONAN_LIBS} pact-cpp-consumer ws2_32 userenv crypt32 secur32 dnsapi ncrypt ntdll)
elseif(UNIX) # LINUX or APPLE
    TARGET_LINK_LIBRARIES(pact-consumer-tests ${CONAN_LIBS} pact-cpp-consumer)
endif()

# Include test
ADD_TEST(NAME consumer
         WORKING_DIRECTORY ${CMAKE_BINARY_DIR}/bin
         COMMAND ${PROJECT_NAME})
