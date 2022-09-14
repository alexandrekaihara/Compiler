#!/bin/bash

apt update 
RUNLEVEL=1 apt install -y --no-install-recommends git gcc nasm ca-certificates libc6-dev python3 make
    