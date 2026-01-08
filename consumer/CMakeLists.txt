cmake_minimum_required(VERSION 3.16)

# set the project name
project(pact-cpp-consumer VERSION 0.1.0)

# specify the C++ standard
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED True)

# Conan integration (optional)
#    cmake -S consumer -B build -DPactUseConan=ON/OFF
# Default ON: use conan build info when available
option(PactUseConan "Use Conan build integration if available" ON)
if(PactUseConan)
	if(EXISTS ${CMAKE_BINARY_DIR}/conanbuildinfo.cmake)
		include(${CMAKE_BINARY_DIR}/conanbuildinfo.cmake)
		conan_basic_setup()
	else()
		message(STATUS "Conan disabled or build info not found; continuing without Conan")
	endif()
endif()

# Detect macOS CPU architecture
if(NOT DEFINED PACT_FFI_ARCH AND DEFINED ENV{PACT_FFI_ARCH})
  set(PACT_FFI_ARCH "$ENV{PACT_FFI_ARCH}")
endif()

if(NOT DEFINED PACT_FFI_ARCH)
  if(APPLE)
    # Prefer CMAKE_OSX_ARCHITECTURES, then uname -m
    if(CMAKE_OSX_ARCHITECTURES MATCHES "arm64")
      set(PACT_FFI_ARCH aarch64)
    elseif(CMAKE_OSX_ARCHITECTURES MATCHES "x86_64")
      set(PACT_FFI_ARCH x86_64)
    else()
      execute_process(COMMAND uname -m OUTPUT_VARIABLE _uname_m OUTPUT_STRIP_TRAILING_WHITESPACE)
      if(_uname_m MATCHES "arm64|aarch64")
        set(PACT_FFI_ARCH aarch64)
      else()
        set(PACT_FFI_ARCH x86_64)
      endif()
    endif()
  else()
    # Non-mac: use CMAKE_SYSTEM_PROCESSOR, fallback to host, then pointer size
    if(CMAKE_SYSTEM_PROCESSOR MATCHES "arm64|aarch64")
      set(PACT_FFI_ARCH aarch64)
    elseif(CMAKE_SYSTEM_PROCESSOR MATCHES "x86_64|amd64")
      set(PACT_FFI_ARCH x86_64)
    elseif(CMAKE_HOST_SYSTEM_PROCESSOR MATCHES "arm64|aarch64")
      set(PACT_FFI_ARCH aarch64)
    elseif(CMAKE_HOST_SYSTEM_PROCESSOR MATCHES "x86_64|amd64")
      set(PACT_FFI_ARCH x86_64)
    elseif(CMAKE_SIZEOF_VOID_P EQUAL 8)
      set(PACT_FFI_ARCH x86_64)
    else()
      set(PACT_FFI_ARCH aarch64)
    endif()
  endif()
endif()

message(STATUS "PactFFI: resolved arch '${PACT_FFI_ARCH}'")

######### Link to pact_ffi #########
set(PACT_FFI_VERSION "0.5.0")
set(PACT_FFI_ROOT "${PACT_FFI_ROOT}" CACHE PATH "Root directory of pact_ffi installation")
if(NOT PACT_FFI_ROOT OR PACT_FFI_ROOT STREQUAL "")
    message(FATAL_ERROR "PACT_FFI_ROOT not set; please set to the root of your pact_ffi installation")
endif()

set(PACT_FFI_INCLUDE "${PACT_FFI_ROOT}/include")
set(PACT_FFI_LIB_DIR "${PACT_FFI_ROOT}/lib")

if(APPLE)
  set(PACT_FFI_OS_ARCH "macos-${PACT_FFI_ARCH}")
  set(_PACT_FFI_FILENAME "libpact_ffi-${PACT_FFI_OS_ARCH}.a")
  set(_PACT_FFI_SHARED "libpact_ffi-${PACT_FFI_OS_ARCH}.dylib")
elseif(WIN32)
  set(PACT_FFI_OS_ARCH "windows-${PACT_FFI_ARCH}")
  set(_PACT_FFI_FILENAME "pact_ffi-${PACT_FFI_OS_ARCH}.lib")
  set(_PACT_FFI_DLL_FILENAME "pact_ffi-${PACT_FFI_OS_ARCH}.dll")
else()
  set(PACT_FFI_OS_ARCH "linux-${PACT_FFI_ARCH}")
  set(_PACT_FFI_FILENAME "libpact_ffi-${PACT_FFI_OS_ARCH}.a")
  set(_PACT_FFI_SHARED "libpact_ffi-${PACT_FFI_OS_ARCH}.so")
endif()

set(PACT_FFI_LIB "${PACT_FFI_LIB_DIR}/${_PACT_FFI_FILENAME}")
########################################

# Require Boost_ROOT and add headers
set(Boost_ROOT "${Boost_ROOT}" CACHE PATH "Root directory of Boost")
if(NOT Boost_ROOT OR Boost_ROOT STREQUAL "")
  message(FATAL_ERROR "Boost_ROOT is required. Example: -DBoost_ROOT=\"D:/dev/fusion/3P/BOOST/1.1850.1/win64_0002\"")
endif()
#if(NOT EXISTS "${Boost_ROOT}/include")
#  message(FATAL_ERROR "Boost include directory not found at '${Boost_ROOT}/include'")
#endif()
include_directories("${Boost_ROOT}")

# Fetch nlohmann/json
include(FetchContent)
FetchContent_Declare(
  nlohmann_json
  GIT_REPOSITORY https://github.com/nlohmann/json.git
  GIT_TAG v3.12.0
)
FetchContent_MakeAvailable(nlohmann_json)

add_subdirectory(src)

# Link deps privately (do not propagate to client apps)
if(TARGET pact-cpp-consumer)
  target_include_directories(pact-cpp-consumer PRIVATE "${PACT_FFI_INCLUDE}")
  target_link_libraries(pact-cpp-consumer PRIVATE "${PACT_FFI_LIB}" nlohmann_json::nlohmann_json)
endif()

# Install your lib + headers (versioned)
install(TARGETS pact-cpp-consumer
  ARCHIVE DESTINATION pact-cpp-consumer/${PROJECT_VERSION}/lib
)
install(FILES include/consumer.h include/matchers.h
  DESTINATION pact-cpp-consumer/${PROJECT_VERSION}/include
)

# Install pact_ffi headers
if(EXISTS "${PACT_FFI_INCLUDE}")
  install(DIRECTORY "${PACT_FFI_INCLUDE}/"
          DESTINATION pact_ffi/${PACT_FFI_VERSION}/include
          FILES_MATCHING PATTERN "*.h")
endif()

# Library + (Windows) DLL renamed
if(WIN32)
  if(EXISTS "${PACT_FFI_LIB}")
    install(FILES "${PACT_FFI_LIB}"
            DESTINATION pact_ffi/${PACT_FFI_VERSION}/lib
            RENAME pact_ffi.lib)
  endif()
  if(DEFINED _PACT_FFI_DLL_FILENAME)
    set(PACT_FFI_DLL "${PACT_FFI_LIB_DIR}/${_PACT_FFI_DLL_FILENAME}")
    if(EXISTS "${PACT_FFI_DLL}")
      install(FILES "${PACT_FFI_DLL}"
              DESTINATION pact_ffi/${PACT_FFI_VERSION}/bin
              RENAME pact_ffi.dll)
    endif()
  endif()
elseif(APPLE OR UNIX)
  if(EXISTS "${PACT_FFI_LIB}")
    # Static archive renamed to conventional libpact_ffi.a
    install(FILES "${PACT_FFI_LIB}"
            DESTINATION pact_ffi/${PACT_FFI_VERSION}/lib
            RENAME libpact_ffi.a)
  endif()
  if(DEFINED _PACT_FFI_SHARED)
    set(PACT_FFI_SHARED "${PACT_FFI_LIB_DIR}/${_PACT_FFI_SHARED}")
    # Shared lib renamed to conventional name
    if(APPLE)
      install(FILES "${PACT_FFI_SHARED}"
              DESTINATION pact_ffi/${PACT_FFI_VERSION}/lib
              RENAME libpact_ffi.dylib)
    else()
      install(FILES "${PACT_FFI_SHARED}"
              DESTINATION pact_ffi/${PACT_FFI_VERSION}/lib
              RENAME libpact_ffi.so)
    endif()
  endif()
endif()

# Add test
#    cmake -S consumer -B build -DPactBuildTests=ON/OFF
# Default OFF: do not build tests unless explicitly requested
option(PactBuildTests "Build consumer tests" OFF)
if(PactBuildTests)
    enable_testing()
    add_subdirectory(test)
endif()