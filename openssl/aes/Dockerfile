# syntax=docker/dockerfile:1.2

FROM fedora AS sysroot

RUN curl -sS https://setup.inaccel.com/repository | sh \
 && dnf install --assumeyes \
        cmake3 \
        gcc-c++ \
        git \
        inaccel-openssl \
        libcurl-devel \
        openssl-devel \
        zlib-devel \
 && dnf clean all

RUN git clone --depth 1 --recurse-submodules https://github.com/aws/aws-sdk-cpp /tmp/aws-sdk-cpp  \
 && cd /tmp/aws-sdk-cpp \
 && cmake -DBUILD_ONLY="s3" -DCMAKE_BUILD_TYPE="Debug" -DENABLE_TESTING="OFF" \
 && make \
 && make install \
 && rm --force --recursive /tmp/aws-sdk-cpp

WORKDIR inaccel

COPY openssl.cpp .

RUN make \
        CXXFLAGS="-std=c++17 -O3 -Wall" \
        LDLIBS="-laws-cpp-sdk-core -laws-cpp-sdk-s3 -lcoral-api -lcrypto -linaccel-crypto" \
        openssl

RUN mkdir aes \
 && mv openssl aes/a.out \
 && echo $'Name: openssl/aes\n\
Version: 0\n\
Description: inaccel/apps\n\
URL: https://github.com/inaccel/apps' > aes/inaccel.pc

FROM inaccel/mkrt AS mkrt

ENV MKRT_SYSROOT_DIR=/host
ENV MKRT_CONFIG_PATH=inaccel

ENV MKRT_TOP_BUILD_DIR=/tmp

RUN --mount=from=sysroot,target=/host,ro LD_LIBRARY_PATH=/usr/local/lib64 mkrt

FROM fedora
COPY --from=mkrt /tmp /
ENTRYPOINT ["/aes/ld.so", "/aes/lib.so"]
