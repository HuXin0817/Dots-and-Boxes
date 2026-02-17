#!/bin/bash

set -e

echo "Configuring Debug Build"
cmake -B build_debug -DCMAKE_BUILD_TYPE=Debug -DCMAKE_EXPORT_COMPILE_COMMANDS=ON

echo "Configuring Release Build"
cmake -B build_release -DCMAKE_BUILD_TYPE=Release

echo "Building Debug"
cmake --build build_debug -j "$(nproc)"

echo "Building Release"
cmake --build build_release -j "$(nproc)"

LINK_NAME="build"
SOURCE_DIR="build_debug"
COMPILE_DB_SRC="build/compile_commands.json"
COMPILE_DB_DST="./compile_commands.json"

if [ ! -d "$SOURCE_DIR" ]; then
    echo "Error: Source directory '$SOURCE_DIR' does not exist. CMake configuration might have failed."
    exit 1
fi

if [ -L "$LINK_NAME" ]; then
    echo "Removing existing symbolic link: $LINK_NAME"
    rm "$LINK_NAME"
elif [ -e "$LINK_NAME" ]; then
    echo "Error: '$LINK_NAME' exists but is not a symbolic link. Please remove it manually."
    exit 1
fi

echo "Creating symbolic link: $LINK_NAME -> $SOURCE_DIR"
ln -s "$SOURCE_DIR" "$LINK_NAME"

if [ -f "$COMPILE_DB_SRC" ]; then
    if [ -e "$COMPILE_DB_DST" ] || [ -L "$COMPILE_DB_DST" ]; then
        rm "$COMPILE_DB_DST"
    fi
    
    echo "Creating symbolic link: $COMPILE_DB_DST -> $COMPILE_DB_SRC"
    ln -s "$COMPILE_DB_SRC" "$COMPILE_DB_DST"
else
    echo "Warning: $COMPILE_DB_SRC not found. Skipping compile_commands.json link."
fi

echo "Setup Complete"