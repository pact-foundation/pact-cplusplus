import os

from conan import ConanFile
from conan.tools.cmake import CMake, CMakeDeps, CMakeToolchain, cmake_layout
from conan.tools.files import copy


class PactcppconsumerConan(ConanFile):
    name = "pact_cpp_consumer"
    version = "0.1.1"
    license = "MIT"
    homepage = "https://github.com/pact-foundation/pact-cplusplus"
    description = "Pact C++ Consumer DSL"
    topics = ("contract-testing", "pact")
    package_type = "static-library"
    settings = "os", "compiler", "build_type", "arch"
    options = {"fPIC": [True, False]}
    default_options = {"fPIC": True}
    exports_sources = "CMakeLists.txt", "src/*", "include/*", "test/*"

    def config_options(self):
        if self.settings.os == "Windows":
            del self.options.fPIC

    def requirements(self):
        self.requires("nlohmann_json/3.11.3")

    def build_requirements(self):
        self.requires("boost/1.83.0", transitive_headers=True)
        self.test_requires("gtest/1.15.0")
        self.test_requires("libcurl/8.10.1")

    def layout(self):
        cmake_layout(self)

    def generate(self):
        CMakeDeps(self).generate()
        tc = CMakeToolchain(self)
        tc.cache_variables["PactUseConan"] = True
        tc.cache_variables["PactBuildTests"] = False
        tc.generate()

    def build(self):
        cmake = CMake(self)
        cmake.configure()
        cmake.build()

    def package(self):
        copy(self, "*.h",
             src=os.path.join(self.source_folder, "include"),
             dst=os.path.join(self.package_folder, "include"))
        for pattern in ("*.lib", "*.a", "*.so*", "*.dylib*"):
            copy(self, pattern, src=self.build_folder,
                 dst=os.path.join(self.package_folder, "lib"), keep_path=False)
        copy(self, "*.dll", src=self.build_folder,
             dst=os.path.join(self.package_folder, "bin"), keep_path=False)

    def package_info(self):
        self.cpp_info.libs = ["pact-cpp-consumer"]
        if self.settings.os == "Windows":
            self.cpp_info.system_libs = ["ws2_32", "userenv", "crypt32",
                                         "secur32", "dnsapi", "ncrypt", "ntdll"]
