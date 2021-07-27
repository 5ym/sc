FROM alpine

WORKDIR /tmp/pcsclite

RUN apk add --no-cache make g++ pcsc-lite-dev

ENTRYPOINT [ "make" ]