# Base Dockerfile for using stormpy
###################################
# The Docker image can be built by executing:
# docker build -t yourusername/stormpy .

FROM movesrwth/storm:1.6.4
# Change to movesrwth/storm:ci-release for the latest build
# or your own container
MAINTAINER Matthias Volk <m.volk@utwente.nl>

# Specify number of threads to use for parallel compilation
# This number can be set from the commandline with:
# --build-arg no_threads=<value>
ARG no_threads=1


# Install dependencies
######################
# Uncomment to update packages beforehand
# RUN apt-get update -qq
RUN apt-get install -y --no-install-recommends \
    maven \
    uuid-dev \
    python3 \
    python3-pip
# Packages maven and uuid-dev are required for carl-parser


# Build carl-parser
###################
WORKDIR /opt/

# Obtain carl-parser from public repository
RUN git clone -b master14 https://github.com/ths-rwth/carl-parser.git

# Switch to build directory
RUN mkdir -p /opt/carl-parser/build
WORKDIR /opt/carl-parser/build

# Configure carl-parser
RUN cmake .. -DCMAKE_BUILD_TYPE=Release

# Build carl-parser
RUN make carl-parser -j $no_threads


# Build pycarl
##############
WORKDIR /opt/

# Obtain latest version of pycarl from public repository
RUN git clone https://github.com/moves-rwth/pycarl.git

# Switch to pycarl directory
WORKDIR /opt/pycarl

# Build pycarl
RUN python3 setup.py build_ext -j $no_threads develop



# Build stormpy
###############
RUN mkdir /opt/stormpy
WORKDIR /opt/stormpy

# Copy the content of the current local stormpy repository into the Docker image
COPY . .

# Build stormpy
RUN python3 setup.py build_ext -j $no_threads develop
