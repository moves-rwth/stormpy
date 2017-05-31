import os
import multiprocessing
import re
import sys
import platform
import subprocess

from setuptools import setup, Extension
from setuptools.command.build_ext import build_ext

def obtain_version():
    verstr = "unknown"
    try:
        verstrline = open('lib/pycarl/_version.py', "rt").read()
    except EnvironmentError:
        pass # Okay, there is no version file.
    else:
        VSRE = r"^__version__ = ['\"]([^'\"]*)['\"]"
        mo = re.search(VSRE, verstrline, re.M)
        if mo:
            verstr = mo.group(1)
        else:
            raise RuntimeError("unable to find version in pycarl/_version.py")
    return verstr


class CMakeExtension(Extension):
    def __init__(self, name, sourcedir='', subdir=''):
        Extension.__init__(self, name, sources=[])
        self.sourcedir = os.path.abspath(sourcedir)
        self.subdir = subdir


class CMakeBuild(build_ext):

    user_options = build_ext.user_options + [
            ('carl-dir=', None, 'Path to carl root (binary) location'),
            ('jobs=', 'j', 'Number of jobs to use for compiling'),
            ('debug', None, 'Build in Debug mode'),
        ]

    def run(self):
        try:
            out = subprocess.check_output(['cmake', '--version'])
        except OSError:
            raise RuntimeError("CMake must be installed to build the following extensions: " +
                               ", ".join(e.name for e in self.extensions))

        for ext in self.extensions:
            self.build_extension(ext)


    def initialize_options(self):
        build_ext.initialize_options(self)
        self.carl_dir = None
        self.debug = False
        try:
            self.jobs = multiprocessing.cpu_count() if multiprocessing.cpu_count() is not None else 1
        except NotImplementedError:
            self.jobs = 1

    def finalize_options(self):
        if self.carl_dir:
            print('The custom carl directory', self.carl_dir)
        build_ext.finalize_options(self)

    def build_extension(self, ext):
        extdir = os.path.abspath(os.path.dirname(self.get_ext_fullpath(ext.name)))
        cmake_args = ['-DCMAKE_LIBRARY_OUTPUT_DIRECTORY=' + os.path.join(extdir, ext.subdir),
                      '-DPYTHON_EXECUTABLE=' + sys.executable]

        build_type = 'Debug' if self.debug else 'Release'
        build_args = ['--config', build_type]
        build_args += ['--', '-j{}'.format(self.jobs)]
        cmake_args += ['-DCMAKE_BUILD_TYPE=' + build_type]
        if self.carl_dir:
            cmake_args += ['-Dcarl_DIR=' + self.carl_dir]

        env = os.environ.copy()
        env['CXXFLAGS'] = '{} -DVERSION_INFO=\\"{}\\"'.format(env.get('CXXFLAGS', ''),
                                                                       self.distribution.get_version())
        if not os.path.exists(self.build_temp):
             os.makedirs(self.build_temp)
        print("CMake args={}".format(cmake_args))
        # Call cmake
        subprocess.check_call(['cmake', ext.sourcedir] + cmake_args, cwd=self.build_temp, env=env)
        subprocess.check_call(['cmake', '--build', '.', '--target', ext.name] + build_args, cwd=self.build_temp)


setup(
    name='pycarl',
    version=obtain_version(),
    author="H. Bruintjes",
    author_email="h.bruintjes@cs.rwth-aachen.de",
    maintainer="S. Junges",
    maintainer_email="sebastian.junges@cs.rwth-aachen.de",
    url="http://moves.rwth-aachen.de",
    description="pycarl - Python Bindings for Carl",
    packages=['pycarl', 'pycarl.cln', 'pycarl.gmp',
              'pycarl.cln.formula', 'pycarl.gmp.formula', 'pycarl.gmp.parse'],
    package_dir={'':'lib'},
    long_description='',
    ext_package='pycarl',
    ext_modules=[CMakeExtension('core', subdir=''),
                 CMakeExtension('cln', subdir='cln'),
                 CMakeExtension('gmp', subdir='gmp'),
                 CMakeExtension('formula', subdir='formula'),
                 CMakeExtension('formula-cln', subdir='cln/formula'),
                 CMakeExtension('formula-gmp', subdir='gmp/formula'),
                 CMakeExtension('parse-gmp', subdir='gmp/parse'),
                 CMakeExtension('parse-cln', subdir='cln/parse')],

cmdclass=dict(build_ext=CMakeBuild),
    zip_safe=False,
    install_requires=['pytest', 'grako'],
)

