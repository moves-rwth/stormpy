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


##########
# Install dependencies
##########

# Jupyter notebook and Jupyterlab
RUN pip install --no-cache-dir notebook==6.4.12
RUN pip install --no-cache-dir jupyterlab==3.4.4
# Dependencies for stormpy
RUN pip install --no-cache-dir numpy==1.23.1

##########
# Prepare files for notebooks
##########

# Copy files
WORKDIR ${HOME}
COPY doc/source/*.ipynb notebooks/
COPY doc/source/doc/ notebooks/doc/
COPY examples/ examples/

# Set owner of files to user
USER root
RUN chown -R ${NB_UID} ${HOME}
USER ${NB_USER}

# Add missing path
ENV PATH="$HOME/.local/bin:$PATH"

# Unset entrypoint
ENTRYPOINT []
