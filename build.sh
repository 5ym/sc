#!/bin/sh

docker build -t sc-builder . && docker run --rm -v $(pwd):/tmp/pcsclite sc-builder