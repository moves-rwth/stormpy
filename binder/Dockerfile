FROM movesrwth/stormpy:1.6.4
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

RUN pip install --no-cache-dir notebook==6.4.8
RUN pip install --no-cache-dir numpy==1.22.1

##########
# Copy files for notebooks
##########

RUN mkdir notebooks
COPY doc/source/*.ipynb notebooks/
COPY doc/source/doc/ notebooks/doc/
