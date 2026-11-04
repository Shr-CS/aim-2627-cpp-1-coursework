#!/usr/bin/env bash
# 本地边界自测：编译并运行 local_tests/boundary_tests.cpp。
# 构建产物放在 local_tests/build/（已被 .gitignore 忽略）。
set -euo pipefail
cd "$(dirname "$0")"
mkdir -p build
${CXX:-g++} -std=c++17 -O2 -Wall -Wextra -pedantic \
    boundary_tests.cpp -o build/boundary_tests
./build/boundary_tests
