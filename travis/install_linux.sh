#!/bin/bash

set -e

# Helper for installing
function install {
    sudo apt-get -qq install --force-yes $*
}

# Helper for travis folding
travis_fold() {
  local action=$1
  local name=$2
  echo -en "travis_fold:${action}:${name}\r"
}

# Update packages
travis_fold start apt_install_dependencies
sudo apt-get -qq update

# Install compiler
case "${COMPILER}" in
gcc-default)   ;;
*) echo "Compiler not supported: ${COMPILER}. See travis/install_linux.sh"; exit 1 ;;
esac

# Install dependencies
install cmake libboost1.48-all-dev libcln-dev libeigen3-dev libstdc++-4.9-dev
travis_fold end apt_install_dependencies
