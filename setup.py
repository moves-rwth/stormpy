import os
import sys
import subprocess
import datetime

from setuptools import setup, Extension, find_packages
from setuptools.command.build_ext import build_ext
from distutils.version import StrictVersion

import setup.helper as setup_helper
from setup.config import SetupConfig

if sys.version_info[0] == 2:
    sys.exit('Sorry, Python 2.x is not supported')

# Minimal carl version required
carl_min_version = "17.12"
carl_max_version = "18.08"
carl_master14_version = "14."

# Get the long description from the README file
with open(os.path.join(os.path.abspath(os.path.dirname(__file__)), 'README.md'), encoding='utf-8') as f:
    long_description = f.read()


class CMakeExtension(Extension):
    def __init__(self, name, sourcedir='', subdir=''):
        Extension.__init__(self, name, sources=[])
        self.sourcedir = os.path.abspath(sourcedir)
        self.subdir = subdir


class CMakeBuild(build_ext):
    user_options = build_ext.user_options + [
        ('carl-dir=', None, 'Path to carl root (binary) location'),
        ('carl-parser-dir=', None, 'Path to carl-parser root (binary) location'),
        ('disable-cln', None, 'Disable support for CLN'),
        ('disable-parser', None, 'Disable parsing support'),
        ('debug', None, 'Build in Debug mode'),
        ('jobs=', 'j', 'Number of jobs to use for compiling'),
    ]

    config = SetupConfig()

    def _extdir(self, extname):
        return os.path.abspath(os.path.dirname(self.get_ext_fullpath(extname)))

    def run(self):
        try:
            _ = subprocess.check_output(['cmake', '--version'])
        except OSError:
            raise RuntimeError("CMake must be installed to build the following extensions: " +
                               ", ".join(e.name for e in self.extensions))

        # Build cmake version info
        build_temp_version = self.build_temp + "-version"
        setup_helper.ensure_dir_exists(build_temp_version)

        # Write config
        setup_helper.ensure_dir_exists("build")
        self.config.write_config("build/build_config.cfg")

        cmake_args = []
        carl_dir = os.path.expanduser(self.config.get_as_string("carl_dir"))
        carl_parser_dir = os.path.expanduser(self.config.get_as_string("carl_parser_dir"))
        if carl_dir:
            cmake_args += ['-Dcarl_DIR=' + carl_dir]
        if carl_parser_dir:
            cmake_args += ['-Dcarl_parser_DIR=' + carl_parser_dir]
        output = subprocess.check_output(['cmake', os.path.abspath("cmake")] + cmake_args, cwd=build_temp_version)
        cmake_conf = setup_helper.load_cmake_config(os.path.join(build_temp_version, 'generated/config.py'))

        # Set carl directory
        if carl_dir == "":
            carl_dir = cmake_conf.CARL_DIR
        if carl_dir != cmake_conf.CARL_DIR:
            print("Pycarl - Warning: Using different carl directory {} instead of given {}!".format(cmake_conf.CARL_DIR,
                                                                                                    carl_dir))
            carl_dir = cmake_conf.CARL_DIR
        # Set carl-parser directory
        if carl_parser_dir == "":
            carl_parser_dir = cmake_conf.CARL_PARSER_DIR
        if carl_parser_dir != cmake_conf.CARL_PARSER_DIR:
            print("Pycarl - Warning: Using different carl-parser directory {} instead of given {}!".format(
                cmake_conf.CARL_PARSER_DIR, carl_parser_dir))
            carl_parser_dir = cmake_conf.CARL_PARSER_DIR

        # Check version
        carl_version, carl_commit = setup_helper.parse_carl_version(cmake_conf.CARL_VERSION)
        if carl_version.startswith(carl_master14_version):
            print("Pycarl - Using carl with master14 branch.")
        elif StrictVersion(carl_version) < StrictVersion(carl_min_version):
            sys.exit("Pycarl - Error: carl version {} from '{}' is not supported anymore!".format(carl_version, carl_dir))
        elif StrictVersion(carl_version) > StrictVersion(carl_max_version):
            sys.exit("Pycarl - Error: carl version {} from '{}' is not supported!".format(carl_version, carl_dir))

        # Check additional support
        use_cln = cmake_conf.CARL_WITH_CLN and not self.config.get_as_bool("disable_cln")
        use_parser = cmake_conf.CARL_WITH_PARSER and not self.config.get_as_bool("disable_parser")

        # Print build info
        print("Pycarl - Using carl {} from {}".format(carl_version, carl_dir))
        if use_parser:
            print("Pycarl - carl parser extension from {} included.".format(carl_parser_dir))
        else:
            print("Pycarl - Warning: No parser support!")
        if use_cln:
            print("Pycarl - Support for CLN found and included.")
        else:
            print("Pycarl - Warning: No support for CLN!")

        # Set general cmake build options
        build_type = 'Debug' if self.config.get_as_bool("debug") else 'Release'
        cmake_args = ['-DPYTHON_EXECUTABLE=' + sys.executable]
        cmake_args += ['-DCMAKE_BUILD_TYPE=' + build_type]
        if carl_dir is not None:
            cmake_args += ['-Dcarl_DIR=' + carl_dir]
        if use_parser and carl_parser_dir:
            cmake_args += ['-Dcarl_parser_DIR=' + carl_parser_dir]
        build_args = ['--config', build_type]
        build_args += ['--', '-j{}'.format(self.config.get_as_int("jobs"))]

        # Build extensions
        for ext in self.extensions:
            setup_helper.ensure_dir_exists(os.path.join(self._extdir(ext.name), ext.subdir))
            if "core" in ext.name:
                with open(os.path.join(self._extdir(ext.name), ext.subdir, "_config.py"), "w") as f:
                    f.write("# Generated from setup.py at {}\n".format(datetime.datetime.now()))
                    f.write('CARL_VERSION = "{}"\n'.format(carl_version))
                    f.write("CARL_WITH_PARSER = {}\n".format(use_parser))
                    f.write("CARL_WITH_CLN = {}\n".format(use_cln))
            if "cln" in ext.name:
                with open(os.path.join(self._extdir(ext.name), ext.subdir, "_config.py"), "w") as f:
                    f.write("# Generated from setup.py at {}\n".format(datetime.datetime.now()))
                    f.write("CARL_WITH_CLN = {}\n".format(use_cln))
                if not use_cln:
                    print("Pycarl - CLN bindings skipped")
                    continue
            if "parse" in ext.name:
                with open(os.path.join(self._extdir(ext.name), ext.subdir, "_config.py"), "w") as f:
                    f.write("# Generated from setup.py at {}\n".format(datetime.datetime.now()))
                    f.write("CARL_WITH_PARSER = {}\n".format(use_parser))
                if not use_parser:
                    print("Pycarl - Parser bindings skipped")
                    continue
            self.build_extension(ext, cmake_args, build_args)

    def initialize_options(self):
        build_ext.initialize_options(self)
        # Load setup config
        self.config.load_from_file("build/build_config.cfg")
        # Set default values for custom cmdline flags
        self.carl_dir = None
        self.carl_parser_dir = None
        self.disable_cln = None
        self.disable_parser = None
        self.debug = None
        self.jobs = None

    def finalize_options(self):
        build_ext.finalize_options(self)
        # Update setup config
        self.config.update("carl_dir", self.carl_dir)
        self.config.update("carl_parser_dir", self.carl_parser_dir)
        self.config.update("disable_cln", self.disable_cln)
        self.config.update("disable_parser", self.disable_parser)
        self.config.update("debug", self.debug)
        self.config.update("jobs", self.jobs)

    def build_extension(self, ext, general_cmake_args, general_build_args):
        extdir = os.path.abspath(os.path.dirname(self.get_ext_fullpath(ext.name)))
        cmake_args = ['-DCMAKE_LIBRARY_OUTPUT_DIRECTORY=' + os.path.join(extdir, ext.subdir)] + general_cmake_args
        build_args = general_build_args

        env = os.environ.copy()
        env['CXXFLAGS'] = '{} -DVERSION_INFO=\\"{}\\"'.format(env.get('CXXFLAGS', ''),
                                                              self.distribution.get_version())
        setup_helper.ensure_dir_exists(self.build_temp)
        print("Pycarl - CMake args={}".format(cmake_args))
        # Call cmake
        subprocess.check_call(['cmake', ext.sourcedir] + cmake_args, cwd=self.build_temp, env=env)
        subprocess.check_call(['cmake', '--build', '.', '--target', ext.name] + build_args, cwd=self.build_temp)


setup(
    name='pycarl',
    version=setup_helper.obtain_version(),
    author="S. Junges",
    author_email="sebastian.junges@cs.rwth-aachen.de",
    maintainer="M. Volk",
    maintainer_email="matthias.volk@cs.rwth-aachen.de",
    url="https://github.com/moves-rwth/pycarl/",
    description="pycarl - Python Bindings for CArL",
    long_description=long_description,
    long_description_content_type='text/markdown',
    project_urls={
        'Documentation': 'https://moves-rwth.github.io/pycarl/',
        'Source': 'https://github.com/moves-rwth/pycarl/',
        'Bug reports': 'https://github.com/moves-rwth/pycarl/issues',
    },
    classifiers=[
        'Intended Audience :: Science/Research',
        'Topic :: Scientific/Engineering',
        'Topic :: Software Development :: Libraries :: Python Modules',
    ],

    packages=find_packages('lib'),
    package_dir={'': 'lib'},
    include_package_data=True,
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

    cmdclass={'build_ext': CMakeBuild},
    zip_safe=False,
    setup_requires=['pytest-runner'],
    tests_require=['pytest'],
    python_requires='>=3',
)
