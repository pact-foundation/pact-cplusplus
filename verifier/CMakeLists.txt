cmake_minimum_required(VERSION 3.16)

project(pact-cpp-verifier VERSION 0.1.0)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Conan integration (optional, default OFF)
#    cmake -S verifier -B build -DPactUseConan=ON/OFF
# Conan 2 supplies dependencies through CMakeDeps/CMakeToolchain, so nothing is
# needed here beyond skipping the vendored FetchContent fallbacks below.
option(PactUseConan "Use Conan build integration if available" OFF)

# Ask the compiler what it actually targets. Under emulation (e.g. an x86_64
# OrbStack machine on Apple Silicon) uname/CMAKE_*_SYSTEM_PROCESSOR can still
# report the native host arch, which would select the wrong pact_ffi binary.
if(UNIX AND NOT APPLE)
  execute_process(
    COMMAND "${CMAKE_CXX_COMPILER}" -dumpmachine
    OUTPUT_VARIABLE _pact_ffi_triple
    OUTPUT_STRIP_TRAILING_WHITESPACE
    ERROR_QUIET
  )
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
    if(_pact_ffi_triple MATCHES "^(arm64|aarch64)")
      set(PACT_FFI_ARCH aarch64)
    elseif(_pact_ffi_triple MATCHES "^(x86_64|amd64)")
      set(PACT_FFI_ARCH x86_64)
    elseif(CMAKE_SYSTEM_PROCESSOR MATCHES "arm64|aarch64")
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

# Detect target libc on Linux (gnu or musl/Alpine)
if(NOT DEFINED PACT_FFI_LIBC AND DEFINED ENV{PACT_FFI_LIBC})
  set(PACT_FFI_LIBC "$ENV{PACT_FFI_LIBC}")
endif()

if(UNIX AND NOT APPLE AND NOT DEFINED PACT_FFI_LIBC)
  if(_pact_ffi_triple MATCHES "musl")
    set(PACT_FFI_LIBC musl)
  elseif(EXISTS "/etc/alpine-release")
    set(PACT_FFI_LIBC musl)
  else()
    set(PACT_FFI_LIBC gnu)
  endif()
endif()

if(PACT_FFI_LIBC STREQUAL "musl")
  set(_PACT_FFI_LIBC_SUFFIX "-musl")
else()
  set(_PACT_FFI_LIBC_SUFFIX "")
endif()

message(STATUS "PactFFI: resolved arch '${PACT_FFI_ARCH}'")
if(UNIX AND NOT APPLE)
  message(STATUS "PactFFI: resolved libc '${PACT_FFI_LIBC}'")
endif()

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
  set(_PACT_FFI_IMPLIB_FILENAME "pact_ffi-${PACT_FFI_OS_ARCH}.dll.lib")
  set(_PACT_FFI_DLL_FILENAME "pact_ffi-${PACT_FFI_OS_ARCH}.dll")
else()
  set(PACT_FFI_OS_ARCH "linux-${PACT_FFI_ARCH}${_PACT_FFI_LIBC_SUFFIX}")
  set(_PACT_FFI_FILENAME "libpact_ffi-${PACT_FFI_OS_ARCH}.a")
  set(_PACT_FFI_SHARED_FILENAME "libpact_ffi-${PACT_FFI_OS_ARCH}.so")
endif()

set(PACT_FFI_LIB "${PACT_FFI_LIB_DIR}/${_PACT_FFI_FILENAME}")
if(DEFINED _PACT_FFI_SHARED_FILENAME)
  set(PACT_FFI_SHARED "${PACT_FFI_LIB_DIR}/${_PACT_FFI_SHARED_FILENAME}")
endif()
if(DEFINED _PACT_FFI_IMPLIB_FILENAME)
  set(PACT_FFI_IMPLIB "${PACT_FFI_LIB_DIR}/${_PACT_FFI_IMPLIB_FILENAME}")
endif()
if(DEFINED _PACT_FFI_DLL_FILENAME)
  set(PACT_FFI_DLL "${PACT_FFI_LIB_DIR}/${_PACT_FFI_DLL_FILENAME}")
endif()

# Rust's staticlib output doesn't bundle native static libs pulled in by a
# dependency's build script (pact-plugin-driver's mlua uses vendored+lua54,
# compiled via a `cc` build script), so the plain .lib/.a is missing Lua
# symbols such as lua_pushinteger. Link against the fully-linked shared
# artifact instead: the DLL's import .lib on Windows, the .dylib/.so
# elsewhere.
if(WIN32)
  set(PACT_FFI_LINK_LIB "${PACT_FFI_IMPLIB}")
else()
  set(PACT_FFI_LINK_LIB "${PACT_FFI_SHARED}")

  # The released dylib/so's install name/soname is the bare "libpact_ffi.dylib"/
  # ".so" (no path), so the dynamic loader won't find the arch-suffixed filename
  # we downloaded it as. Symlink a bare-named copy alongside it so DYLD/LD
  # library path lookups against PACT_FFI_LIB_DIR succeed for local builds/tests.
  if(DEFINED PACT_FFI_SHARED AND EXISTS "${PACT_FFI_SHARED}")
    if(APPLE)
      set(_pact_ffi_bare "${PACT_FFI_LIB_DIR}/libpact_ffi.dylib")
    else()
      set(_pact_ffi_bare "${PACT_FFI_LIB_DIR}/libpact_ffi.so")
    endif()
    if(NOT EXISTS "${_pact_ffi_bare}")
      file(CREATE_LINK "${PACT_FFI_SHARED}" "${_pact_ffi_bare}" SYMBOLIC)
    endif()
  endif()
endif()

# Add test
#    cmake -S verifier -B build -DPactBuildTests=ON/OFF
# Default OFF: do not build tests unless explicitly requested
option(PactBuildTests "Build verifier tests" OFF)

# Optional macOS universal build via lipo for pact_ffi
# Provide -DPactBuildUniversal=ON; expects both arm64 and x86_64 libs in the same PACT_FFI_ROOT/lib
# Conan/test dependencies are built for a single arch, so universal is off in that case
if(PactUseConan OR PactBuildTests)
  set(_pact_universal_default OFF)
else()
  set(_pact_universal_default ON)
endif()
option(PactBuildUniversal "Build macOS universal binaries (arm64 + x86_64)" ${_pact_universal_default})
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
  set(PACT_FFI_LINK_LIB "${_ffi_univ_dyl}")
endif()

if(PactUseConan)
  find_package(nlohmann_json REQUIRED CONFIG)
else()
  # Fetch nlohmann/json
  include(FetchContent)
  FetchContent_Declare(
    nlohmann_json
    GIT_REPOSITORY https://github.com/nlohmann/json.git
    GIT_TAG v3.12.0
  )
  FetchContent_MakeAvailable(nlohmann_json)
endif()

# Sources
add_subdirectory(src)

if(TARGET pact-cpp-verifier)
  set_target_properties(pact-cpp-verifier PROPERTIES
    OUTPUT_NAME "pact-cpp-verifier"
  )
  target_include_directories(pact-cpp-verifier PRIVATE "${PACT_FFI_INCLUDE}")
  target_link_libraries(pact-cpp-verifier PRIVATE "${PACT_FFI_LIB}" nlohmann_json::nlohmann_json)

  # System dependencies of the Rust crates bundled inside the static pact_ffi archive
  if(APPLE)
    target_link_libraries(pact-cpp-verifier PUBLIC
      "-framework CoreFoundation"
      "-framework Security"
      "-framework SystemConfiguration"
      "-framework IOKit"
      bz2 lzma iconv)
  elseif(UNIX)
    find_package(Threads REQUIRED)
    target_link_libraries(pact-cpp-verifier PUBLIC
      Threads::Threads ${CMAKE_DL_LIBS} m bz2 lzma)
  endif()
endif()

# Install project lib + headers
function(pact_normalize_install_path output path)
  if(COMMAND cmake_path)
    cmake_path(SET normalized_path NORMALIZE "${path}")
  else()
    set(normalized_path "${path}")
  endif()
  set(${output} "${normalized_path}" PARENT_SCOPE)
endfunction()

pact_normalize_install_path(PACT_VERIFIER_INSTALL_LIB_DIR
  "pact-cpp-verifier/${PROJECT_VERSION}/lib")
pact_normalize_install_path(PACT_VERIFIER_INSTALL_INCLUDE_DIR
  "pact-cpp-verifier/${PROJECT_VERSION}/include")
pact_normalize_install_path(PACT_FFI_INSTALL_INCLUDE_DIR
  "pact_ffi/${PACT_FFI_VERSION}/include")
pact_normalize_install_path(PACT_FFI_INSTALL_LIB_DIR
  "pact_ffi/${PACT_FFI_VERSION}/lib")
pact_normalize_install_path(PACT_FFI_INSTALL_BIN_DIR
  "pact_ffi/${PACT_FFI_VERSION}/bin")

install(TARGETS pact-cpp-verifier
  ARCHIVE DESTINATION "${PACT_VERIFIER_INSTALL_LIB_DIR}"
)
install(FILES include/verifier.h include/http_server.h
  DESTINATION "${PACT_VERIFIER_INSTALL_INCLUDE_DIR}"
)

# Install pact_ffi headers
if(EXISTS "${PACT_FFI_INCLUDE}")
  install(DIRECTORY "${PACT_FFI_INCLUDE}/"
      DESTINATION "${PACT_FFI_INSTALL_INCLUDE_DIR}"
          FILES_MATCHING PATTERN "*.h")
endif()

# Install pact_ffi libraries
if(WIN32)
  if(EXISTS "${PACT_FFI_LIB}")
    install(FILES "${PACT_FFI_LIB}"
          DESTINATION "${PACT_FFI_INSTALL_LIB_DIR}"
            RENAME pact_ffi.lib)
  endif()
  if(DEFINED PACT_FFI_IMPLIB AND EXISTS "${PACT_FFI_IMPLIB}")
    install(FILES "${PACT_FFI_IMPLIB}"
          DESTINATION "${PACT_FFI_INSTALL_LIB_DIR}"
            RENAME pact_ffi.dll.lib)
  endif()
  if(DEFINED PACT_FFI_DLL AND EXISTS "${PACT_FFI_DLL}")
    install(FILES "${PACT_FFI_DLL}"
          DESTINATION "${PACT_FFI_INSTALL_BIN_DIR}"
            RENAME pact_ffi.dll)
  endif()
else()
  if(APPLE AND PactBuildUniversal)
    # Universal install: no existence guards, because pact-cpp-verifier depends on pact_ffi_universal
    install(FILES "${_ffi_univ_lib}"
          DESTINATION "${PACT_FFI_INSTALL_LIB_DIR}"
            RENAME libpact_ffi.a)
    install(FILES "${_ffi_univ_dyl}"
          DESTINATION "${PACT_FFI_INSTALL_LIB_DIR}"
            RENAME libpact_ffi.dylib)
  else()
    # Per-arch fallback
    if(EXISTS "${PACT_FFI_LIB}")
      install(FILES "${PACT_FFI_LIB}"
              DESTINATION "${PACT_FFI_INSTALL_LIB_DIR}"
              RENAME libpact_ffi.a)
    endif()
    if(DEFINED PACT_FFI_SHARED AND EXISTS "${PACT_FFI_SHARED}")
      if(APPLE)
        install(FILES "${PACT_FFI_SHARED}"
          DESTINATION "${PACT_FFI_INSTALL_LIB_DIR}"
                RENAME libpact_ffi.dylib)
      else()
        install(FILES "${PACT_FFI_SHARED}"
          DESTINATION "${PACT_FFI_INSTALL_LIB_DIR}"
                RENAME libpact_ffi.so)
      endif()
    endif()
  endif()
endif()

# Add test
#    cmake -S verifier -B build -DPactBuildTests=ON/OFF
if(PactBuildTests)
    enable_testing()
    add_subdirectory(test)
endif()
