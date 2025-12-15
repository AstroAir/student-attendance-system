# Testing

## CMake

```bash
cmake --preset ninja-debug
cmake --build --preset ninja-debug
ctest --preset ninja-debug
```

## xmake

```bash
xmake f --build_tests=y
xmake
xmake run unit_tests
xmake run integration_tests
```
