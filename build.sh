#!/bin/bash

git submodule update --init --recursive
cmake -B build -S . -DCMAKE_BUILD_TYPE=${1:-Debug}
ln -sf build/compile_commands.json compile_commands.json
cmake --build build --parallel --target Dots_and_Boxes
