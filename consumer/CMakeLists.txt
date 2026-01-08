cmake_minimum_required(VERSION 3.16)

project(pact-cpp-consumer VERSION 0.1.0)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Conan integration (optional, default OFF)
#    cmake -S consumer -B build -DPactUseConan=ON/OFF
option(PactUseConan "Use Conan build integration if available" OFF)
if(PactUseConan)
  if(EXISTS "${CMAKE_BINARY_DIR}/conanbuildinfo.cmake")
    include("${CMAKE_BINARY_DIR}/conanbuildinfo.cmake")
		conan_basic_setup()
	else()
    message(STATUS "Conan ON but build info not found; continuing without Conan")
	endif()
endif()

# Detect target arch for pact_ffi
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
set(PACT_FFI_VERSION "${PACT_FFI_VERSION}" CACHE STRING "Version of pact_ffi to use")
set(PACT_FFI_ROOT "${PACT_FFI_ROOT}" CACHE PATH "Root directory of pact_ffi installation")
if(NOT PACT_FFI_ROOT OR PACT_FFI_ROOT STREQUAL "")
    message(FATAL_ERROR "PACT_FFI_ROOT not set; please set to the root of your pact_ffi installation")
endif()

set(PACT_FFI_INCLUDE "${PACT_FFI_ROOT}/include")
set(PACT_FFI_LIB_DIR "${PACT_FFI_ROOT}/lib")

if(APPLE)
  set(PACT_FFI_OS_ARCH "macos-${PACT_FFI_ARCH}")
  set(_PACT_FFI_FILENAME "libpact_ffi-${PACT_FFI_OS_ARCH}.a")
  set(_PACT_FFI_SHARED_FILENAME "libpact_ffi-${PACT_FFI_OS_ARCH}.dylib")
elseif(WIN32)
  set(PACT_FFI_OS_ARCH "windows-${PACT_FFI_ARCH}")
  set(_PACT_FFI_FILENAME "pact_ffi-${PACT_FFI_OS_ARCH}.lib")
  set(_PACT_FFI_DLL_FILENAME "pact_ffi-${PACT_FFI_OS_ARCH}.dll")
else()
  set(PACT_FFI_OS_ARCH "linux-${PACT_FFI_ARCH}")
  set(_PACT_FFI_FILENAME "libpact_ffi-${PACT_FFI_OS_ARCH}.a")
  set(_PACT_FFI_SHARED_FILENAME "libpact_ffi-${PACT_FFI_OS_ARCH}.so")
endif()

set(PACT_FFI_LIB "${PACT_FFI_LIB_DIR}/${_PACT_FFI_FILENAME}")
if(DEFINED _PACT_FFI_SHARED_FILENAME)
  set(PACT_FFI_SHARED "${PACT_FFI_LIB_DIR}/${_PACT_FFI_SHARED_FILENAME}")
endif()

# Optional macOS universal build via lipo for pact_ffi
# Provide -DPactBuildUniversal=ON; expects both arm64 and x86_64 libs in the same PACT_FFI_ROOT/lib
option(PactBuildUniversal "Build macOS universal binaries (arm64 + x86_64)" ON)
if(APPLE AND PactBuildUniversal)
  set(CMAKE_OSX_ARCHITECTURES "arm64;x86_64" CACHE STRING "macOS architectures" FORCE)

  # Expect these files under ${PACT_FFI_ROOT}/lib
  set(_ffi_lib_arm64 "${PACT_FFI_LIB_DIR}/libpact_ffi-macos-aarch64.a")
  set(_ffi_lib_x86   "${PACT_FFI_LIB_DIR}/libpact_ffi-macos-x86_64.a")
  set(_ffi_dyl_arm64 "${PACT_FFI_LIB_DIR}/libpact_ffi-macos-aarch64.dylib")
  set(_ffi_dyl_x86   "${PACT_FFI_LIB_DIR}/libpact_ffi-macos-x86_64.dylib")

  set(_ffi_univ_lib "${CMAKE_BINARY_DIR}/libpact_ffi-macos-universal.a")
  set(_ffi_univ_dyl "${CMAKE_BINARY_DIR}/libpact_ffi-macos-universal.dylib")

  add_custom_command(
    OUTPUT "${_ffi_univ_lib}"
    BYPRODUCTS "${_ffi_univ_lib}"
    COMMAND /usr/bin/lipo -create "${_ffi_lib_arm64}" "${_ffi_lib_x86}" -output "${_ffi_univ_lib}"
    DEPENDS "${_ffi_lib_arm64}" "${_ffi_lib_x86}"
    COMMENT "Creating universal static pact_ffi via lipo"
    VERBATIM
  )
  add_custom_command(
    OUTPUT "${_ffi_univ_dyl}"
    BYPRODUCTS "${_ffi_univ_dyl}"
    COMMAND /usr/bin/lipo -create "${_ffi_dyl_arm64}" "${_ffi_dyl_x86}" -output "${_ffi_univ_dyl}"
    DEPENDS "${_ffi_dyl_arm64}" "${_ffi_dyl_x86}"
    COMMENT "Creating universal shared pact_ffi via lipo"
    VERBATIM
  )
  
  # Build universal outputs as part of the default 'all' target
  add_custom_target(pact_ffi_universal ALL
    DEPENDS "${_ffi_univ_lib}" "${_ffi_univ_dyl}"
  )

  # Link with universal static pact_ffi and install universal shared
  set(PACT_FFI_LIB "${_ffi_univ_lib}")
  set(PACT_FFI_SHARED_UNIV "${_ffi_univ_dyl}")
endif()

# Boost config
set(Boost_ROOT "${Boost_ROOT}" CACHE PATH "Root directory of Boost")
if(NOT Boost_ROOT OR Boost_ROOT STREQUAL "")
  message(FATAL_ERROR "Boost_ROOT is required. Example: -DBoost_ROOT=\"D:/dev/fusion/3P/BOOST/1.1850.1/win64_0002\"")
endif()
include_directories("${Boost_ROOT}")

# Fetch nlohmann/json
include(FetchContent)
FetchContent_Declare(
  nlohmann_json
  GIT_REPOSITORY https://github.com/nlohmann/json.git
  GIT_TAG v3.12.0
)
FetchContent_MakeAvailable(nlohmann_json)

# Sources
add_subdirectory(src)

if(TARGET pact-cpp-consumer)
  set_target_properties(pact-cpp-consumer PROPERTIES
    OUTPUT_NAME "pact-cpp-consumer"
  )
  target_include_directories(pact-cpp-consumer PRIVATE "${PACT_FFI_INCLUDE}")
  target_link_libraries(pact-cpp-consumer PRIVATE "${PACT_FFI_LIB}" nlohmann_json::nlohmann_json)
endif()

# Install project lib + headers
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

# Install pact_ffi libraries
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
else()
  if(APPLE AND PactBuildUniversal)
    # Universal install: no existence guards, because pact-cpp-consumer depends on pact_ffi_universal
    install(FILES "${_ffi_univ_lib}"
            DESTINATION pact_ffi/${PACT_FFI_VERSION}/lib
            RENAME libpact_ffi.a)
    install(FILES "${_ffi_univ_dyl}"
            DESTINATION pact_ffi/${PACT_FFI_VERSION}/lib
            RENAME libpact_ffi.dylib)
  else()
    # Per-arch fallback
  if(EXISTS "${PACT_FFI_LIB}")
    install(FILES "${PACT_FFI_LIB}"
            DESTINATION pact_ffi/${PACT_FFI_VERSION}/lib
            RENAME libpact_ffi.a)
  endif()
    if(DEFINED PACT_FFI_SHARED AND EXISTS "${PACT_FFI_SHARED}")
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
endif()

# Add test
#    cmake -S consumer -B build -DPactBuildTests=ON/OFF
# Default OFF: do not build tests unless explicitly requested
option(PactBuildTests "Build consumer tests" OFF)
if(PactBuildTests)
    enable_testing()
    add_subdirectory(test)
endif()