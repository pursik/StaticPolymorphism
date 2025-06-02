from conan import ConanFile

class Recipe(ConanFile):
    name = "dataexchange"
    version = "1.0"
    settings = "os", "compiler", "build_type", "arch"
    requires =  ["boost/1.81.0", "gtest/[>=1.11.0]", "cryptopp/8.9.0"]
    generators = "CMakeDeps", "CMakeToolchain"

    def configure(self):
        self.options["boost"].header_only = False
        self.options["cryptopp"].header_only = False

    def configure(self):
        # Apply specific configurations for debug and release
        if self.settings.build_type == "Debug":
            self.options["boost"].build_type = "Debug"
        elif self.settings.build_type == "Release":
            self.options["boost"].build_type = "Release"

    def layout(self):
        self.folders.build = "build"
        self.folders.source = "src"
