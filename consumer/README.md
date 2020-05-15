# C++ Consumer DSL

This is a shared library (pact-cpp-consumer) that you can link into your test binaries and it provides a DSL for setting up and running a consumer test. It wraps the Pact-Rust [pact_mock_server_ffi](https://github.com/pact-foundation/pact-reference/tree/master/rust/pact_mock_server_ffi) library, which will need to be linked as well.

## Using the DSL

Documentation is [here](docs/dsl.md). For an example, see [consumer_test.cpp](test/src/consumer_test.cpp).

### Setup the expectations 

Firstly, you need to instantiate an instance of `pact_consumer::Pact` in your test. This class takes the name of the consumer
and provider as parameters, and can then be used to define the expectations.

For instance:

```c++
  auto provider = Pact("TodoAppCpp", "TodoServiceCpp");
  provider
    .given("i have a list of projects")
    .uponReceiving("a request for projects")
    .withRequest("GET", "/projects")
    .withQuery(query)
    .withHeaders(headers)
    .willRespondWith(200)
    .withResponseHeaders(res_headers)
    .withResponseJsonBody(Object({
      { "projects", EachLike(Object({
        { "id", Integer(3) },
        { "name", Like("Project 1") },
        { "due", DateTime("yyyy-MM-dd'T'HH:mm:ss.SSSX") },
        { "tasks", AtLeastOneLike(4, Object({
          { "id", Integer() },
          { "name", Matching("Task \\d+", "Task 1") },
          { "done", Like(true) }
        }))}
      }))}
    }));
```

### Execute the test

The actual test is then run with the `run_test` method. This takes a clsoure to execute and will startup a mock server before and the validate the result afterwards.

For example:

```c++
auto result = provider.run_test([] (auto mock_server) {
    // setup any client code you have. The mock_server parameter has the base address of the mock server started for the test
    TodoClient todo;
    todo.serverUrl = mock_server->get_url();

    std::vector<Project> projects = todo.getProjects();
    
    return true; // return a boolean value here to indicate if all is OK
});
```

The `run_test` method will return a `PactTestResult`. Your test should check that this value is ok.

## Using the Conan package

The library has been released to Bintray as a Conan package. To use it in your project, you need to add https://api.bintray.com/conan/pact-foundation/conan as a remote.

For example:

```console
$ conan remote add pact-foundation https://api.bintray.com/conan/pact-foundation/conan
$ conan search pact_cpp_consumer -r=pact-foundation
Existing package recipes:

pact_cpp_consumer/0.0.1@pact/beta
```

You can then use the library by adding `pact_cpp_consumer/0.0.1@pact/beta` to the dependencies to your project conan file.

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
