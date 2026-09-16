cmake_minimum_required(VERSION 3.16)
project(PackageTest CXX)

# specify the C++ standard
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED True)

find_package(pact_cpp_verifier CONFIG REQUIRED)

add_executable(example example.cpp)
target_link_libraries(example PRIVATE pact_cpp_verifier::pact_cpp_verifier)

# CTest is a testing tool that can be used to test your project.
# enable_testing()
# add_test(NAME example
#          WORKING_DIRECTORY ${CMAKE_BINARY_DIR}/bin
#          COMMAND example)
