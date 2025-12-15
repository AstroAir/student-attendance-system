# Getting Started

## 依赖

- CMake >= 3.21（推荐）
- Ninja（可选）
- C++ 编译器：MSVC / clang / gcc

## 使用 CMake 构建

```bash
cmake --preset ninja-debug
cmake --build --preset ninja-debug
ctest --preset ninja-debug
```

## 使用 xmake 构建

```bash
xmake f -m debug
xmake
xmake run cpp_quick_starter_app
```
