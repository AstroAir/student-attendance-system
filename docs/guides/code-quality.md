# Code Quality

## clang-format

```bash
clang-format -i src/**/*.cpp include/**/*.hpp tests/**/*.cpp examples/**/*.cpp
```

## clang-tidy

确保生成 `compile_commands.json`：

```bash
cmake --preset ninja-debug
clang-tidy -p build/ninja-debug src/main.cpp
```
