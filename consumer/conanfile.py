from conans import ConanFile, CMake


class PactcppconsumerConan(ConanFile):
    name = "pact_cpp_consumer"
    version = "0.1.1"
    license = "MIT"
    homepage = "https://github.com/pact-foundation/pact-cplusplus"
    description = "Pact C++ Consumer DSL"
    topics = ("Contract Testing")
    settings = "os", "compiler", "build_type", "arch"
    options = {"shared": [True, False]}
    default_options = {"shared": False}
    generators = "cmake"
    exports_sources = "src/*", "include/*"
    requires = "pact_ffi/0.0.0@pact/beta", "nlohmann_json/3.7.3", "gtest/1.10.0", "cpprestsdk/2.10.15"
    scm = {
        "type": "git",
        "subfolder": "src",
        "url": "auto",
        "revision": "auto"
     }

    def build(self):
        cmake = CMake(self)
        cmake.configure(source_folder="src")
        cmake.build()

    def package(self):
        self.copy("*.h", dst="include", src="include")
        self.copy("*.lib", dst="lib", keep_path=False)
        self.copy("*.dll", dst="bin", keep_path=False)
        self.copy("*.dylib*", dst="lib", keep_path=False)
        self.copy("*.so", dst="lib", keep_path=False)
        self.copy("*.a", dst="lib", keep_path=False)

    def package_info(self):
        self.cpp_info.libs = ["pact-cpp-consumer"]
