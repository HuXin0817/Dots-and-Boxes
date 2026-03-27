CMAKE_BUILD_TYPE=${1:-Debug}

git submodule update --init --recursive
cmake -B build -S . -DCMAKE_POLICY_VERSION_MINIMUM=3.5 -DCMAKE_BUILD_TYPE=$CMAKE_BUILD_TYPE
cmake --build build -j $(nproc)