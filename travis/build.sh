#!/bin/bash -x

: ${N_JOBS:=2}
: ${TIMEOUT:=2000}

if [ "$STL" != "" ]
then
  STLARG="-stdlib=$STL"
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
    docker exec pycarl mkdir pycarl
    docker cp . pycarl:/pycarl
    # Install virtualenv
    docker exec pycarl apt-get install -qq -y python python3 virtualenv
    set +e

    # Execute main process
    docker exec pycarl bash -c "
        export COMPILER=$COMPILER;
        export N_JOBS=$N_JOBS;
        export STLARG=$STLARG;
        export OS=$OS;
        export PYTHON=$PYTHON;
        cd pycarl;
        travis/build-helper.sh $1"
    exit $?
    ;;

osx)
    # Mac OSX
    export COMPILER
    export N_JOBS
    export STLARG
    export OS
    export PYTHON
    travis/build-helper.sh "$1"
    exit $?
    ;;

*)
    # Other OS
    echo "Unsupported OS: $OS"
    exit 1
esac
