
FROM ubuntu:24.04

WORKDIR /app 

RUN apt update &&  apt install git cmake gdb build-essential clang \
  clang-tidy clang-format gcc-doc pkg-config glibc-doc tcpdump tshark

# Set the default C++ compiler alternatives
RUN update-alternatives --install/usr/bin/gcc gcc /usr/bin/gcc-12 100 \
  && update-alternatives --install /usr/bin/g++ g++ /usr/bin/g++-12 100 \
  & update-alternatives --install /usr/bin/clang clang/usr/bin/clang-15 100

# Set environment variables
ENV CC=gcc
ENV CXX=g++

# Clean up
RUN apt-get clean & rm -rf/var/lib/apt/lists/*

# CMD specifies the command to run on container start
CMD ["/bin/bash"]

