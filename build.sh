#!/bin/bash

ARG_OS=${1:-}

export _CMAKE_OSX_ARCHITECTURES=""

if [ "$ARG_OS" == "mac" ]; then
  export _CMAKE_OSX_ARCHITECTURES="arm64;x86_64"
  export _BOOST_ROOT=/Users/avancir/dev/fusion/3P/BOOST/1.1850.1/mac64_0002
  export _PACT_FFI_ROOT=/Users/avancir/dev/pact_ffi-v0.4.28  
  echo "Building for macOS"
else
  export _BOOST_ROOT=/home/buser/client-delivery/3P/BOOST/1.1850.1/linux_0003
  export _PACT_FFI_ROOT=/home/buser/pact_ffi-v0.4.28
  echo "Building for linux"
fi

mkdir -p build
cmake -S consumer -B build -DCMAKE_BUILD_TYPE=Release -DBoost_ROOT=$_BOOST_ROOT -DPACT_FFI_ROOT=$_PACT_FFI_ROOT -DPactUseConan=OFF -DPactBuildTests=OFF -DCMAKE_OSX_ARCHITECTURES=$_CMAKE_OSX_ARCHITECTURES
cmake --build build --config Release
cmake --install build --prefix ./build/install/pact-cpp

# cd build/install/pact-cpp
# 7za -mm=lzma -mx9 -r a ../MAC64.zip *
# cd ../../../

# wget https://github.com/pact-foundation/pact-reference/releases/download/libpact_ffi-v0.4.28/pact-cpp.h
# gunzip ./libpact_ffi-linux-x86_64.so.gz