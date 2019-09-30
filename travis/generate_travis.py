
# Configuration for Linux
configs_linux = [
    # OS, compiler
    ("ubuntu-18.04", "gcc", ""),
    ("debian-9", "gcc", ""),
    ("debian-10", "gcc", ""),
    ("ubuntu-19.04", "gcc", ""),
]

# Configurations for Mac
configs_mac = [
    # OS, compiler
    ("osx", "clang", ""),
]

# Build types
build_types = [
    "DebugClnParser",
    "Release",
    "ReleaseCln",
    "ReleaseClnParser",
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
    s += "sudo: required\n"
    s += "dist: trusty\n"
    s += "language: generic\n"
    s += "\n"
    s += "# Enable docker support\n"
    s += "services:\n"
    s += "- docker\n"
    s += "\n"

    s += "notifications:\n"
    s += "  email:\n"
    s += "    on_failure: always\n"
    s += "    on_success: change\n"
    s += "    recipients:\n"
    s += '    - secure: "EQ9/ZrsTO2bBr3EReU8msl2XlV68ujQEdWVJdSFH5ZM7O1PxDbGARVVZspSCQcf4AdrZE2U8fMPPEgLp9pLIkMA4phm9wk+0USRzIO2xh+leN0t9Ka18cb4LhMtso5CYBU4OauQj/1D8jtu1tWEhSvW8wI2deVqPHvxdlLo4Cl7OiEBvyDj6jeRy/95fDWoMKYK/WrIWAdSs10kh1bVQecpRjdiD0OxHr8VWvP+6Ext1ZhuJuyHPP2Vui02FrSklA315MaVopVhapd3Oj5vExcmQjTkvO7GItcxwvgUHFinHp0aEhJ/FNkNGhzFnZcz8ijKa0MyrGVctYgZbOrycM1NE2uK/Pvaxc5hTFrAI3Jv+09lI3WD/iAtn59Uub18xJlm1z+4icwcHpFG8ch7jiJ5mIazPoivBoiFaIZLjLt0EB9KsPR1sPY3Y5d3oz8wN2w12gjbBBesIgqrjZvdf5+8WAzQrgV8tRlWidlucvr88bGC2IPO8t61ZKdVAZHSEeBejE5/VExq6RyGga6YUdm/sH7a8kmU2BXIxf3UIuQG03Atx7GMF4nv2XDtdWFkDoTeHQyLbu8mQfFFjbIKrGou3GGXm/I+cA99as9VLbvU+prs0ENBIZVddi8fojX2JqcI9n5f4Fe1ZrSaKBt72vtQ2PI38p8f84/qvT69fRp4="\n'
    s += '    - secure: "pSV8JF2LGd0CT6N3CYMrJE5bE2wjmCouKtEEH2NfIhLvkYR0tbB6TcGyKBANkbqXSp4Tvtja+hyRjeOJpabLHXNg9XPy2NkXSvNeGjwB2HVlrwC8EB4TxouP3jQrNRo1Rz+OrPkePhkS2OkMQ9gx8ZFfeYfpUttXGoqZdoIb3k9aUN7VjxQndp/VMO8BiRlmfEt/RmfKjcSJYFs8/+4flT45n63R+3JscuawPIgtg+R0Cl11PcG63Vfn+cBpi4FOlU5A3w7PEfoxQ1swlu3fcv4R174LB7cWh1HHL+Yi81kdxpDhAbjjrUSD+jLjTigkEV9duzHu+NN34g2uBzVZ5p9RW6vAC3YyG5wAy0CphaP92FYfHpOV41uHO0VXfkMvo8Xc+TKZRxsDkzjHzyfOp/RujPR/uvafKdYFqz9PvQSunpandKT+pjWdO8K60UCaE9H2wFEWZUULPGlOejMM2Ke/6V4l0wGrVSBZ3GcrGNOIqO97ML644/p78NB6/mz0AnWeeHOlyZPBRryqYNlZSs9hV2Vo8FLUjG5odE4ppBchUCyGDAnfmZ8kCsuONM+eye4l2jm+iiWPbXeVQ2Ffc2boEBcyMDw+LK9EDZUConJJUyuedwoRbmSvveF5rbEPmKEw5Vpzpst+OVH00cWnN+Ka6KdAGMmimqgzZ/YDtew="\n'
    s += "\n"

    s += "#\n"
    s += "# Dependencies\n"
    s += "#\n"
    s += "addons:\n"
    s += "  homebrew:\n"
    s += "    packages:\n"
    s += "    - coreutils\n"
    s += "    - cmake\n"
    s += "    - gmp\n"
    s += "    - cln\n"
    s += "    - ginac\n"
    s += "    - boost\n"
    s += "    - eigen\n"
    s += "    - python\n"
    s += "    - python3\n"
    s += "    - maven\n"
    s += "\n"

    s += "#\n"
    s += "# Configuration\n"
    s += "#\n"
    s += "jobs:\n"
    s += "  include:\n"

    # Generate all configurations
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
                buildConfig += "      script: travis/build.sh\n"
            s += buildConfig

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
                buildConfig += "      env: TASK=Test PYTHON={} CONFIG={} LINUX={} COMPILER={}\n".format(python, build, linux, compiler)
                buildConfig += "      script: travis/build.sh\n"
            s += buildConfig

    # Documentation
    config = configs_linux[-1]
    linux = config[0]
    compiler = "{}{}".format(config[1], config[2])
    s += "    # Documentation\n".format()
    python = python_versions[0]
    buildConfig = ""
    build = build_types[-1]
    buildConfig += "    - os: linux\n"
    buildConfig += "      compiler: {}\n".format(config[1])
    buildConfig += "      env: TASK=Documentation PYTHON={} CONFIG={} LINUX={} COMPILER={}\n".format(python, build, linux, compiler)
    buildConfig += "      script: travis/build.sh\n"
    buildConfig += "      before_deploy:\n"
    buildConfig += "        docker cp pycarl:/opt/pycarl/. .\n"
    buildConfig += "      deploy:\n"
    buildConfig += "        provider: pages\n"
    buildConfig += "        skip_cleanup: true\n"
    buildConfig += "        github_token: $GITHUB_TOKEN\n"
    buildConfig += "        local_dir: doc/build/html/\n"
    buildConfig += "        on:\n"
    buildConfig += "          branch: master\n"
    s += buildConfig

    print(s)
