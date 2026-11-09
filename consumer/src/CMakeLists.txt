cmake_minimum_required(VERSION 3.16)

# set the project name
project(pact-cpp-consumer VERSION 0.2.0)

# specify the C++ standard
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED True)

# Initialize Conan
# Conan 2 injects dependencies via CMakeDeps, so the targets are already available.

add_library(pact-cpp-consumer STATIC consumer.cpp matchers.cpp)

target_include_directories(pact-cpp-consumer
  PUBLIC
    $<INSTALL_INTERFACE:include>
    $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/../include>
    # The public headers include <pact.h>, so callers need the pact_ffi headers too
    $<BUILD_INTERFACE:${PACT_FFI_INCLUDE}>
)

# Link deps privately so they aren’t required by client apps
target_link_libraries(pact-cpp-consumer PRIVATE "${PACT_FFI_LINK_LIB}" nlohmann_json::nlohmann_json)

# matchers.h is an installed header that includes Boost, so it is a public dep
if(TARGET Boost::headers)
    target_link_libraries(pact-cpp-consumer PUBLIC Boost::headers)
elseif(Boost_ROOT)
    target_include_directories(pact-cpp-consumer PUBLIC $<BUILD_INTERFACE:${Boost_ROOT}>)
endif()