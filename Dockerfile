FROM movesrwth/stormpy:1.6.0
MAINTAINER Matthias Volk <matthias.volk@cs.rwth-aachen.de>


##########
# Create user
##########

ARG NB_USER=jovyan
ARG NB_UID=1000
ENV USER ${NB_USER}
ENV NB_UID ${NB_UID}
ENV HOME /home/${NB_USER}

RUN adduser --disabled-password \
    --gecos "Default user" \
    --uid ${NB_UID} \
    ${NB_USER}

# Change the owner of the virtual environment
WORKDIR /opt
USER root
RUN chown -R ${NB_UID} venv
USER ${NB_USER}

WORKDIR ${HOME}
# Add missing path
ENV PATH="$HOME/.local/bin:$PATH"


##########
# Install dependencies
##########

RUN pip install --no-cache-dir notebook==5.7.9

##########
# Copy files for notebooks
##########

RUN mkdir doc
COPY doc/source/*.ipynb doc/
COPY doc/source/doc/*.ipynb doc/
