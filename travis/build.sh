#!/bin/bash -x

N_JOBS=2

OS=$TRAVIS_OS_NAME

BUILD_CARL_PARSER=FALSE
if [[ "$CONFIG" == *Parser ]]
then
    BUILD_CARL_PARSER=TRUE
fi


case $OS in
linux)
    # Execute docker image on linux
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
    docker exec pycarl apt-get install -qq -y python python3 virtualenv
    # Install dependencies for carl-parser
    if [[ "$BUILD_CARL_PARSER" == TRUE ]]
    then
        docker exec pycarl apt-get install -qq -y maven
    fi
    set +e

    # Execute main process
    docker exec pycarl bash -c "
        export COMPILER=$COMPILER;
        export N_JOBS=$N_JOBS;
        export OS=$OS;
        export PYTHON=$PYTHON;
        export CONFIG=$CONFIG;
        export BUILD_CARL_PARSER=$BUILD_CARL_PARSER;
        export TASK=$TASK;
        export STLARG=;
        cd opt/pycarl;
        travis/build-helper.sh"
    exit $?
    ;;

osx)
    # Mac OSX
    STLARG="-stdlib=libc++"
    export N_JOBS
    export OS
    export BUILD_CARL_PARSER
    travis/build-helper.sh
    exit $?
    ;;

*)
    # Other OS
    echo "Unsupported OS: $OS"
    exit 1
esac
