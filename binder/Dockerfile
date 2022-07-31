FROM movesrwth/stormpy:1.7.0
MAINTAINER Matthias Volk <m.volk@utwente.nl>


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

WORKDIR ${HOME}
# Add missing path
ENV PATH="$HOME/.local/bin:$PATH"


##########
# Install dependencies
##########

RUN pip install --no-cache-dir notebook==6.4.12
RUN pip install --no-cache-dir numpy==1.23.1

##########
# Copy files for notebooks
##########

RUN mkdir notebooks
COPY doc/source/*.ipynb notebooks/
COPY doc/source/doc/ notebooks/doc/
