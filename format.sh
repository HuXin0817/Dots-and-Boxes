#!/bin/bash

find src -iname "*.cpp" -o -iname "*.h" | parallel clang-format -i
find frontend -iname "*.cpp" -o -iname "*.h" | parallel clang-format -i
clang-format -i main.cpp