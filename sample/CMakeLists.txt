cmake_minimum_required(VERSION 3.16)
project(pact_cpp_consumer_sample LANGUAGES CXX)

if(POLICY CMP0167)
    cmake_policy(SET CMP0167 NEW)
endif()

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

set(PACT_CPP_ROOT "${CMAKE_CURRENT_LIST_DIR}/../build/install/pact-cpp" CACHE PATH
    "Path to the Pact C++ release installation")
set(PACT_CPP_CONSUMER_VERSION "0.1.0" CACHE STRING "Pact C++ Consumer release version")
set(PACT_FFI_VERSION "v0.5.0" CACHE STRING "Pact FFI release version")

set(PACT_CPP_CONSUMER_ROOT "${PACT_CPP_ROOT}/pact-cpp-consumer/${PACT_CPP_CONSUMER_VERSION}")
set(PACT_FFI_ROOT "${PACT_CPP_ROOT}/pact_ffi/${PACT_FFI_VERSION}")

find_package(Boost REQUIRED)

find_path(PACT_CPP_CONSUMER_INCLUDE_DIR consumer.h
    PATHS "${PACT_CPP_CONSUMER_ROOT}/include" REQUIRED NO_DEFAULT_PATH)
find_library(PACT_CPP_CONSUMER_LIBRARY NAMES pact-cpp-consumer
    PATHS "${PACT_CPP_CONSUMER_ROOT}/lib" REQUIRED NO_DEFAULT_PATH)
find_path(PACT_FFI_INCLUDE_DIR pact.h
    PATHS "${PACT_FFI_ROOT}/include" REQUIRED NO_DEFAULT_PATH)

if(WIN32)
    set(PACT_FFI_LIBRARY "${PACT_FFI_ROOT}/lib/pact_ffi.lib")
    set(PACT_FFI_RUNTIME_LIBRARY "${PACT_FFI_ROOT}/lib/pact_ffi.dll")
elseif(APPLE)
    set(PACT_FFI_LIBRARY "${PACT_FFI_ROOT}/lib/libpact_ffi.dylib")
else()
    find_library(PACT_FFI_LIBRARY NAMES pact_ffi
        PATHS "${PACT_FFI_ROOT}/lib" REQUIRED NO_DEFAULT_PATH)
endif()

add_library(PactCppConsumer::PactCppConsumer STATIC IMPORTED GLOBAL)
set_target_properties(PactCppConsumer::PactCppConsumer PROPERTIES
    IMPORTED_LOCATION "${PACT_CPP_CONSUMER_LIBRARY}"
    INTERFACE_INCLUDE_DIRECTORIES "${PACT_CPP_CONSUMER_INCLUDE_DIR};${PACT_FFI_INCLUDE_DIR}")

add_library(PactFfi::PactFfi SHARED IMPORTED GLOBAL)
if(WIN32)
    set_target_properties(PactFfi::PactFfi PROPERTIES
        IMPORTED_IMPLIB "${PACT_FFI_LIBRARY}"
        IMPORTED_LOCATION "${PACT_FFI_RUNTIME_LIBRARY}"
        INTERFACE_INCLUDE_DIRECTORIES "${PACT_FFI_INCLUDE_DIR}")
else()
    set_target_properties(PactFfi::PactFfi PROPERTIES
        IMPORTED_LOCATION "${PACT_FFI_LIBRARY}"
        INTERFACE_INCLUDE_DIRECTORIES "${PACT_FFI_INCLUDE_DIR}")
endif()

add_executable(pact-consumer-sample main.cpp)
target_link_libraries(pact-consumer-sample PRIVATE
    PactCppConsumer::PactCppConsumer
    PactFfi::PactFfi
    Boost::headers)

if(APPLE)
    add_custom_command(TARGET pact-consumer-sample POST_BUILD
        COMMAND ${CMAKE_COMMAND} -E copy_if_different
            "${PACT_FFI_LIBRARY}"
            "$<TARGET_FILE_DIR:pact-consumer-sample>"
        COMMAND install_name_tool -change libpact_ffi.dylib @loader_path/libpact_ffi.dylib
            "$<TARGET_FILE:pact-consumer-sample>")
elseif(UNIX)
    set_property(TARGET pact-consumer-sample APPEND PROPERTY BUILD_RPATH "${PACT_FFI_ROOT}/lib")
elseif(WIN32)
    add_custom_command(TARGET pact-consumer-sample POST_BUILD
        COMMAND ${CMAKE_COMMAND} -E copy_if_different
            "${PACT_FFI_RUNTIME_LIBRARY}"
            "$<TARGET_FILE_DIR:pact-consumer-sample>")
endif()