
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
    s += "notifications:\n"
    s += "  email:\n"
    s += "    on_failure: always\n"
    s += "    on_success: change\n"
    s += "    recipients:\n"
    s += '    - secure: "QEKHoyya6CiCt2C8d2TupExTUy24tb8JxYUGVgBm2vqcWaEMuOpVbGeSFUJ9DVsg7VGDcL7rTFGjAJ+AlQLpRP+QKdHidECqbBn17P6koIuSrGuE9Q9VX9GEDiutYjWN8ex2Vx/tgzW7eVV71YdkWfwbnjbXTw4Uf4KqF91yoMLvubAJCaRqjrmnlyd4TWZmi+jxVHaev6OjJ82j1lCKWqNd11PpQMVWMyKE0AqX87P6fcnm5lFgV/8sK++pOR+j4eJpKeZfaUBa7PxFVIKEzEwjU3B9CKy59Mv5+omLm+56zoiLFy+2kBzdJZN96c/V4gcbO3qwItRixromj8mi0PcldAT2pvEmg50Z6/HHhVIRBiiqJo2CvAwUfp/UCRccx03GJPvzLol/DomArvVWiXHhf5HeNcgMTJ0eeeBZh35KITcltaZpR4+Zdxv+sy/M1AH3Ia0NFPcDZ8+XcR1dZUvNWs14J57gVkzocxDyXVL5CmL07qeyj9eUKLyvWer08TbY1XPpYQBxq9EuV4ZplhEYsHg0c6bMAc7PQIMWmaiVNCuzFb/ws/eTF7MUmbYoMHU3Veva/pf0HQESyoMucA+zXDk5MWJ6VRnYtVRMRdLtsfod9dSfgMNUi+TXNb3ZxjG1KNa9SnvBK+9RJ3hTnpIm9vN7lw84lxv1joElmUw="\n'
    s += '    - secure: "V+e+fWWLx/SmFeseTdMvtdu3l/jFUH1SuOXYQXxyXeHcyofp/plxT5k+3ZWuCm2w3o9J9zSXo8qSFlcUR0KkSOi6moj5ki8mPjiKPbVQEnrXprZ7eVIPf1n9XF2LEON0BIlrh+PbDK5nRtPiW80nbg5UXFO+SnQr5YQgS49VPMqRIP7waIxG5Ha5AEjLXJocbPi0kFD1YBMSaL5mZ3EiGn4edXwdGjDvDhT2mw2gUlD3DQiT8MCpL6Tymt9sMK2fekhcyq7Vx4j1sDzz4X2KQZ0VpYfX+LgqtzmmJl9sd4is90tk5xV2hoxpoEePAUOg3409As0ZwadSgIrU6jLRB8TBZxZB9dcVwp++ZvjmFaHRkBr+p1rQKiiDU+K4SVWrzOtYKsKnhA8bu6TxBxfV7RQ9VPgei7JNJwjhDasWqyoASc6F8/6N03XnDsumZDiGoW6VJh9Ll3gszwqGb/+N1q9zAt1DrQSgI5BYkTjKqqavfxbxzL6ZCYl8ifu6259w45zWVygfhntT7Oe+TB9Sk3Yf9bT+XOIYPkqREFUUEi+DJtvokfJZzL1dVDrbod0vPa4rnmUdKIA2sNjlompb9d3toJlVTprJiDD2fV5DNWk4XYLxG8QvDSUUQY16BfRx2RVbG68kZzXCASdws8JeArI/FZwxyUyohv+9V4namuY="\n'
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
                buildConfig += "      env: TASK=Test LINUX={} PYTHON={} CONFIG={} COMPILER={}\n".format(linux, python, build, compiler)
                buildConfig += "      install: travis/install_linux.sh\n"
                buildConfig += "      script: travis/build.sh\n"
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
                buildConfig += "      env: TASK=Test PYTHON={} CONFIG={} COMPILER={} STL=libc++\n".format(python, build, compiler)
                buildConfig += "      install: travis/install_osx.sh\n"
                buildConfig += "      script: travis/build.sh\n"
            s += buildConfig

    # Documentation
    config = configs_mac[0]
    osx = config[0]
    compiler = "{}{}".format(config[1], config[2])
    s += "    # Documentation\n".format()
    python = python_versions[0]
    buildConfig = ""
    build = build_types[0]
    buildConfig += "    - os: osx\n"
    buildConfig += "      compiler: {}\n".format(config[1])
    buildConfig += "      env: TASK=Documentation PYTHON={} CONFIG={} COMPILER={} STL=libc++\n".format(python, build, compiler)
    buildConfig += "      install: travis/install_osx.sh\n"
    buildConfig += "      script: travis/build.sh\n"
    buildConfig += "      deploy:\n"
    buildConfig += "        provider: pages\n"
    buildConfig += "        skip_cleanup: true\n"
    buildConfig += "        github_token: $GITHUB_TOKEN\n"
    buildConfig += "        local_dir: doc/build/html/\n"
    buildConfig += "        on:\n"
    buildConfig += "          branch: master\n"
    s += buildConfig

    print(s)
