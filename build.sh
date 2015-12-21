#!/bin/bash


BUILD_DIR="./build"

[[ ! -d "$BUILD_DIR" ]] && mkdir -p "$BUILD_DIR";

cd "$BUILD_DIR"

cmake .. && make

exit $?