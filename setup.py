#!/usr/bin/env python
import os
import re
import sys
import platform
import subprocess

from setuptools import setup, Extension
from setuptools.command.build_ext import build_ext
from distutils.version import LooseVersion

if sys.version_info[0] == 2:
    sys.exit('Sorry, Python 2.x is not supported')

class CMakeExtension(Extension):
    def __init__(self, name, sourcedir='', subdir=''):
        Extension.__init__(self, name, sources=[])
        self.sourcedir = os.path.abspath(sourcedir)
        self.subdir = subdir
        
class CMakeBuild(build_ext):
    
    user_options = build_ext.user_options + [
                                             ('storm-dir', None, 'Path to storm root (binary) location')
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
        self.storm_dir = None
    
    def finalize_options(self):
        build_ext.finalize_options(self)
    

    def build_extension(self, ext):
        extdir = os.path.abspath(os.path.dirname(self.get_ext_fullpath(ext.name)))
        cmake_args = ['-DCMAKE_LIBRARY_OUTPUT_DIRECTORY=' + os.path.join(extdir, ext.subdir),
                      '-DPYTHON_EXECUTABLE=' + sys.executable]
            
        cfg = 'Release'# if self.debug else 'Release'
        build_args = ['--config', cfg]
                      
                      
        cmake_args += ['-DCMAKE_BUILD_TYPE=' + cfg]
        build_args += ['--', '-j2']
        if self.storm_dir:
            cmake_args += ['-Dstorm_DIR=' + self.storm_dir]
                      
        env = os.environ.copy()
        env['CXXFLAGS'] = '{} -DVERSION_INFO=\\"{}\\"'.format(env.get('CXXFLAGS', ''),
                                                                            self.distribution.get_version())
        if not os.path.exists(self.build_temp):
             os.makedirs(self.build_temp)
        subprocess.check_call(['cmake', ext.sourcedir] + cmake_args, cwd=self.build_temp, env=env)
        subprocess.check_call(['cmake', '--build', '.', '--target', ext.name] + build_args, cwd=self.build_temp)
        
setup(
      name="stormpy",
      version="0.9",
      author="M. Volk",
      author_email="matthias.volk@cs.rwth-aachen.de",
      maintainer="S. Junges",
      maintainer_email="sebastian.junges@cs.rwth-aachen.de",
      url="http://moves.rwth-aachen.de",
      description="stormpy - Python Bindings for Storm",
      packages=['stormpy', 'stormpy.info', 'stormpy.expressions', 'stormpy.logic', 'stormpy.storage'],
      package_dir={'':'lib'},
      ext_package='stormpy',
      ext_modules=[CMakeExtension('core', subdir=''), CMakeExtension('info', subdir='info'),CMakeExtension('expressions', subdir='expressions'), CMakeExtension('logic', subdir='logic'), CMakeExtension('storage', subdir='storage')],
      cmdclass=dict(build_ext=CMakeBuild),
      zip_safe=False
)
