import os

from conan import ConanFile
from conan.errors import ConanInvalidConfiguration
from conan.tools.cmake import CMake, CMakeDeps, CMakeToolchain, cmake_layout
from conan.tools.files import copy, rename


class PactcppverifierConan(ConanFile):
    name = "pact_cpp_verifier"
    version = "0.1.0"
    license = "MIT"
    homepage = "https://github.com/pact-foundation/pact-cplusplus"
    description = "Pact C++ Provider Verifier"
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
        self.test_requires("gtest/1.15.0")
        self.test_requires("libcurl/8.10.1")

    def layout(self):
        cmake_layout(self)

    def generate(self):
        CMakeDeps(self).generate()
        tc = CMakeToolchain(self)
        tc.cache_variables["PactUseConan"] = True
        tc.cache_variables["PactBuildTests"] = False
        # The pact_ffi binaries are downloaded outside of conan
        ffi_root = os.environ.get("PACT_FFI_ROOT")
        if not ffi_root:
            raise ConanInvalidConfiguration("PACT_FFI_ROOT must point at the pact_ffi installation")
        tc.cache_variables["PACT_FFI_ROOT"] = ffi_root.replace("\\", "/")
        tc.cache_variables["PACT_FFI_VERSION"] = os.environ.get("PACT_FFI_VERSION", "v0.5.6")
        tc.generate()

    def build(self):
        cmake = CMake(self)
        cmake.configure()
        cmake.build()

    def _ffi_files(self):
        arch = "aarch64" if str(self.settings.arch) in ("armv8", "arm64") else "x86_64"
        if self.settings.os == "Macos":
            return {f"libpact_ffi-macos-{arch}.a": "libpact_ffi.a"}
        if self.settings.os == "Windows":
            return {f"pact_ffi-windows-{arch}.lib": "pact_ffi.lib"}
        # os.distro=alpine is a settings_user.yml sub-setting (see conan-io/conan#16179);
        # it's what actually differentiates musl package_ids across the whole dependency graph
        libc = "-musl" if self.settings.get_safe("os.distro") == "alpine" else ""
        return {f"libpact_ffi-linux-{arch}{libc}.a": "libpact_ffi.a"}

    def package(self):
        copy(self, "*.h",
             src=os.path.join(self.source_folder, "include"),
             dst=os.path.join(self.package_folder, "include"))
        for pattern in ("*.lib", "*.a", "*.so*", "*.dylib*"):
            copy(self, pattern, src=self.build_folder,
                 dst=os.path.join(self.package_folder, "lib"), keep_path=False)
        copy(self, "*.dll", src=self.build_folder,
             dst=os.path.join(self.package_folder, "bin"), keep_path=False)

        # verifier.h includes <pact.h>, so the FFI headers and archive ship with the package
        ffi_root = os.environ["PACT_FFI_ROOT"]
        copy(self, "*.h",
             src=os.path.join(ffi_root, "include"),
             dst=os.path.join(self.package_folder, "include"))
        for source, target in self._ffi_files().items():
            copy(self, source, src=os.path.join(ffi_root, "lib"),
                 dst=os.path.join(self.package_folder, "lib"), keep_path=False)
            rename(self, os.path.join(self.package_folder, "lib", source),
                   os.path.join(self.package_folder, "lib", target))

    def package_info(self):
        self.cpp_info.libs = ["pact-cpp-verifier", "pact_ffi"]
        if self.settings.os == "Windows":
            self.cpp_info.system_libs = ["ws2_32", "userenv", "crypt32",
                                         "secur32", "dnsapi", "ncrypt", "ntdll"]
        elif self.settings.os == "Macos":
            self.cpp_info.frameworks = ["CoreFoundation", "Security",
                                        "SystemConfiguration", "IOKit"]
            self.cpp_info.system_libs = ["bz2", "lzma", "iconv"]
        else:
            self.cpp_info.system_libs = ["pthread", "dl", "m", "bz2", "lzma"]
