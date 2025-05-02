ARG VERSION_UBUNTU=24.04
FROM docker.io/ubuntu:${VERSION_UBUNTU}
ENV LANG=C.utf8
RUN apt-get update && apt-get install -y gcc-arm-none-eabi libnewlib-arm-none-eabi
