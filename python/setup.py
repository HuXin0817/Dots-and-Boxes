import os
import subprocess
import sys
from pathlib import Path
from setuptools import setup, Extension
from setuptools.command.build_ext import build_ext
from setuptools import find_packages


class CMakeExtension(Extension):
    def __init__(self, name, sourcedir=""):
        Extension.__init__(self, name, sources=[])
        self.sourcedir = os.path.abspath(sourcedir)


class CMakeBuild(build_ext):
    def build_extension(self, ext):
        if not isinstance(ext, CMakeExtension):
            super().build_extension(ext)
            return

        extdir = os.path.abspath(os.path.dirname(
            self.get_ext_fullpath(ext.name)))

        cmake_args = [
            f"-DCMAKE_LIBRARY_OUTPUT_DIRECTORY={extdir}",
            f"-DPYTHON_EXECUTABLE={sys.executable}",
            "-DCMAKE_BUILD_TYPE=Release",
        ]

        build_temp = Path(self.build_temp) / ext.name
        build_temp.mkdir(parents=True, exist_ok=True)

        subprocess.check_call(["cmake", ext.sourcedir] +
                              cmake_args, cwd=build_temp)
        subprocess.check_call(
            ["cmake", "--build", ".", "--config",
                "Release", "-j", str(os.cpu_count())],
            cwd=build_temp,
        )


setup(
    name="PyDab",
    version="0.1",
    ext_modules=[
        CMakeExtension(
            "PyDab", os.path.dirname(
                os.path.dirname(os.path.abspath(__file__)))
        )
    ],
    cmdclass={"build_ext": CMakeBuild},
    zip_safe=False,
    packages=find_packages(),
)
