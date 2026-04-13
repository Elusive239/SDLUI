#!/bin/bash
g++ main.cpp \
    -I/usr/include/SDL3 -I/usr/local/include/SDL3 \
    -I/usr/include/SDL3_ttf -I/usr/local/include/SDL3_ttf \
    -I/usr/include/SDL3_image -I/usr/local/include/SDL3_image \
    -lSDL3 -lSDL3_ttf -lSDL3_image \
-D_REENTRANT -Wno-write-strings -w -ggdb
