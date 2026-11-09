# Minimal downstream project that pulls pact-cpp in with FetchContent.
# CI builds this against the checked-out tree; real projects pin a release tag.
cmake_minimum_required(VERSION 3.16)
project(pact_cpp_example CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

include(FetchContent)

FetchContent_Declare(pact_cpp_consumer
  GIT_REPOSITORY https://github.com/pact-foundation/pact-cplusplus.git
  GIT_TAG        pact-cpp-consumer-v0.2.0
  SOURCE_SUBDIR  consumer
)
FetchContent_Declare(pact_cpp_verifier
  GIT_REPOSITORY https://github.com/pact-foundation/pact-cplusplus.git
  GIT_TAG        pact-cpp-verifier-v0.1.0
  SOURCE_SUBDIR  verifier
)
FetchContent_MakeAvailable(pact_cpp_consumer pact_cpp_verifier)

add_executable(pact_cpp_example main.cpp)
target_link_libraries(pact_cpp_example PRIVATE pact-cpp-consumer pact-cpp-verifier)
