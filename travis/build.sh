#!/bin/bash -x

N_JOBS=2

OS=$TRAVIS_OS_NAME

# Use CLN?
BUILD_CARL_CLN=FALSE
if [[ "$CONFIG" == *Cln* ]]
then
    BUILD_CARL_CLN=TRUE
fi
# Use carlparser?
BUILD_CARL_PARSER=FALSE
if [[ "$CONFIG" == *Parser* ]]
then
    BUILD_CARL_PARSER=TRUE
fi


case $OS in
linux)
    # Execute docker image on Linux
    # Stop previous session
    docker rm -f pycarl &>/dev/null
    # Run container
    set -e
    docker run -d -it --name pycarl --privileged mvolk/storm-basesystem:$LINUX
    # Copy local content into container
    docker exec pycarl mkdir opt/pycarl
    docker cp . pycarl:/opt/pycarl
    # Install virtualenv
    docker exec pycarl apt-get update
    docker exec pycarl apt-get install -qq -y libeigen3-dev python python3 virtualenv
    # Install dependencies for carl-parser
    if [[ "$BUILD_CARL_PARSER" == TRUE ]]
    then
        docker exec pycarl apt-get install -qq -y openjdk-8-jdk maven uuid-dev pkg-config
    fi
    set +e

    # Execute main process
    docker exec pycarl bash -c "
        export TASK=$TASK;
        export PYTHON=$PYTHON;
        export COMPILER=$COMPILER;
        export N_JOBS=$N_JOBS;
        export STLARG=;
        export OS=$OS;
        export BUILD_CARL_CLN=$BUILD_CARL_CLN;
        export BUILD_CARL_PARSER=$BUILD_CARL_PARSER;
        cd opt/pycarl;
        travis/build-helper.sh"
    exit $?
    ;;

osx)
    # Mac OSX
    STLARG="-stdlib=libc++"
    export TASK=$TASK
    export PYTHON
    export COMPILER
    export N_JOBS
    export STLARG
    export OS
    export BUILD_CARL_CLN
    export BUILD_CARL_PARSER
    travis/build-helper.sh
    exit $?
    ;;

*)
    # Unknown OS
    echo "Unsupported OS: $OS"
    exit 1
esac
