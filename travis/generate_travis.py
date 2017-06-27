
# Configuration for Linux
configs_linux = [
    # OS, compiler
    ("ubuntu-16.10", "gcc", "-6"),
    ("debian-9", "gcc", "-6"),
]

# Configurations for Mac
configs_mac = [
    # OS, compiler
    ("osx", "clang", "-4.0"),
]

# Build types
build_types = [
#    "DefaultDebug",
    "DefaultRelease",
]

python_versions = [
    "python3",
]


if __name__ == "__main__":
    s = ""
    # Initial config
    s += "#\n"
    s += "# General config\n"
    s += "#\n"
    s += "branches:\n"
    s += "  only:\n"
    s += "  - master\n"
    s += "dist: trusty\n"
    s += "language: generic\n"
    s += "\n"
    s += "# Enable docker support\n"
    s += "services:\n"
    s += "- docker\n"
    s += "sudo: required\n"
    s += "\n"
    s += "#\n"
    s += "# Configurations\n"
    s += "#\n"
    s += "jobs:\n"
    s += "  include:\n"

    # Generate all configurations
    # Linux via Docker
    for config in configs_linux:
        linux = config[0]
        compiler = "{}{}".format(config[1], config[2])
        s += "    # {}\n".format(linux)
        for python in python_versions:
            buildConfig = ""
            for build in build_types:
                buildConfig += "    - os: linux\n"
                buildConfig += "      compiler: {}\n".format(config[1])
                buildConfig += "      env: PYTHON={} BUILD={} COMPILER={} LINUX={}\n".format(python, build, compiler, linux)
                buildConfig += "      install: export OS=linux; export COMPILER='{}'; export LINUX='{}'; export PYTHON='{}';\n".format(compiler, linux, python)
                buildConfig += "        travis/install_linux.sh\n"
                buildConfig += "      script: export OS=linux; export COMPILER='{}'; export LINUX='{}'; export PYTHON='{}';\n".format(compiler, linux, python)
                buildConfig += "        travis/build.sh {}\n".format(build)
            s += buildConfig

    # Mac OS X
    for config in configs_mac:
        osx = config[0]
        compiler = "{}{}".format(config[1], config[2])
        s += "    # {}\n".format(osx)
        for python in python_versions:
            buildConfig = ""
            for build in build_types:
                buildConfig += "    - os: osx\n"
                buildConfig += "      compiler: {}\n".format(config[1])
                buildConfig += "      env: PYTHON={} BUILD={} COMPILER={} STL=libc++\n".format(python, build, compiler)
                buildConfig += "      install: export OS=osx; export COMPILER='{}'; export STL='libc++'; export PYTHON='{}';\n".format(compiler, python)
                buildConfig += "        travis/install_osx.sh\n"
                buildConfig += "      script: export OS=osx; export COMPILER='{}'; export STL='libc++'; export PYTHON='{}';\n".format(compiler, python)
                buildConfig += "        travis/build.sh {}\n".format(build)
            s += buildConfig

    print(s)
