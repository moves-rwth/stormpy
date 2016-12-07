import os
import re
import sys
import platform
import subprocess

from setuptools import setup, Extension
from setuptools.command.build_ext import build_ext
from distutils.version import LooseVersion


class CMakeExtension(Extension):
    def __init__(self, name, sourcedir='', subdir=''):
        Extension.__init__(self, name, sources=[])
        self.sourcedir = os.path.abspath(sourcedir)
        self.subdir = subdir


class CMakeBuild(build_ext):
       
    user_options = build_ext.user_options + [
                                             ('carl-dir=', None, 'Path to carl root (binary) location')
                                             ]

    def run(self):
        print("---run----")
        try:
            out = subprocess.check_output(['cmake', '--version'])
        except OSError:
            raise RuntimeError("CMake must be installed to build the following extensions: " +
                               ", ".join(e.name for e in self.extensions))
        
        for ext in self.extensions:
            self.build_extension(ext)


    def initialize_options(self):
        print("--iop---")
        build_ext.initialize_options(self)
        self.carl_dir = None
    
    def finalize_options(self):
        print('The custom option for install is ', self.carl_dir)
        build_ext.finalize_options(self)
    

    def build_extension(self, ext):
        extdir = os.path.abspath(os.path.dirname(self.get_ext_fullpath(ext.name)))
        cmake_args = ['-DCMAKE_LIBRARY_OUTPUT_DIRECTORY=' + os.path.join(extdir, ext.subdir),
                      '-DPYTHON_EXECUTABLE=' + sys.executable]
            
        cfg = 'Debug' if self.debug else 'Release'
        build_args = ['--config', cfg]
                      
                      
        cmake_args += ['-DCMAKE_BUILD_TYPE=' + cfg]
        build_args += ['--', '-j2']
        if self.carl_dir:
            cmake_args += ['-Dcarl_DIR=' + self.carl_dir]
                      
        env = os.environ.copy()
        env['CXXFLAGS'] = '{} -DVERSION_INFO=\\"{}\\"'.format(env.get('CXXFLAGS', ''),
                                                                            self.distribution.get_version())
        if not os.path.exists(self.build_temp):
             os.makedirs(self.build_temp)
             print("cmake args={}".format(cmake_args))
        subprocess.check_call(['cmake', ext.sourcedir] + cmake_args, cwd=self.build_temp, env=env)
             
        try:
            subprocess.check_call(['cmake', '--build', '.', '--target', ext.name] + build_args, cwd=self.build_temp)
        except subprocess.CalledProcessError as e:
            if e.output:
                raise RuntimeError("CMake build returned with an error: " + e.output)
            
            print(e.stdout)

setup(
    name='pycarl',
    version="1.1",
    author="H. Bruintjes",
    author_email="h.bruintjes@cs.rwth-aachen.de",
    maintainer="S. Junges",
    maintainer_email="sebastian.junges@cs.rwth-aachen.de",
    url="http://moves.rwth-aachen.de",
    description="pycarl - Python Bindings for Carl",
    packages=['pycarl', 'pycarl.formula', 'pycarl.parse'],
    package_dir={'':'lib'},
    long_description='',
    ext_package='pycarl',
    ext_modules=[CMakeExtension('core', subdir=''), CMakeExtension('formula', subdir='formula'), CMakeExtension('parse', subdir='parse')],
    cmdclass=dict(build_ext=CMakeBuild),
    zip_safe=False,
        install_requires=['grako'],
)

