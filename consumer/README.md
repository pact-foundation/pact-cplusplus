# C++ Consumer DSL

This is a shared library (pact-cpp-consumer) that you can link into your test binaries and it provides a DSL for setting up and running a consumer test. It wraps the Pact-Rust [pact_mock_server_ffi](https://github.com/pact-foundation/pact-reference/tree/master/rust/pact_mock_server_ffi) library, which will need to be linked as well.

## Using Conan package

The library has been released to Bintray as a Conan package. To use it in your project, you need to add https://api.bintray.com/conan/pact-foundation/conan as a remote.

For example:

```console
$ conan remote add pact-foundation https://api.bintray.com/conan/pact-foundation/conan
$ conan search pact_cpp_consumer -r=pact-foundation
Existing package recipes:

pact_cpp_consumer/0.0.0@pact/beta
```

You can then use the library by adding `pact_cpp_consumer/0.0.0@pact/beta` to the dependencies to your project conan file.

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
6. use conan to install all the dependencies

```
conan remote add bincrafters https://api.bintray.com/conan/bincrafters/public-conan
conan remote add pact-foundation https://api.bintray.com/conan/pact-foundation/conan
conan install .. --build missing
```

7. use cmake to build the library

```
 cmake ..
 cmake --build . --config Release
```

On Windows, you may need to set 64 bit architecture.

```
cmake .. -A x64
cmake --build . --config Release
```
