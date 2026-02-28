FROM alpine:edge as build
RUN apk add g++ libc-dev zlib-dev make openssl-dev
RUN apk add cmake wget tar linux-headers curl-dev

RUN mkdir build

ADD uWebSockets /build/uWebSockets
ADD includes /build/includes
COPY Makefile /build/Makefile
ADD src /build/src

RUN set -x && \
    mkdir build && cd build && cmake -DCMAKE_BUILD_TYPE=Release -DENABLE_TESTING=OFF .. && \
    make -j4 && make install && \
    cd .. && rm -rf * && \
    set +x

RUN cd /build; ls -la .; make -j4 release

FROM alpine:edge as prod
RUN apk add libstdc++ libgcc openssl-dev
COPY --from=build /build/GameServer .

ENTRYPOINT ["./GameServer"]
