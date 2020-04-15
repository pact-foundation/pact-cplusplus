cmake_minimum_required(VERSION 3.16)

# set the project name
project(pact-cplusplus-consumer-tests CXX)

# Initialize Conan
INCLUDE(${CMAKE_BINARY_DIR}/conanbuildinfo.cmake)
CONAN_BASIC_SETUP()

# specify the C++ standard
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED True)

ADD_EXECUTABLE(${PROJECT_NAME} consumer_test.cpp)
TARGET_LINK_LIBRARIES(${CMAKE_PROJECT_NAME} ${CONAN_LIBS})

# Include test
ENABLE_TESTING()
ADD_TEST(NAME consumer
         WORKING_DIRECTORY ${CMAKE_BINARY_DIR}/bin
         COMMAND ${PROJECT_NAME})
