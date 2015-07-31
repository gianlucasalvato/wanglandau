#!/bin/bash

EXEC_NAME="wl"
BUILD_DIR="./build"

[[ ! -d "$BUILD_DIR" ]] && mkdir -p build;

cd "$BUILD_DIR"

cmake .. && make

if [ ! -f "$EXEC_NAME" ]; then
	echo "Failed with exit code: $?!";
	exit $?;
fi

mv {,../}"$EXEC_NAME"

exit $?