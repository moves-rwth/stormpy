#!/bin/bash -x

N_JOBS=2

OS=$TRAVIS_OS_NAME

case $OS in
linux)
    # Execute docker image on Linux
    # Stop previous session
    docker rm -f stormpy &>/dev/null
    # Run container
    set -e
    docker run -d -it --name stormpy --privileged mvolk/storm:$DOCKER
    # Copy local content into container
    docker exec stormpy mkdir opt/stormpy
    docker cp . stormpy:/opt/stormpy
    # Install virtualenv
    docker exec stormpy apt-get install -qq -y python python3 virtualenv
    set +e

    # Execute main process
    docker exec stormpy bash -c "
        export N_JOBS=$N_JOBS;
        export OS=$OS;
        export PYTHON=$PYTHON;
        export TASK=$TASK;
        cd opt/stormpy;
        travis/build-helper.sh"
    exit $?
    ;;

osx)
    echo "MacOS currently unsupported"
    exit 1
    ;;

*)
    # Other OS
    echo "Unsupported OS: $OS"
    exit 1
esac
