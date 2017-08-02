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
  # Create virtual environment
  virtualenv --python=$PYTHON stormpy-env
  source stormpy-env/bin/activate
  # Print version
  python --version

  # Build pycarl
  travis_fold start build_pycarl
  git clone https://github.com/moves-rwth/pycarl.git
  cd pycarl
  python setup.py build_ext -j 1 develop
  travis_fold end build_pycarl
  cd ..

  # Build stormpy
  travis_fold start build_stormpy
  python setup.py build_ext -j 1 develop
  travis_fold end build_stormpy

  # Perform task
  case $TASK in
  Test)
    # Run tests
    set +e
    python setup.py test
    ;;

  Documentation)
    # Generate documentation
    pip install sphinx
    cd doc
    make html
    touch build/html/.nojekyll
    rm -r build/html/_sources
    ;;

  *)
    echo "Unrecognized value of TASK: $TASK"
    exit 1
  esac
}


# This only exists in OS X, but it doesn't cause issues in Linux (the dir doesn't exist, so it's
# ignored).
export PATH="/usr/local/opt/coreutils/libexec/gnubin:$PATH"

run
