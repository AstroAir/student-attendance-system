# Benchmarks

## CMake

```bash
cmake --preset ninja-debug -DCPP_QUICK_STARTER_BUILD_BENCHMARKS=ON
cmake --build --preset ninja-debug
./build/ninja-debug/benchmarks/benchmarks
```

## xmake

```bash
xmake f --build_benchmarks=y
xmake
xmake run benchmarks
```
