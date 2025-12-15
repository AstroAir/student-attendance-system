# cpp-quick-starter

[![CI](https://github.com/AstroAir/cpp-quick-starter/actions/workflows/ci.yml/badge.svg)](https://github.com/AstroAir/cpp-quick-starter/actions/workflows/ci.yml)
[![License: MIT](https://img.shields.io/badge/License-MIT-yellow.svg)](https://opensource.org/licenses/MIT)
[![C++20](https://img.shields.io/badge/C%2B%2B-20-blue.svg)](https://isocpp.org/std/the-standard)

[English](README.md) | [中文](README_CN.md)

现代化的 C++20 项目模板，集成最佳实践，助您快速启动新项目。

## ✨ 特性

- **现代 C++20** - 使用最新 C++ 标准特性
- **多构建系统** - 支持 CMake、xmake
- **包管理器** - 集成 vcpkg、Conan
- **测试框架** - Google Test 单元测试和集成测试
- **性能基准** - Google Benchmark 支持
- **代码质量** - clang-format、clang-tidy、cppcheck
- **CI/CD** - GitHub Actions 工作流
- **文档生成** - Doxygen + MkDocs

## 📁 项目结构

```text
cpp-quick-starter/
├── .github/                    # GitHub 配置
│   ├── ISSUE_TEMPLATE/         # Issue 模板
│   └── workflows/              # CI/CD 工作流
├── benchmarks/                 # 基准测试
├── cmake/                      # CMake 模块
│   ├── CompilerWarnings.cmake
│   ├── Conan.cmake
│   ├── Doxygen.cmake
│   ├── Sanitizers.cmake
│   └── StaticAnalyzers.cmake
├── docs/                       # 文档
│   ├── api/
│   └── guides/
├── examples/                   # 示例代码
├── include/                    # 公共头文件
│   └── project_name/
│       ├── core/
│       │   └── greeting.hpp
│       └── utils/
│           └── string_utils.hpp
├── scripts/                    # 工具脚本
├── src/                        # 源文件
│   ├── core/
│   │   └── greeting.cpp
│   ├── utils/
│   │   └── string_utils.cpp
│   └── main.cpp
├── tests/                      # 测试文件
│   ├── integration/
│   └── unit/
├── .clang-format               # 代码格式化规则
├── .clang-tidy                 # 静态分析规则
├── .clangd                     # Clangd 配置
├── CMakeLists.txt              # CMake 构建配置
├── CMakePresets.json           # CMake 预设
├── conanfile.txt               # Conan 依赖
├── vcpkg.json                  # vcpkg 依赖
└── xmake.lua                   # xmake 构建配置
```

## 🚀 快速开始

### 环境要求

- 支持 C++20 的编译器 (GCC 10+, Clang 10+, MSVC 2019+)
- CMake 3.21+ 或 xmake
- (可选) vcpkg 或 Conan 包管理器

### 使用 CMake 构建

```bash
# 配置（使用预设）
cmake --preset ninja-debug

# 构建
cmake --build --preset ninja-debug

# 运行测试
ctest --preset ninja-debug

# 或手动配置
mkdir build && cd build
cmake ..
cmake --build .
```

### 使用 xmake 构建

```bash
# 配置并构建
xmake

# 运行应用程序
xmake run cpp_quick_starter_app

# 运行测试
xmake run unit_tests
```

### CMake 预设

| 预设 | 描述 |
|------|------|
| `ninja-debug` | Ninja 调试构建 |
| `ninja-release` | Ninja 发布构建 |
| `ninja-ci` | CI 构建（警告视为错误） |
| `vs-debug` | Visual Studio 调试 |
| `vs-release` | Visual Studio 发布 |

### CMake 选项

| 选项 | 默认值 | 描述 |
|------|--------|------|
| `CPP_QUICK_STARTER_ENABLE_WARNINGS` | ON | 启用编译器警告 |
| `CPP_QUICK_STARTER_WARNINGS_AS_ERRORS` | OFF | 将警告视为错误 |
| `CPP_QUICK_STARTER_ENABLE_SANITIZERS` | OFF | 启用内存检测工具 |
| `CPP_QUICK_STARTER_ENABLE_CLANG_TIDY` | OFF | 启用 clang-tidy |
| `CPP_QUICK_STARTER_ENABLE_CPPCHECK` | OFF | 启用 cppcheck |
| `CPP_QUICK_STARTER_BUILD_TESTS` | ON | 构建测试 |
| `CPP_QUICK_STARTER_BUILD_BENCHMARKS` | OFF | 构建基准测试 |
| `CPP_QUICK_STARTER_BUILD_EXAMPLES` | ON | 构建示例 |
| `CPP_QUICK_STARTER_BUILD_DOCS` | OFF | 构建文档 |

## 📦 包管理

### vcpkg

```bash
# 安装依赖
vcpkg install

# 使用 vcpkg 工具链配置
cmake -B build -S . -DCMAKE_TOOLCHAIN_FILE=$VCPKG_ROOT/scripts/buildsystems/vcpkg.cmake
```

### Conan

```bash
# 安装依赖
conan install . --output-folder=build --build=missing

# 使用 Conan 工具链配置
cmake -B build -S . -DCMAKE_TOOLCHAIN_FILE=build/conan_toolchain.cmake
```

## 🧪 测试

```bash
# 使用 CTest 运行所有测试
ctest --test-dir build --output-on-failure

# 或直接运行测试可执行文件
./build/tests/unit_tests
./build/tests/integration_tests
```

## 📊 基准测试

```bash
# 启用基准测试
cmake -B build -S . -DCPP_QUICK_STARTER_BUILD_BENCHMARKS=ON
cmake --build build

# 运行基准测试
./build/benchmarks/benchmarks
```

## 🔍 代码质量

### 代码格式化

```bash
# 使用 clang-format
find include src tests -name '*.cpp' -o -name '*.hpp' | xargs clang-format -i

# 或使用脚本
./scripts/format.sh
```

### 静态分析

```bash
# 启用 clang-tidy
cmake -B build -S . -DCPP_QUICK_STARTER_ENABLE_CLANG_TIDY=ON

# 启用 cppcheck
cmake -B build -S . -DCPP_QUICK_STARTER_ENABLE_CPPCHECK=ON
```

## 📚 文档

```bash
# 构建文档
cmake -B build -S . -DCPP_QUICK_STARTER_BUILD_DOCS=ON
cmake --build build --target docs

# 本地预览 MkDocs
mkdocs serve
```

## 🤝 贡献

欢迎贡献！请阅读 [CONTRIBUTING.md](CONTRIBUTING.md) 了解贡献指南。

## 📄 许可证

本项目采用 MIT 许可证 - 详见 [LICENSE](LICENSE) 文件。

## 🙏 致谢

- [Google Test](https://github.com/google/googletest)
- [Google Benchmark](https://github.com/google/benchmark)
- [CMake](https://cmake.org/)
- [xmake](https://xmake.io/)
