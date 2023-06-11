import os

from conan import ConanFile
from conan.tools.files import copy


class Engine(ConanFile):
    settings = "os", "compiler", "build_type", "arch"
    generators = "CMakeToolchain", "CMakeDeps"

    def requirements(self):
        self.requires("sdl/2.26.1")

    def generate(self):
        include_dir = os.path.join(self.source_folder, 'include')
        for dep in self.dependencies.values():
            for dir in dep.cpp_info.includedirs:
                copy(self, "*.h", dir, include_dir)
                copy(self, "*.hpp", dir, include_dir)

