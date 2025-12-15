# CMake

本项目使用现代 CMake 组织代码：

- `cpp_quick_starter`：库
- `cpp_quick_starter_app`：示例可执行程序

## 常用命令

```bash
cmake --preset ninja-debug
cmake --build --preset ninja-debug
ctest --preset ninja-debug
```

## 常用开关

- `CPP_QUICK_STARTER_BUILD_TESTS`：构建测试
- `CPP_QUICK_STARTER_BUILD_BENCHMARKS`：构建基准
- `CPP_QUICK_STARTER_BUILD_DOCS`：生成 Doxygen 文档
