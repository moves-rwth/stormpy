import os
import multiprocessing
import sys
import subprocess
import importlib
import datetime

from setuptools import setup, Extension
from setuptools.command.build_ext import build_ext
from setuptools.command.test import test

import setup.helper as setup_helper
from setup.config import SetupConfig

if sys.version_info[0] == 2:
    sys.exit('Sorry, Python 2.x is not supported')

config_path = "build/build_config.cfg"


class CMakeExtension(Extension):
    def __init__(self, name, sourcedir='', subdir=''):
        Extension.__init__(self, name, sources=[])
        self.sourcedir = os.path.abspath(sourcedir)
        self.subdir = subdir


class CMakeBuild(build_ext):
    user_options = build_ext.user_options + [
        ('carl-dir=', None, 'Path to carl root (binary) location'),
        ('carl-parser-dir=', None, 'Path to carl-parser root (binary) location'),
        ('jobs=', 'j', 'Number of jobs to use for compiling'),
        ('debug', None, 'Build in Debug mode'),
    ]

    config = SetupConfig()

    def _extdir(self, extname):
        return os.path.abspath(os.path.dirname(self.get_ext_fullpath(extname)))

    def run(self):
        try:
            out = subprocess.check_output(['cmake', '--version'])
        except OSError:
            raise RuntimeError("CMake must be installed to build the following extensions: " +
                               ", ".join(e.name for e in self.extensions))

        # Build cmake version info
        build_temp_version = self.build_temp + "-version"
        if not os.path.exists(build_temp_version):
            os.makedirs(build_temp_version)
        self.config.write_config(config_path)

        cmake_args = []
        carl_dir = self.config.get_as_string("carl_dir")
        carl_parser_dir = self.config.get_as_string("carl_parser_dir")
        if carl_dir:
            cmake_args += ['-Dcarl_DIR=' + carl_dir]
        if carl_parser_dir:
            cmake_args += ['-Dcarl_parser_DIR=' + carl_parser_dir]
        output = subprocess.check_output(['cmake', os.path.abspath("cmake")] + cmake_args, cwd=build_temp_version)
        if sys.version_info[1] >= 5:
            # Method for Python >= 3.5
            spec = importlib.util.spec_from_file_location("genconfig",
                                                          os.path.join(build_temp_version, 'generated/config.py'))
            self.conf = importlib.util.module_from_spec(spec)
            spec.loader.exec_module(self.conf)
        else:
            # Deprecated method for Python <= 3.4
            from importlib.machinery import SourceFileLoader
            self.conf = SourceFileLoader("genconfig",
                                         os.path.join(build_temp_version, "generated/config.py")).load_module()

        # check version
        major, minor, patch = setup_helper.parse_carl_version(self.conf.CARL_VERSION)
        setup_helper.check_carl_compatible(self.conf.CARL_DIR, major, minor, patch)

        print("Pycarl - Using carl {} from {}".format(self.conf.CARL_VERSION, self.conf.CARL_DIR))
        if self.conf.CARL_PARSER:
            print("Pycarl - Carl parser extension found and included.")
        else:
            print("Pycarl - Warning: No parser support!")
        if self.conf.CARL_WITH_CLN:
            print("Pycarl - Support for CLN found and included.")
        else:
            print("Pycarl - Warning: No support for CLN!")

        for ext in self.extensions:
            setup_helper.ensure_dir_exists(os.path.join(self._extdir(ext.name), ext.subdir))
            if "core" in ext.name:
                with open(os.path.join(self._extdir(ext.name), ext.subdir, "_config.py"), "w") as f:
                    f.write("# Generated from setup.py at {}\n".format(datetime.datetime.now()))
                    f.write('CARL_VERSION = "{}.{}.{}"\n'.format(major, minor, patch))
                    f.write("CARL_PARSER = {}\n".format(self.conf.CARL_PARSER))
                    f.write("CARL_WITH_CLN = {}\n".format(self.conf.CARL_WITH_CLN))
            if "cln" in ext.name:
                with open(os.path.join(self._extdir(ext.name), ext.subdir, "_config.py"), "w") as f:
                    f.write("# Generated from setup.py at {}\n".format(datetime.datetime.now()))
                    f.write("CARL_WITH_CLN = {}\n".format(self.conf.CARL_WITH_CLN))
                if not self.conf.CARL_WITH_CLN:
                    print("Pycarl - CLN bindings skipped")
                    continue
            if "parse" in ext.name:
                with open(os.path.join(self._extdir(ext.name), ext.subdir, "_config.py"), "w") as f:
                    f.write("# Generated from setup.py at {}\n".format(datetime.datetime.now()))
                    f.write("CARL_PARSER = {}\n".format(self.conf.CARL_PARSER))
                if not self.conf.CARL_PARSER:
                    print("Pycarl - Parser bindings skipped")
                    continue
            self.build_extension(ext)

    def initialize_options(self):
        build_ext.initialize_options(self)
        self.carl_dir = None
        self.carl_parser_dir = None
        self.debug = None
        self.jobs = None

    def finalize_options(self):
        self.config.load_from_file(config_path)
        self.config.update("carl_dir", self.carl_dir)
        self.config.update("carl_parser_dir", self.carl_parser_dir)
        self.config.update("debug", self.debug)
        self.config.update("jobs", self.jobs)
        print('Pycarl - The custom carl directory', self.config.get_as_string("carl_dir"))
        print('Pycarl - The custom carl-parser directory', self.config.get_as_string("carl_parser_dir"))
        build_ext.finalize_options(self)

    def build_extension(self, ext):
        extdir = os.path.abspath(os.path.dirname(self.get_ext_fullpath(ext.name)))
        cmake_args = ['-DCMAKE_LIBRARY_OUTPUT_DIRECTORY=' + os.path.join(extdir, ext.subdir),
                      '-DPYTHON_EXECUTABLE=' + sys.executable]

        build_type = 'Debug' if self.config.get_as_bool("debug") else 'Release'
        build_args = ['--config', build_type]
        build_args += ['--', '-j{}'.format(self.config.get_as_int("jobs"))]
        cmake_args += ['-DCMAKE_BUILD_TYPE=' + build_type]
        carl_dir = self.config.get_as_string("carl_dir")
        carl_parser_dir = self.config.get_as_string("carl_parser_dir")
        if carl_dir is not None:
            cmake_args += ['-Dcarl_DIR=' + carl_dir]
        if carl_parser_dir:
            cmake_args += ['-Dcarl_parser_DIR=' + carl_parser_dir]

        env = os.environ.copy()
        env['CXXFLAGS'] = '{} -DVERSION_INFO=\\"{}\\"'.format(env.get('CXXFLAGS', ''),
                                                              self.distribution.get_version())
        if not os.path.exists(self.build_temp):
            os.makedirs(self.build_temp)
        print("Pycarl - CMake args={}".format(cmake_args))
        # Call cmake
        subprocess.check_call(['cmake', ext.sourcedir] + cmake_args, cwd=self.build_temp, env=env)
        subprocess.check_call(['cmake', '--build', '.', '--target', ext.name] + build_args, cwd=self.build_temp)


class PyTest(test):
    def run_tests(self):
        # import here, cause outside the eggs aren't loaded
        import pytest
        errno = pytest.main(['tests'])
        sys.exit(errno)


setup(
    name='pycarl',
    version=setup_helper.obtain_version(),
    author="H. Bruintjes",
    author_email="h.bruintjes@cs.rwth-aachen.de",
    maintainer="S. Junges",
    maintainer_email="sebastian.junges@cs.rwth-aachen.de",
    url="http://moves.rwth-aachen.de",
    description="pycarl - Python Bindings for CArL",
    packages=['pycarl',
              'pycarl.cln',
              'pycarl.gmp',
              'pycarl.formula',
              'pycarl.cln.formula',
              'pycarl.gmp.formula',
              'pycarl.parse',
              'pycarl.gmp.parse',
              'pycarl.cln.parse'],
    package_dir={'': 'lib'},
    long_description='',
    ext_package='pycarl',
    ext_modules=[CMakeExtension('core', subdir=''),
                 CMakeExtension('cln', subdir='cln'),
                 CMakeExtension('gmp', subdir='gmp'),
                 CMakeExtension('formula', subdir='formula'),
                 CMakeExtension('formula-cln', subdir='cln/formula'),
                 CMakeExtension('formula-gmp', subdir='gmp/formula'),
                 CMakeExtension('parse', subdir='parse'),
                 CMakeExtension('parse-gmp', subdir='gmp/parse'),
                 CMakeExtension('parse-cln', subdir='cln/parse')],

    cmdclass={'build_ext': CMakeBuild, 'test': PyTest},
    zip_safe=False,
    setup_requires=['pytest-runner'],
    tests_require=['pytest'],
    python_requires='>=3',
)
