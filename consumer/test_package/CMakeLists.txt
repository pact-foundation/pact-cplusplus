cmake_minimum_required(VERSION 3.16)
project(PackageTest CXX)

# specify the C++ standard
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED True)

include(${CMAKE_BINARY_DIR}/conanbuildinfo.cmake)
conan_basic_setup()

add_executable(example example.cpp)
if(WIN32)
    TARGET_LINK_LIBRARIES(example ${CONAN_LIBS} ws2_32 userenv crypt32 secur32 dnsapi ncrypt ntdll)
elseif(UNIX) # LINUX or APPLE
    TARGET_LINK_LIBRARIES(example ${CONAN_LIBS})
endif()

# CTest is a testing tool that can be used to test your project.
# enable_testing()
# add_test(NAME example
#          WORKING_DIRECTORY ${CMAKE_BINARY_DIR}/bin
#          COMMAND example)
