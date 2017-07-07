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
  virtualenv --python=$PYTHON pycarl-env
  source pycarl-env/bin/activate
  # Print version
  python --version

  # Build Carl
  travis_fold start install_carl
  git clone https://github.com/smtrat/carl.git
  cd carl
  mkdir build
  cd build
  cmake .. "${CMAKE_ARGS[@]}" "-DUSE_CLN_NUMBERS=ON" "-DUSE_GINAC=ON"
  make lib_carl -j$N_JOBS
  cd ../..
  travis_fold end install_carl

  # Install carl-parser
  if [[ "$BUILD_CARL_PARSER" == TRUE ]]
  then
    travis_fold start install_carl_parser
    git clone https://github.com/smtrat/carl-parser.git
    cd carl-parser
    mkdir build
    cd build
    cmake .. "${CMAKE_ARGS[@]}"
    make -j$N_JOBS
    cd ../..
    travis_fold stop install_carl_parser
  fi

  # Build Pycarl
  travis_fold start build_pycarl
  python setup.py build_ext -j 1 develop
  travis_fold end build_pycarl

  # Perform task
  case $TASK in
  Test)
    # Run tests
    set +e
    python -m pytest -sv tests/
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

case $COMPILER in
gcc-4.8)
    export CC=gcc-4.8
    export CXX=g++-4.8
    ;;

gcc-4.9)
    export CC=gcc-4.9
    export CXX=g++-4.9
    ;;

gcc-5)
    export CC=gcc-5
    export CXX=g++-5
    ;;

gcc-6)
    export CC=gcc-6
    export CXX=g++-6
    ;;

gcc-default)
    export CC=gcc
    export CXX=g++
    ;;

clang-3.5)
    export CC=clang-3.5
    export CXX=clang++-3.5
    ;;

clang-3.6)
    export CC=clang-3.6
    export CXX=clang++-3.6
    ;;

clang-3.7)
    export CC=clang-3.7
    export CXX=clang++-3.7
    ;;

clang-3.8)
    export CC=clang-3.8
    export CXX=clang++-3.8
    ;;

clang-3.9)
    export CC=clang-3.9
    export CXX=clang++-3.9
    ;;

clang-4.0)
    case "$OS" in
    linux)
        export CC=clang-4.0
        export CXX=clang++-4.0
        ;;
    osx)
        export CC=/usr/local/opt/llvm/bin/clang-4.0
        export CXX=/usr/local/opt/llvm/bin/clang++
        ;;
    *) echo "Error: unexpected OS: $OS"; exit 1 ;;
    esac
    ;;

clang-default)
    export CC=clang
    export CXX=clang++
    ;;

*)
    echo "Unrecognized value of COMPILER: $COMPILER"
    exit 1
esac

# Build
echo CXX version: $($CXX --version)
echo C++ Standard library location: $(echo '#include <vector>' | $CXX -x c++ -E - | grep 'vector\"' | awk '{print $3}' | sed 's@/vector@@;s@\"@@g' | head -n 1)
echo Normalized C++ Standard library location: $(readlink -f $(echo '#include <vector>' | $CXX -x c++ -E - | grep 'vector\"' | awk '{print $3}' | sed 's@/vector@@;s@\"@@g' | head -n 1))

case "$CONFIG" in
DefaultDebug*)           CMAKE_ARGS=(-DCMAKE_BUILD_TYPE=Debug   -DCMAKE_CXX_FLAGS="$STLARG") ;;
DefaultRelease*)         CMAKE_ARGS=(-DCMAKE_BUILD_TYPE=Release -DCMAKE_CXX_FLAGS="$STLARG") ;;
*) echo "Unrecognized value of CONFIG: $CONFIG"; exit 1 ;;
esac

run
