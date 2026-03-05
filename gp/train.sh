#!/usr/bin/env bash

while true;
do
    pkill genetic
    echo "==============================================="
    make -j$(( $(nproc) / 2 ))
    ./genetic &
    inotifywait -r src -e MODIFY
done
