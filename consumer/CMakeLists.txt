cmake_minimum_required(VERSION 3.16)

# set the project name
project(pact-consumer VERSION 0.0.0 LANGUAGES CXX)

# Initialize Conan
INCLUDE(${CMAKE_BINARY_DIR}/conanbuildinfo.cmake)
CONAN_BASIC_SETUP()

# specify the C++ standard
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED True)

add_library(pact-consumer src/consumer.cpp)
target_include_directories(pact-consumer
    PUBLIC 
        $<INSTALL_INTERFACE:include>    
        $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>
    PRIVATE
        ${CMAKE_CURRENT_SOURCE_DIR}/src
)
TARGET_LINK_LIBRARIES(pact-consumer ${CONAN_LIBS})

# Install library
install(TARGETS pact-consumer DESTINATION lib)
install(FILES src/consumer.h DESTINATION include)

## Add test
enable_testing()
add_subdirectory(test)
