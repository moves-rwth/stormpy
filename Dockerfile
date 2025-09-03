# Base Dockerfile for using stormpy
###################################
# The Docker image can be built by executing:
# docker build -t yourusername/stormpy .
# A different Storm base image can be set from the commandline with:
# --build-arg STORM_BASE=<new_base_image>

# Set Storm base image
ARG STORM_BASE=movesrwth/storm:stable
FROM $STORM_BASE
LABEL org.opencontainers.image.authors="dev@stormchecker.org"


# Configuration arguments
#########################
# The arguments can be set from the commandline with:
# --build-arg <arg_name>=<value>

# CMake build type
ARG build_type=Release
# Additional arguments for compiling stormpy
ARG setup_args=""
# Optional support to install for stormpy, such as '[test,doc]'
ARG options=""
# Number of threads to use for parallel compilation
ARG no_threads=2


# Install dependencies
######################
# Uncomment to update packages beforehand
RUN apt-get update -qq
RUN apt-get install -y --no-install-recommends \
    maven \
    uuid-dev \
    python3 \
    python3-dev \
    python3-venv
# Packages maven and uuid-dev are required for carl-parser

# Build carl-parser
###################
WORKDIR /opt/

# Obtain carl-parser from public repository
RUN git clone https://github.com/moves-rwth/carl-parser.git

# Switch to build directory
RUN mkdir -p /opt/carl-parser/build
WORKDIR /opt/carl-parser/build

# Configure carl-parser
# Set hint for carl directory to Storm directory
RUN cmake .. -DCMAKE_BUILD_TYPE=$build_type -DPORTABLE=ON -Dcarl_DIR=/opt/storm/build/_deps/carl-build

# Build carl-parser
RUN make carl-parser -j $no_threads


# Set-up virtual environment
############################
ENV VIRTUAL_ENV=/opt/venv
RUN python3 -m venv $VIRTUAL_ENV
ENV PATH="$VIRTUAL_ENV/bin:$PATH"


# Build stormpy
###############
RUN mkdir /opt/stormpy
WORKDIR /opt/stormpy

# Copy the content of the current local stormpy repository into the Docker image
COPY . .

# Build stormpy
RUN pip install -v \
    --config-settings=cmake.define.CMAKE_BUILD_PARALLEL_LEVEL=$no_threads \
    --config-settings=cmake.build-type=$build_type \
    --config-settings=cmake.define.CARLPARSER_DIR_HINT=/opt/carl-parser/build/ \
    $setup_args .$options
