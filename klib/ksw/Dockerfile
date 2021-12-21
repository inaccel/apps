# syntax=docker/dockerfile:1.2

FROM gcc AS sysroot
RUN curl -sS https://setup.inaccel.com/repository | sh \
 && apt-get update \
 && apt-get install --yes \
        coral-api \
 && rm --force --recursive /var/lib/apt/lists/*
WORKDIR /src/ksw
ADD inaccel.pc .
RUN git clone --depth 1 https://github.com/inaccel/klib \
 && gcc -D_KSW_MAIN klib/ksw.c -lcoral-api -lz \
 && rm --force --recursive klib

FROM inaccel/mkrt AS mkrt

ENV MKRT_SYSROOT_DIR=/host
ENV MKRT_CONFIG_PATH=/src

ENV MKRT_TOP_BUILD_DIR=/tmp

RUN --mount=from=sysroot,target=/host,ro mkrt

FROM scratch
COPY --from=mkrt /tmp /
ENTRYPOINT ["ksw/ld.so", "ksw/lib.so"]
