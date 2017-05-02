#!/usr/bin/env python
import os
import sys
import subprocess
import datetime

from setuptools import setup, Extension
from setuptools.command.build_ext import build_ext
from setuptools.command.test import test

import importlib.util

if sys.version_info[0] == 2:
    sys.exit('Sorry, Python 2.x is not supported')


def check_storm_compatible(storm_v_major, storm_v_minor, storm_v_patch):
    if storm_v_major < 1 or (storm_v_major == 1 and storm_v_minor == 0 and storm_v_patch < 1):
        sys.exit('Sorry, Storm version {}.{}.{} is not supported anymore!'.format(storm_v_major, storm_v_minor, storm_v_patch))

def parse_storm_version(version_string):
    elems = version_string.split(".")
    if len(elems) != 3:
        sys.exit('Storm version string is ill-formed: "{}"'.format(version_string))
    return int(elems[0]),  int(elems[1]), int(elems[2])

class CMakeExtension(Extension):
    def __init__(self, name, sourcedir='', subdir=''):
        Extension.__init__(self, name, sources=[])
        self.sourcedir = os.path.abspath(sourcedir)
        self.subdir = subdir



class CMakeBuild(build_ext):
    user_options = build_ext.user_options + [
        ('storm-dir=', None, 'Path to storm root (binary) location')
    ]


    def extdir(self, extname):
        return os.path.abspath(os.path.dirname(self.get_ext_fullpath(extname)))

    def run(self):
        self.conf = None
        try:
            _ = subprocess.check_output(['cmake', '--version'])
        except OSError:
            raise RuntimeError("CMake must be installed to build the following extensions: " +
                               ", ".join(e.name for e in self.extensions))

        build_temp_version = self.build_temp + "-version"
        if not os.path.exists(build_temp_version):
            os.makedirs(build_temp_version)

        # Check cmake variable values
        cmake_args = []
        if self.storm_dir is not None:
            cmake_args = ['-Dstorm_DIR=' + self.storm_dir]
        output = subprocess.check_output(['cmake', os.path.abspath("cmake")] + cmake_args, cwd=build_temp_version)
        spec = importlib.util.spec_from_file_location("genconfig", os.path.join(build_temp_version, 'generated/config.py'))
        self.conf = importlib.util.module_from_spec(spec)
        spec.loader.exec_module(self.conf)
        print(self.conf.HAVE_STORM_DFT)

        storm_v_major, storm_v_minor, storm_v_patch = parse_storm_version(self.conf.STORM_VERSION)
        check_storm_compatible(storm_v_major, storm_v_minor, storm_v_patch)



        for ext in self.extensions:
            if ext.name == "core":
                with open(os.path.join(self.extdir(ext.name), ext.subdir, "_config.py"), "w") as f:
                    f.write("# Generated from setup.py at {}\n".format(datetime.datetime.now()))

                    f.write("import pycarl\n")
                    if self.conf.STORM_CLN_EA or self.conf.STORM_CLN_RF:
                        f.write("import pycarl.cln\n")
                    if not self.conf.STORM_CLN_EA or not self.conf.STORM_CLN_RF:
                        f.write("import pycarl.gmp\n")

                    if self.conf.STORM_CLN_EA:
                        f.write("Rational = pycarl.cln.Rational\n")
                    else:
                        f.write("Rational = pycarl.gmp.Rational\n")

                    if self.conf.STORM_CLN_RF:
                        rfpackage = "cln"
                    else:
                        rfpackage = "gmp"
                    f.write("RationalRF = pycarl.{}.Rational\n".format(rfpackage))
                    f.write("Polynomial = pycarl.{}.Polynomial\n".format(rfpackage))
                    f.write("FactorizedPolynomial = pycarl.{}.FactorizedPolynomial\n".format(rfpackage))
                    f.write("RationalFunction = pycarl.{}.RationalFunction\n".format(rfpackage))
                    f.write("FactorizedRationalFunction = pycarl.{}.FactorizedRationalFunction\n".format(rfpackage))

            elif ext.name == "info":
                with open(os.path.join(self.extdir(ext.name), ext.subdir, "_config.py"), "w") as f:
                    f.write("# Generated from setup.py at {}\n".format(datetime.datetime.now()))
                    f.write("storm_version = {}\n".format(self.conf.STORM_VERSION))
                    f.write("storm_cln_ea = {}\n".format(self.conf.STORM_CLN_EA))
                    f.write("storm_cln_rf = {}".format(self.conf.STORM_CLN_RF))
            elif ext.name == "dft":
                with open(os.path.join(self.extdir(ext.name),  ext.subdir, "_config.py"), "w") as f:
                    f.write("# Generated from setup.py at {}\n".format(datetime.datetime.now()))
                    f.write("has_storm_dft = {}".format(self.conf.HAVE_STORM_DFT))
                if not self.conf.HAVE_STORM_DFT:
                    print("WARNING: storm-dft not found. No support for DFTs will be built.")
                    continue
            self.build_extension(ext)




    def initialize_options(self):
        build_ext.initialize_options(self)
        self.storm_dir = None

    def finalize_options(self):
        if self.storm_dir is not None:
            print('The custom storm directory', self.storm_dir)
        build_ext.finalize_options(self)

    def build_extension(self, ext):
        extdir = self.extdir(ext.name)
        print(extdir)
        cmake_args = ['-DSTORMPY_LIB_DIR=' + extdir,
                      '-DPYTHON_EXECUTABLE=' + sys.executable]

        cfg = 'Release'  # if self.debug else 'Release'
        build_args = ['--config', cfg]

        cmake_args += ['-DCMAKE_BUILD_TYPE=' + cfg]
        build_args += ['--', '-j{}'.format(os.cpu_count() if os.cpu_count() is not None else 2)]
        if self.conf.STORM_DIR is not None:
            cmake_args += ['-Dstorm_DIR=' + self.conf.STORM_DIR]
        if self.conf.HAVE_STORM_DFT:
            cmake_args += ['-DHAVE_STORM_DFT=ON']

        env = os.environ.copy()
        env['CXXFLAGS'] = '{} -DVERSION_INFO=\\"{}\\"'.format(env.get('CXXFLAGS', ''),
                                                              self.distribution.get_version())
        if not os.path.exists(self.build_temp):
            os.makedirs(self.build_temp)
        subprocess.check_call(['cmake', ext.sourcedir] + cmake_args, cwd=self.build_temp, env=env)
        subprocess.check_call(['cmake', '--build', '.', '--target', ext.name] + build_args, cwd=self.build_temp)


class PyTest(test):
    def run_tests(self):
        #import here, cause outside the eggs aren't loaded
        import pytest
        errno = pytest.main(['tests'])
        sys.exit(errno)


setup(
    name="stormpy",
    version="0.9.1",
    author="M. Volk",
    author_email="matthias.volk@cs.rwth-aachen.de",
    maintainer="S. Junges",
    maintainer_email="sebastian.junges@cs.rwth-aachen.de",
    url="http://moves.rwth-aachen.de",
    description="stormpy - Python Bindings for Storm",
    packages=['stormpy', 'stormpy.info', 'stormpy.expressions', 'stormpy.logic', 'stormpy.storage', 'stormpy.utility', 'stormpy.dft'],
    package_dir={'': 'lib'},
    ext_package='stormpy',
    ext_modules=[CMakeExtension('core', subdir=''),
                 CMakeExtension('info', subdir='info'),
                 CMakeExtension('expressions', subdir='expressions'),
                 CMakeExtension('logic', subdir='logic'),
                 CMakeExtension('storage', subdir='storage'),
                 CMakeExtension('utility', subdir='utility'),
                 CMakeExtension('dft', subdir='dft')
                 ],
    cmdclass={'build_ext': CMakeBuild, 'test': PyTest},
    zip_safe=False,
    install_requires=['pycarl>=2.0.0'],
    tests_require=['pytest'],
)
