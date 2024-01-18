
FROM ubuntu:24.04

# FROM ubuntu:22.04

WORKDIR /app

RUN apt-get update --fix-missing && apt-get install -y git cmake gdb build-essential clang \
  clang-tidy clang-format gcc-doc pkg-config glibc-doc tcpdump

# GCC 12
RUN apt-get install -y gcc-12 g++-12
RUN update-alternatives --install /usr/bin/gcc gcc /usr/bin/gcc-12 100
RUN update-alternatives --install /usr/bin/g++ g++ /usr/bin/g++-12 100

# install clang 15
RUN apt-get install -y clang-15


# Set environment variables
ENV CC=gcc
ENV CXX=g++

# Clean up
RUN apt-get clean & rm -rf /var/lib/apt/lists/*

# CMD specifies the command to run on container start
CMD ["/bin/bash"]