#!/bin/bash
set -euo pipefail

find src include tests \
    -type f \( -name "*.cpp" -o -name "*.hpp" -o -name "*.h" \) \
    -exec clang-format -i {} +

echo "clang-format done."
