#!/bin/bash

cd ../../../
docker rmi -f ereva/valgrind/grep
docker build . -t ereva/valgrind/grep -f materials/build/Valgrind/Dockerfile
docker run --rm ereva/valgrind/grep
