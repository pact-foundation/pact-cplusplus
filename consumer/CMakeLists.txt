cmake_minimum_required(VERSION 3.16)

# set the project name
project(pact-cpp-consumer VERSION 0.1.1)

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

add_subdirectory(src)

######### Link to pact_ffi #########
set(PACT_FFI_ROOT "/Users/avancir/dev/pact_ffi-v0.4.28") # Adjust this path as needed
set(PACT_FFI_INCLUDE "${PACT_FFI_ROOT}/include")
set(PACT_FFI_LIB_DIR "${PACT_FFI_ROOT}/lib")

set(PACT_FFI_OS_ARCH "macos-${PACT_FFI_ARCH}")
if(APPLE)
  set(_PACT_FFI_FILENAME "libpact_ffi-${PACT_FFI_OS_ARCH}.a")
  set(_PACT_FFI_KIND STATIC)
elseif(WIN32)
  set(PACT_FFI_OS_ARCH "windows-${PACT_FFI_ARCH}")
  set(_PACT_FFI_FILENAME "libpact_ffi-${PACT_FFI_OS_ARCH}.dll")
  set(_PACT_FFI_KIND SHARED)
else()
  set(PACT_FFI_OS_ARCH "linux-${PACT_FFI_ARCH}")
  set(_PACT_FFI_FILENAME "libpact_ffi-${PACT_FFI_OS_ARCH}.a")
  set(_PACT_FFI_KIND STATIC)
endif()

set(PACT_FFI_LIB "${PACT_FFI_LIB_DIR}/${_PACT_FFI_FILENAME}")

if(TARGET pact-cpp-consumer)
  # Name library by OS+arch: libpact-cpp-consumer-<os-arch>.a/.dylib
  set_target_properties(pact-cpp-consumer PROPERTIES OUTPUT_NAME "pact-cpp-consumer-${PACT_FFI_OS_ARCH}")
    if(EXISTS "${PACT_FFI_LIB}")
        message(STATUS "Using static pact_ffi: ${PACT_FFI_LIB}")
        target_include_directories(pact-cpp-consumer PRIVATE "${PACT_FFI_INCLUDE}")
        target_link_libraries(pact-cpp-consumer PRIVATE "${PACT_FFI_LIB}")
    else()
        message(WARNING "Static pact_ffi archive not found: ${PACT_FFI_LIB}")
    endif()
endif()
########################################

######### Find and link Boost #########
# Allow overriding Boost root from command line:
#   cmake -S consumer -B build -DBoost_ROOT=/path/to/boost
# Only set by user
set(Boost_ROOT "${Boost_ROOT}" CACHE PATH "Root directory of Boost installation")

# Derive library dir if not provided
set(Boost_LIBRARY_DIR "${Boost_LIBRARY_DIR}" CACHE PATH "Boost library directory")
if(NOT Boost_LIBRARY_DIR OR Boost_LIBRARY_DIR STREQUAL "")
    set(Boost_LIBRARY_DIR "${Boost_ROOT}/lib/release/lib")
endif()

set(Boost_NO_SYSTEM_PATHS ON)          # force use of given root
set(Boost_USE_STATIC_LIBS ON)
set(Boost_USE_MULTITHREADED ON)
set(Boost_USE_RELEASE_LIBS ON)

find_package(Boost 1.85.0 REQUIRED COMPONENTS filesystem system)

if(TARGET pact-cpp-consumer)
    target_include_directories(pact-cpp-consumer PRIVATE ${Boost_INCLUDE_DIR})
    target_link_libraries(pact-cpp-consumer PRIVATE Boost::filesystem Boost::system)
endif()
#########################################

include(FetchContent)
FetchContent_Declare(
  nlohmann_json
  GIT_REPOSITORY https://github.com/nlohmann/json.git
  GIT_TAG v3.12.0
)
FetchContent_MakeAvailable(nlohmann_json)
target_link_libraries(pact-cpp-consumer PRIVATE nlohmann_json::nlohmann_json)

# Install library and headers
install(TARGETS pact-cpp-consumer DESTINATION lib)
install(FILES include/consumer.h include/matchers.h DESTINATION include)

# Install pact_ffi artifacts alongside pact-cpp-consumer
if(EXISTS "${PACT_FFI_LIB}")
  install(FILES "${PACT_FFI_LIB}" DESTINATION lib)
endif()
if(EXISTS "${PACT_FFI_INCLUDE}")
  install(DIRECTORY "${PACT_FFI_INCLUDE}/" DESTINATION include FILES_MATCHING PATTERN "*.h")
endif()

# Add test
#    cmake -S consumer -B build -DPactBuildTests=ON/OFF
# Default OFF: do not build tests unless explicitly requested
option(PactBuildTests "Build consumer tests" OFF)
if(PactBuildTests)
    enable_testing()
    add_subdirectory(test)
endif()