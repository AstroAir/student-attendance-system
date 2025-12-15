# cpp-quick-starter

[![CI](https://github.com/AstroAir/cpp-quick-starter/actions/workflows/ci.yml/badge.svg)](https://github.com/AstroAir/cpp-quick-starter/actions/workflows/ci.yml)
[![License: MIT](https://img.shields.io/badge/License-MIT-yellow.svg)](https://opensource.org/licenses/MIT)
[![C++20](https://img.shields.io/badge/C%2B%2B-20-blue.svg)](https://isocpp.org/std/the-standard)

[English](README.md) | [中文](README_CN.md)

A modern C++20 project template with best practices for quick project bootstrapping.

## ✨ Features

- **Modern C++20** - Leverages latest C++ standard features
- **Multiple Build Systems** - CMake, xmake support
- **Package Managers** - vcpkg, Conan integration
- **Testing** - Google Test for unit and integration tests
- **Benchmarking** - Google Benchmark support
- **Code Quality** - clang-format, clang-tidy, cppcheck
- **CI/CD** - GitHub Actions workflows
- **Documentation** - Doxygen + MkDocs

## 📁 Project Structure

```text
cpp-quick-starter/
├── .github/                    # GitHub configurations
│   ├── ISSUE_TEMPLATE/         # Issue templates
│   └── workflows/              # CI/CD workflows
├── benchmarks/                 # Benchmark tests
├── cmake/                      # CMake modules
│   ├── CompilerWarnings.cmake
│   ├── Conan.cmake
│   ├── Doxygen.cmake
│   ├── Sanitizers.cmake
│   └── StaticAnalyzers.cmake
├── docs/                       # Documentation
│   ├── api/
│   └── guides/
├── examples/                   # Example code
├── include/                    # Public headers
│   └── project_name/
│       ├── core/
│       │   └── greeting.hpp
│       └── utils/
│           └── string_utils.hpp
├── scripts/                    # Utility scripts
├── src/                        # Source files
│   ├── core/
│   │   └── greeting.cpp
│   ├── utils/
│   │   └── string_utils.cpp
│   └── main.cpp
├── tests/                      # Test files
│   ├── integration/
│   └── unit/
├── .clang-format               # Code formatting rules
├── .clang-tidy                 # Static analysis rules
├── .clangd                     # Clangd configuration
├── CMakeLists.txt              # CMake build configuration
├── CMakePresets.json           # CMake presets
├── conanfile.txt               # Conan dependencies
├── vcpkg.json                  # vcpkg dependencies
└── xmake.lua                   # xmake build configuration
```

## 🚀 Quick Start

### Prerequisites

- C++20 compatible compiler (GCC 10+, Clang 10+, MSVC 2019+)
- CMake 3.21+ or xmake
- (Optional) vcpkg or Conan for package management

### Build with CMake

```bash
# Configure (using presets)
cmake --preset ninja-debug

# Build
cmake --build --preset ninja-debug

# Run tests
ctest --preset ninja-debug

# Or manually
mkdir build && cd build
cmake ..
cmake --build .
```

### Build with xmake

```bash
# Configure and build
xmake

# Run the application
xmake run cpp_quick_starter_app

# Run tests
xmake run unit_tests
```

### CMake Presets

| Preset | Description |
|--------|-------------|
| `ninja-debug` | Debug build with Ninja |
| `ninja-release` | Release build with Ninja |
| `ninja-ci` | CI build with warnings as errors |
| `vs-debug` | Visual Studio Debug |
| `vs-release` | Visual Studio Release |

### CMake Options

| Option | Default | Description |
|--------|---------|-------------|
| `CPP_QUICK_STARTER_ENABLE_WARNINGS` | ON | Enable compiler warnings |
| `CPP_QUICK_STARTER_WARNINGS_AS_ERRORS` | OFF | Treat warnings as errors |
| `CPP_QUICK_STARTER_ENABLE_SANITIZERS` | OFF | Enable sanitizers |
| `CPP_QUICK_STARTER_ENABLE_CLANG_TIDY` | OFF | Enable clang-tidy |
| `CPP_QUICK_STARTER_ENABLE_CPPCHECK` | OFF | Enable cppcheck |
| `CPP_QUICK_STARTER_BUILD_TESTS` | ON | Build tests |
| `CPP_QUICK_STARTER_BUILD_BENCHMARKS` | OFF | Build benchmarks |
| `CPP_QUICK_STARTER_BUILD_EXAMPLES` | ON | Build examples |
| `CPP_QUICK_STARTER_BUILD_DOCS` | OFF | Build documentation |

## 📦 Package Management

### vcpkg

```bash
# Install dependencies
vcpkg install

# Configure with vcpkg toolchain
cmake -B build -S . -DCMAKE_TOOLCHAIN_FILE=$VCPKG_ROOT/scripts/buildsystems/vcpkg.cmake
```

### Conan

```bash
# Install dependencies
conan install . --output-folder=build --build=missing

# Configure with Conan toolchain
cmake -B build -S . -DCMAKE_TOOLCHAIN_FILE=build/conan_toolchain.cmake
```

## 🧪 Testing

```bash
# Run all tests with CTest
ctest --test-dir build --output-on-failure

# Or run test executables directly
./build/tests/unit_tests
./build/tests/integration_tests
```

## 📊 Benchmarking

```bash
# Enable benchmarks
cmake -B build -S . -DCPP_QUICK_STARTER_BUILD_BENCHMARKS=ON
cmake --build build

# Run benchmarks
./build/benchmarks/benchmarks
```

## 🔍 Code Quality

### Format Code

```bash
# Using clang-format
find include src tests -name '*.cpp' -o -name '*.hpp' | xargs clang-format -i

# Or use the script
./scripts/format.sh
```

### Static Analysis

```bash
# Enable clang-tidy
cmake -B build -S . -DCPP_QUICK_STARTER_ENABLE_CLANG_TIDY=ON

# Enable cppcheck
cmake -B build -S . -DCPP_QUICK_STARTER_ENABLE_CPPCHECK=ON
```

## 📚 Documentation

```bash
# Build documentation
cmake -B build -S . -DCPP_QUICK_STARTER_BUILD_DOCS=ON
cmake --build build --target docs

# Serve MkDocs locally
mkdocs serve
```

## 🤝 Contributing

Contributions are welcome! Please read [CONTRIBUTING.md](CONTRIBUTING.md) for guidelines.

## 📄 License

This project is licensed under the MIT License - see the [LICENSE](LICENSE) file for details.

## 🙏 Acknowledgments

- [Google Test](https://github.com/google/googletest)
- [Google Benchmark](https://github.com/google/benchmark)
- [CMake](https://cmake.org/)
- [xmake](https://xmake.io/)
