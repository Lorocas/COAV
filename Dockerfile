# Image
FROM debian:stable-slim

SHELL ["/bin/bash", "-c"]

# Dependencies
RUN apt-get update && apt-get install -y --no-install-recommends \
    build-essential \
    man \
    wget \
    curl \
    git \
    make \
    gdb \
    vim \
    valgrind \
    zip \
    sudo \
    linux-perf \
    libopenmpi-dev \
    libgmp-dev \
    libmpc-dev \
    libmpfr-dev \
    && apt clean && rm -rf /var/lib/apt/lists/* /var/cache/apt/archives/*

# Copy GCC sources
COPY gcc-12.2.0.tar.gz /tmp/
# Extract sources
RUN cd /tmp && tar -xf gcc-12.2.0.tar.gz && mv gcc-12.2.0 /usr/local/src/gcc-12.2.0
# Compilation compiler
RUN mkdir -p /usr/local/src/gcc-12.2.0/MYBUILD && cd /usr/local/src/gcc-12.2.0/MYBUILD \
    && ../configure \
        --prefix=/opt/gcc-12.2.0 \
        --enable-languages=c,c++,fortran \
        --enable-plugin \
        --disable-bootstrap \
        --disable-multilib \
    && make -j"$(nproc)" \
    && make install

# Add GCC in PATH
ENV PATH="/workspace/gcc-12.2.0/bin:${PATH}"
ENV LD_LIBRARY_PATH="/workspace/gcc-12.2.0/lib:${LD_LIBRARY_PATH}"
# ENV OMPI_CC="/workspace/gcc-12.2.0/bin/gcc:${OMPI_CC}"

# Create default user
RUN useradd -m -s /bin/bash user
USER user
WORKDIR /workspace

CMD ["/bin/bash"]


# docker build -t gcc-from-source .
# docker run -it gcc-from-source bash
