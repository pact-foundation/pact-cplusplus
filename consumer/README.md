# C++ Consumer DSL

## Building the library

To build the library, you need to do the following:

1. Install Python
2. Install CMake
3. Install a C++ compiler
4. Use python/pip to install conan

```
pip install wheel setuptools
pip install conan
```

5. Create a build directory and cd into it
6. Download the Pact-Rust Conan package (https://raw.githubusercontent.com/pact-foundation/pact-reference/master/rust/pact_mock_server_ffi/conanfile.py)
7. use conan to install all the dependencies

```
conan remote add bincrafters https://api.bintray.com/conan/bincrafters/public-conan
conan create . user/channel 
conan install .. --build missing
```

8. use cmake to build the library

```
 cmake ..
 cmake --build . --config Release
```

On Windows, you may need to set 64 bit architecture.

```
cmake .. -A x64
cmake --build . --config Release
```
