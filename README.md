# bulk_average

A C++ library for computing the average of a very large number of `int64_t` values without overflow.

## How it works

The library maintains the running average using the invariant:

```
true_sum == mean * count + remainder
```

where `0 <= remainder < count`. This avoids storing the full sum (which would overflow `int64_t` for large inputs) by incrementally updating `mean` and `remainder` as each value is added.

## Usage

```cpp
#include "bulk_average.hpp"

bulk_average::BulkAverage avg;
avg += 3;
avg += 7;
avg += 11;

avg.mean();       // 7 (integer part of average)
avg.remainder();  // 0
avg.to_double();  // 7.0
```

## Build

```bash
nix develop              # enter dev environment
cmake -B build && cmake --build build
cmake --build build --target test
```

## Project structure

```
include/bulk_average.hpp   - public header
src/bulk_average.cpp       - implementation
tests/bulk_average_test.cpp - unit tests (Google Test)
nix/                       - Nix environment configs
```
