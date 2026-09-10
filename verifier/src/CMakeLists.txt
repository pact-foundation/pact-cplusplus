cmake_minimum_required(VERSION 3.16)

# set the project name
project(pact-cpp-verifier VERSION 0.1.0)

# specify the C++ standard
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED True)

# Initialize Conan
# Conan 2 injects dependencies via CMakeDeps, so the targets are already available.

add_library(pact-cpp-verifier STATIC verifier.cpp http_server.cpp)

target_compile_definitions(pact-cpp-verifier PRIVATE PACT_CPP_VERIFIER_VERSION="${PROJECT_VERSION}")

target_include_directories(pact-cpp-verifier
  PUBLIC
    $<INSTALL_INTERFACE:include>
    $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/../include>
  PRIVATE
    ${PACT_FFI_INCLUDE}
)

# Link deps privately so they aren't required by client apps
target_link_libraries(pact-cpp-verifier PRIVATE "${PACT_FFI_LINK_LIB}" nlohmann_json::nlohmann_json)

if(UNIX)
    find_package(Threads REQUIRED)
    target_link_libraries(pact-cpp-verifier PUBLIC Threads::Threads)
endif()

# The bundled HTTP server used to host the provider state endpoint uses Winsock
if(WIN32)
    target_link_libraries(pact-cpp-verifier PUBLIC ws2_32)
endif()
