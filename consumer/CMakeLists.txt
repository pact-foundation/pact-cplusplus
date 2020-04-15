cmake_minimum_required(VERSION 3.16)

# set the project name
project(pact-cplusplus-consumer VERSION 0.0.0)

# Initialize Conan
INCLUDE(${CMAKE_BINARY_DIR}/conanbuildinfo.cmake)
CONAN_BASIC_SETUP()

# specify the C++ standard
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED True)

add_library (PactConsumer src/consumer.cpp)
target_include_directories (PactConsumer PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})
#TARGET_LINK_LIBRARIES(${CMAKE_PROJECT_NAME} ${CONAN_LIBS})

# Install library
#INSTALL(TARGETS ${CMAKE_PROJECT_NAME}
#        DESTINATION lib)
