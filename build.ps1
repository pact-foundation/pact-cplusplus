# Windows build without Conan. pact_ffi is downloaded and checksum-verified at
# configure time; add -DPACT_FFI_ROOT=<dir> to use a local copy instead.
# Point Boost_ROOT at a Boost install (headers only are needed).

$BoostRoot = $env:BOOST_ROOT

md build -Force
cmake -S consumer -B build -DBoost_ROOT="$BoostRoot" -DPactUseConan=OFF -DPactBuildTests=OFF
cmake --build build --config Release
cmake --install build --config Release --prefix .\build\install\pact-cpp

md build-verifier -Force
cmake -S verifier -B build-verifier -DPactUseConan=OFF -DPactBuildTests=OFF
cmake --build build-verifier --config Release
cmake --install build-verifier --config Release --prefix .\build\install\pact-cpp
