cmake_minimum_required(VERSION 3.16)

# set the project name
project(pact-cpp-consumer VERSION 0.0.0)

# specify the C++ standard
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED True)

# Initialize Conan
INCLUDE(${CMAKE_BINARY_DIR}/conanbuildinfo.cmake)
CONAN_BASIC_SETUP()

add_library(pact-cpp-consumer consumer.cpp matchers.cpp)
target_include_directories(pact-cpp-consumer
    PUBLIC 
        $<INSTALL_INTERFACE:include>    
        $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/../include>
    PRIVATE
        ${CMAKE_CURRENT_SOURCE_DIR}
)
TARGET_LINK_LIBRARIES(pact-cpp-consumer ${CONAN_LIBS})
