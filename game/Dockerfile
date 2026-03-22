FROM alpine:edge as build
RUN apk add gcc g++ libc-dev zlib-dev make openssl-dev
RUN apk add cmake wget tar linux-headers curl-dev

RUN mkdir build

COPY Makefile /build/Makefile
ADD uWebSockets /build/uWebSockets
ADD includes /build/includes
ADD src /build/src

RUN cd /build; ls -la .; make -j4

FROM alpine:edge as prod
RUN apk add libstdc++ libgcc openssl-dev
COPY --from=build /build/GameServer .

ENTRYPOINT ["./GameServer"]
