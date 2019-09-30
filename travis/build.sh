#!/bin/bash -x

# Helper for travis folding
travis_fold() {
  local action=$1
  local name=$2
  echo -en "travis_fold:${action}:${name}\r"
}

N_JOBS=2

OS=$TRAVIS_OS_NAME

case $OS in
linux)
    # Execute docker image on Linux
    # Stop previous session
    docker rm -f stormpy &>/dev/null
    # Run container
    set -e
    docker run -d -it --name stormpy --privileged movesrwth/$DOCKER
    # Copy local content into container
    docker exec stormpy mkdir /opt/stormpy
    docker cp . stormpy:/opt/stormpy

    travis_fold start install_dependencies
    docker exec stormpy apt-get update
    # Install dependencies for carl-parser
    docker exec stormpy apt-get install -qq -y maven uuid-dev
    # Install virtualenv
    docker exec stormpy apt-get install -qq -y python python3 virtualenv
    travis_fold end install_dependencies
    set +e

    # Execute main process
    docker exec stormpy bash -c "
        export N_JOBS=$N_JOBS;
        export OS=$OS;
        export STLARG=;
        export PYTHON=$PYTHON;
        export CONFIG=$CONFIG;
        export TASK=$TASK;
        cd /opt/stormpy;
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
