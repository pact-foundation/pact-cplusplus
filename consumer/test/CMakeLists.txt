cmake_minimum_required(VERSION 3.16)

# set the project name
project(pact-consumer-tests CXX)

# Initialize Conan
INCLUDE(${CMAKE_BINARY_DIR}/conanbuildinfo.cmake)
CONAN_BASIC_SETUP()

# specify the C++ standard
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED True)

ADD_EXECUTABLE(${PROJECT_NAME} src/consumer_test.cpp src/todo.cpp)
TARGET_LINK_LIBRARIES(pact-consumer-tests ${CONAN_LIBS} pact-consumer)
target_include_directories(pact-consumer-tests PRIVATE ../include)

# Include test
ADD_TEST(NAME consumer
         WORKING_DIRECTORY ${CMAKE_BINARY_DIR}/bin
         COMMAND ${PROJECT_NAME})
