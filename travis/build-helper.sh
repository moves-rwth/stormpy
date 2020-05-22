#!/bin/bash

set -e

# Helper for travis folding
travis_fold() {
  local action=$1
  local name=$2
  echo -en "travis_fold:${action}:${name}\r"
}

# Helper for building and testing
run() {
  # We start in /opt/stormpy
  cd ..

  # Build carl-parser
  travis_fold start build_carl_parser
  git clone --single-branch -b master14 https://github.com/ths-rwth/carl-parser
  cd carl-parser
  mkdir build
  cd build
  cmake .. "${CMAKE_ARGS[@]}"
  make carl-parser -j 1
  cd ../..
  travis_fold end build_carl_parser

  # Create virtual environment
  travis_fold start virtualenv
  virtualenv --python=$PYTHON venv
  source venv/bin/activate
  # Print version
  python --version
  travis_fold end virtualenv

  # Build pycarl
  travis_fold start build_pycarl
  git clone https://github.com/moves-rwth/pycarl.git
  cd pycarl
  case "$CONFIG" in
  Debug*)
    python setup.py build_ext --debug -j 1 develop
    ;;
  *)
    python setup.py build_ext -j 1 develop
    ;;
  esac
  cd ..
  travis_fold end build_pycarl

  # Build stormpy
  travis_fold start build_stormpy
  cd stormpy
  case "$CONFIG" in
  Debug*)
    python setup.py build_ext --storm-dir /opt/storm/build/ --debug -j 1 develop
    ;;
  *)
    python setup.py build_ext --storm-dir /opt/storm/build/ -j 1 develop
    ;;
  esac
  travis_fold end build_stormpy

  # Perform tasks
  if [[ "$TASK" == *Test* ]]
  then
    # Run tests
    python setup.py test
  fi

  if [[ "$TASK" == *Documentation* ]]
  then
    # Generate documentation
    pip install sphinx sphinx_bootstrap_theme
    cd doc
    make html
    touch build/html/.nojekyll
    rm -r build/html/_sources
  fi
}


# This only exists in OS X, but it doesn't cause issues in Linux (the dir doesn't exist, so it's
# ignored).
export PATH="/usr/local/opt/coreutils/libexec/gnubin:$PATH"

case "$CONFIG" in
Debug*)           CMAKE_ARGS=(-DCMAKE_BUILD_TYPE=Debug   -DCMAKE_CXX_FLAGS="$STLARG") ;;
Release*)         CMAKE_ARGS=(-DCMAKE_BUILD_TYPE=Release -DCMAKE_CXX_FLAGS="$STLARG") ;;
*) echo "Unrecognized value of CONFIG: $CONFIG"; exit 1 ;;
esac

run
