import os
import sys
import subprocess
import datetime

from setuptools import setup, Extension, find_packages
from setuptools.command.build_ext import build_ext

import setup.helper as setup_helper
from setup.config import SetupConfig

if sys.version_info[0] == 2:
    sys.exit('Sorry, Python 2.x is not supported')

# Minimal storm version required
storm_min_version = "1.8.0"

# Get the long description from the README file
with open(os.path.join(os.path.abspath(os.path.dirname(__file__)), 'README.md'), encoding='utf-8') as f:
    long_description = f.read()


class CMakeExtension(Extension):
    def __init__(self, name, sourcedir=''):
        Extension.__init__(self, name, sources=[])
        self.sourcedir = os.path.abspath(sourcedir)


class CMakeBuild(build_ext):
    user_options = build_ext.user_options + [
        ('storm-dir=', None, 'Path to storm root (binary) location'),
        ('disable-dft', None, 'Disable support for DFTs'),
        ('disable-gspn', None, 'Disable support for GSPNs'),
        ('disable-pars', None, 'Disable support for parametric models'),
        ('disable-pomdp', None, 'Disable support for POMDP analysis'),
        ('debug', None, 'Build in Debug mode'),
        ('jobs=', 'j', 'Number of jobs to use for compiling'),
        ('pybind-version=', None, 'Pybind11 version to use'),
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
        print("Stormpy - Building into {}".format(self.build_temp))
        build_temp_version = self.build_temp + "-version"
        setup_helper.ensure_dir_exists(build_temp_version)

        # Write config
        setup_helper.ensure_dir_exists(self.build_temp)
        self.config.write_config(os.path.join(self.build_temp, "build_config.cfg"))

        cmake_args = []
        storm_dir = os.path.expanduser(self.config.get_as_string("storm_dir"))
        if storm_dir:
            cmake_args += ['-Dstorm_DIR=' + storm_dir]
        _ = subprocess.check_output(['cmake', os.path.abspath("cmake")] + cmake_args, cwd=build_temp_version)
        cmake_conf = setup_helper.load_cmake_config(os.path.join(build_temp_version, 'generated/config.py'))

        # Set storm directory
        if storm_dir == "":
            storm_dir = cmake_conf.STORM_DIR
        if storm_dir != cmake_conf.STORM_DIR:
            print("Stormpy - WARNING: Using different storm directory {} instead of given {}!".format(
                cmake_conf.STORM_DIR,
                storm_dir))
            storm_dir = cmake_conf.STORM_DIR

        # Check version
        from packaging.version import Version # Need to import here because otherwise packaging cannot be automatically installed as required dependency
        storm_version, storm_commit = setup_helper.parse_storm_version(cmake_conf.STORM_VERSION)
        if Version(storm_version) < Version(storm_min_version):
            print('Stormpy - Error: Storm version {} from \'{}\' is not supported anymore!'.format(storm_version, storm_dir))
            print("                 For more information, see https://moves-rwth.github.io/stormpy/installation.html#compatibility-of-stormpy-and-storm")
            sys.exit(42)  # Custom exit code which can be used for incompatible checks

        # Check additional support
        use_dft = cmake_conf.HAVE_STORM_DFT and not self.config.get_as_bool("disable_dft")
        use_gspn = cmake_conf.HAVE_STORM_GSPN and not self.config.get_as_bool("disable_gspn")
        use_pars = cmake_conf.HAVE_STORM_PARS and not self.config.get_as_bool("disable_pars")
        use_pomdp = cmake_conf.HAVE_STORM_POMDP and not self.config.get_as_bool("disable_pomdp")

        # Set pybind version
        from pycarl._config import PYBIND_VERSION as pycarl_pybind_version
        pybind_version = self.config.get_as_string("pybind_version")
        if pybind_version == "":
            pybind_version = pycarl_pybind_version
        elif Version(pybind_version) != Version(pycarl_pybind_version):
            print("Stormpy - WARNING: Given pybind11 version {} differs from pycarl pybind11 version {}!".format(pybind_version, pycarl_pybind_version))

        # Print build info
        print("Stormpy - Using storm {} from {}".format(storm_version, storm_dir))
        print("Stormpy - Using pybind11 version {}".format(pybind_version))
        if use_dft:
            print("Stormpy - Support for DFTs found and included.")
        else:
            print("Stormpy - WARNING: No support for DFTs!")
        if use_gspn:
            print("Stormpy - Support for GSPNs found and included.")
        else:
            print("Stormpy - WARNING: No support for GSPNs!")
        if use_pars:
            print("Stormpy - Support for parametric models found and included.")
        else:
            print("Stormpy - WARNING: No support for parametric models!")
        if use_pomdp:
            print("Stormpy - Support for POMDP analysis found and included.")
        else:
            print("Stormpy - WARNING: No support for POMDP analysis!")

        build_type = 'Debug' if self.config.get_as_bool("debug") else 'Release'
        # Set cmake build options
        cmake_args = ['-DCMAKE_LIBRARY_OUTPUT_DIRECTORY=' + self._extdir("core")]
        cmake_args += ['-DPYTHON_EXECUTABLE=' + sys.executable]
        cmake_args += ['-DCMAKE_BUILD_TYPE=' + build_type]
        cmake_args += ['-DPYBIND_VERSION=' + pybind_version]
        if storm_dir is not None:
            cmake_args += ['-Dstorm_DIR=' + storm_dir]
        cmake_args += ['-DUSE_STORM_DFT=' + ('ON' if use_dft else 'OFF')]
        cmake_args += ['-DUSE_STORM_GSPN=' + ('ON' if use_gspn else 'OFF')]
        cmake_args += ['-DUSE_STORM_PARS=' + ('ON' if use_pars else 'OFF')]
        cmake_args += ['-DUSE_STORM_POMDP=' + ('ON' if use_pomdp else 'OFF')]
        # Configure extensions
        env = os.environ.copy()
        env['CXXFLAGS'] = '{} -DVERSION_INFO=\\"{}\\"'.format(env.get('CXXFLAGS', ''), self.distribution.get_version())
        setup_helper.ensure_dir_exists(self.build_temp)
        print("Stormpy - CMake args={}".format(cmake_args))
        # Call cmake
        subprocess.check_call(['cmake', os.path.abspath("")] + cmake_args, cwd=self.build_temp, env=env)

        # Set build args
        build_args = ['--config', build_type]
        build_args += ['--', '-j{}'.format(self.config.get_as_int("jobs"))]

        # Build extensions
        for ext in self.extensions:
            if ext.name == "dft" and not use_dft:
                print("Stormpy - Bindings for DFTs skipped")
                continue
            if ext.name == "gspn" and not use_gspn:
                print("Stormpy - Bindings for GSPNs skipped")
                continue
            if ext.name == "pars" and not use_pars:
                print("Stormpy - Bindings for parametric models skipped")
                continue
            if ext.name == "pomdp" and not use_pomdp:
                print("Stormpy - Bindings for POMDP analysis skipped")
                continue
            # Call make
            subprocess.check_call(['cmake', '--build', '.', '--target', ext.name] + build_args, cwd=self.build_temp)

    def initialize_options(self):
        build_ext.initialize_options(self)
        # Set default values for custom cmdline flags
        self.storm_dir = None
        self.disable_dft = None
        self.disable_gspn = None
        self.disable_pars = None
        self.disable_pomdp = None
        self.debug = None
        self.jobs = None
        self.pybind_version = None

    def finalize_options(self):
        build_ext.finalize_options(self)
        # Load setup config
        # This can only be done after the finalization step, because otherwise build_temp is not initialized yet.
        self.config.load_from_file(os.path.join(self.build_temp, "build_config.cfg"))
        # Update setup config
        self.config.update("storm_dir", self.storm_dir)
        self.config.update("disable_dft", self.disable_dft)
        self.config.update("disable_gspn", self.disable_gspn)
        self.config.update("disable_pars", self.disable_pars)
        self.config.update("disable_pomdp", self.disable_pomdp)
        self.config.update("debug", self.debug)
        self.config.update("jobs", self.jobs)
        self.config.update("pybind_version", self.pybind_version)


setup(
    name="stormpy",
    version=setup_helper.obtain_version(),
    author="M. Volk",
    author_email="matthias.volk@cs.rwth-aachen.de",
    maintainer="S. Junges",
    maintainer_email="sebastian.junges@cs.rwth-aachen.de",
    url="https://github.com/moves-rwth/stormpy/",
    description="stormpy - Python Bindings for Storm",
    long_description=long_description,
    long_description_content_type='text/markdown',
    project_urls={
        'Documentation': 'https://moves-rwth.github.io/stormpy/',
        'Source': 'https://github.com/moves-rwth/stormpy/',
        'Bug reports': 'https://github.com/moves-rwth/stormpy/issues',
    },
    classifiers=[
        'Intended Audience :: Science/Research',
        'Topic :: Scientific/Engineering',
        'Topic :: Software Development :: Libraries :: Python Modules',
    ],

    packages=find_packages('lib'),
    package_dir={'': 'lib'},
    include_package_data=True,
    package_data={'stormpy.examples': ['examples/files/*']},
    ext_package='stormpy',
    ext_modules=[CMakeExtension('core'),
                 CMakeExtension('info'),
                 CMakeExtension('logic'),
                 CMakeExtension('storage'),
                 CMakeExtension('utility'),
                 CMakeExtension('dft'),
                 CMakeExtension('gspn'),
                 CMakeExtension('pars'),
                 CMakeExtension('pomdp')
                 ],
    cmdclass={'build_ext': CMakeBuild},
    zip_safe=False,
    install_requires=['pycarl>=2.2.0'],
    setup_requires=['pycarl>=2.2.0', # required to check pybind version used for pycarl
                   'pytest-runner',
                   'packaging'
                   ],
    tests_require=['pytest', 'nbval', 'numpy'],
    extras_require={
        "numpy":  ["numpy"],
        "plot":  ["matplotlib","numpy","scipy"],
        "doc": ["Sphinx", "sphinx-bootstrap-theme", "nbsphinx", "ipython", "ipykernel"], # also requires pandoc to be installed
    },
    python_requires='>=3.7', # required by packaging
)
