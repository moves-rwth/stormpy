#!/bin/bash -x

case $OS in
linux)

    # Linux
    # Execute main process
    export COMPILER=$COMPILER;
    export N_JOBS=$N_JOBS;
    export STLARG=$STLARG;
    export OS=$OS;
    export PYTHON=$PYTHON;
    travis/build-helper.sh "$1"
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
