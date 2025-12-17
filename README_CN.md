# 学生考勤系统

[![CI](https://github.com/AstroAir/student-attendance-system/actions/workflows/ci.yml/badge.svg)](https://github.com/AstroAir/student-attendance-system/actions/workflows/ci.yml)
[![License: MIT](https://img.shields.io/badge/License-MIT-yellow.svg)](https://opensource.org/licenses/MIT)
[![C++20](https://img.shields.io/badge/C%2B%2B-20-blue.svg)](https://isocpp.org/std/the-standard)

[English](README.md) | [中文](README_CN.md)

基于 Drogon 框架的现代化 C++20 学生考勤管理系统，提供完整的 RESTful API。

## ✨ 特性

- **RESTful API** - 完整的学生和考勤记录 CRUD 操作
- **用户认证** - 基于 Session 的用户认证
- **统计报表** - 日报、汇总、异常、请假报表
- **数据导入导出** - 支持 JSON 和 CSV 格式
- **SQLite 数据库** - 持久化数据存储
- **现代 C++20** - 使用最新 C++ 标准特性
- **多构建系统** - 支持 CMake、xmake
- **测试框架** - Google Test 单元测试和 API 测试
- **代码质量** - clang-format、clang-tidy、cppcheck
- **CI/CD** - GitHub Actions 工作流
- **文档生成** - Doxygen + MkDocs

## 📁 项目结构

```text
student-attendance-system/
├── .github/                    # GitHub 配置
│   ├── ISSUE_TEMPLATE/         # Issue 模板
│   └── workflows/              # CI/CD 工作流
├── cmake/                      # CMake 模块
│   ├── CompilerWarnings.cmake
│   ├── Doxygen.cmake
│   ├── Sanitizers.cmake
│   └── StaticAnalyzers.cmake
├── docs/                       # 文档
│   ├── api/                    # API 文档
│   │   ├── REST-API.md         # RESTful API 参考
│   │   └── openapi.yaml        # OpenAPI 规范
│   ├── guides/                 # 用户指南
│   └── server/                 # 服务器文档
├── include/student_attendance/ # 公共头文件
│   ├── controllers/            # API 控制器
│   ├── services/               # 业务逻辑
│   ├── models/                 # 数据模型
│   ├── filters/                # 请求过滤器
│   └── utils/                  # 工具类
├── src/                        # 源文件
│   ├── controllers/            # 控制器实现
│   ├── services/               # 服务层实现
│   ├── models/                 # 模型实现
│   ├── filters/                # 过滤器实现
│   ├── db/                     # 数据库管理
│   └── server_main.cpp         # 服务器入口
├── tests/                      # 测试文件
│   └── api/                    # API 测试
├── db/                         # 数据库文件
├── config.json                 # 服务器配置
├── CMakeLists.txt              # CMake 构建配置
├── CMakePresets.json           # CMake 预设
├── vcpkg.json                  # vcpkg 依赖
└── xmake.lua                   # xmake 构建配置
```

## 🚀 快速开始

### 环境要求

- 支持 C++20 的编译器 (GCC 10+, Clang 10+, MSVC 2019+)
- CMake 3.21+ 或 xmake
- (可选) vcpkg 包管理器

### 使用 CMake 构建

```bash
# 配置（使用预设）
cmake --preset ninja-debug

# 构建
cmake --build --preset ninja-debug

# 运行服务器
./build/student_attendance_server

# 或手动配置
mkdir build && cd build
cmake ..
cmake --build .
./student_attendance_server
```

### 使用 xmake 构建

```bash
# 配置并构建（启用服务器）
xmake config --build_server=y
xmake build student_attendance_server

# 运行服务器
xmake run student_attendance_server
```

服务器将在 `http://localhost:8080` 启动。

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
| `STUDENT_ATTENDANCE_BUILD_SERVER` | ON | 构建考勤服务器 |
| `STUDENT_ATTENDANCE_BUILD_TESTS` | ON | 构建测试 |
| `STUDENT_ATTENDANCE_BUILD_BENCHMARKS` | OFF | 构建基准测试 |
| `STUDENT_ATTENDANCE_ENABLE_WARNINGS` | ON | 启用编译器警告 |
| `STUDENT_ATTENDANCE_WARNINGS_AS_ERRORS` | OFF | 将警告视为错误 |
| `STUDENT_ATTENDANCE_ENABLE_SANITIZERS` | OFF | 启用内存检测工具 |
| `STUDENT_ATTENDANCE_ENABLE_CLANG_TIDY` | OFF | 启用 clang-tidy |
| `STUDENT_ATTENDANCE_ENABLE_CPPCHECK` | OFF | 启用 cppcheck |
| `STUDENT_ATTENDANCE_BUILD_DOCS` | OFF | 构建文档 |

## 🌐 API 端点

### 认证接口 (3个)

| 方法 | 路径 | 描述 |
|------|------|------|
| POST | `/api/v1/auth/login` | 用户登录 |
| POST | `/api/v1/auth/logout` | 用户退出 |
| GET | `/api/v1/auth/me` | 获取当前用户信息 |

### 学生管理 (5个)

| 方法 | 路径 | 描述 |
|------|------|------|
| GET | `/api/v1/students` | 获取学生列表 |
| POST | `/api/v1/students` | 新增学生 |
| GET | `/api/v1/students/{student_id}` | 获取单个学生 |
| PUT | `/api/v1/students/{student_id}` | 修改学生信息 |
| DELETE | `/api/v1/students/{student_id}` | 删除学生 |

### 考勤记录 (6个)

| 方法 | 路径 | 描述 |
|------|------|------|
| GET | `/api/v1/attendances` | 获取考勤列表 |
| POST | `/api/v1/attendances` | 新增考勤记录 |
| POST | `/api/v1/attendances/batch` | 批量新增考勤 |
| GET | `/api/v1/attendances/{id}` | 获取单条考勤 |
| PUT | `/api/v1/attendances/{id}` | 修改考勤记录 |
| DELETE | `/api/v1/attendances/{id}` | 删除考勤记录 |

### 统计报表 (5个)

| 方法 | 路径 | 描述 |
|------|------|------|
| GET | `/api/v1/reports/details` | 考勤明细表 |
| GET | `/api/v1/reports/daily` | 考勤日报表 |
| GET | `/api/v1/reports/summary` | 考勤汇总表 |
| GET | `/api/v1/reports/abnormal` | 考勤异常表 |
| GET | `/api/v1/reports/leave` | 请假汇总表 |

### 数据导入导出 (2个)

| 方法 | 路径 | 描述 |
|------|------|------|
| GET | `/api/v1/data/export` | 导出数据 (JSON/CSV) |
| POST | `/api/v1/data/import` | 导入数据 |

### 班级管理 (2个)

| 方法 | 路径 | 描述 |
|------|------|------|
| GET | `/api/v1/classes` | 获取班级列表 |
| GET | `/api/v1/classes/{class_name}/students` | 获取班级学生 |

## 🧪 测试

```bash
# 使用 CTest 运行所有测试
ctest --test-dir build --output-on-failure

# 或直接运行测试可执行文件
./build/tests/api_tests
```

## 🔍 代码质量

### 代码格式化

```bash
# 使用 clang-format
find include src tests -name '*.cpp' -o -name '*.hpp' -o -name '*.h' -o -name '*.cc' | xargs clang-format -i

# 或使用脚本
./scripts/format.sh
```

### 静态分析

```bash
# 启用 clang-tidy
cmake -B build -S . -DSTUDENT_ATTENDANCE_ENABLE_CLANG_TIDY=ON

# 启用 cppcheck
cmake -B build -S . -DSTUDENT_ATTENDANCE_ENABLE_CPPCHECK=ON
```

## 📚 文档

```bash
# 构建文档
cmake -B build -S . -DSTUDENT_ATTENDANCE_BUILD_DOCS=ON
cmake --build build --target docs

# 本地预览 MkDocs
mkdocs serve
```

详细的 API 文档请参阅 [docs/api/REST-API.md](docs/api/REST-API.md)。

## 🤝 贡献

欢迎贡献！请阅读 [CONTRIBUTING.md](CONTRIBUTING.md) 了解贡献指南。

## 📄 许可证

本项目采用 MIT 许可证 - 详见 [LICENSE](LICENSE) 文件。

## 🙏 致谢

- [Drogon](https://github.com/drogonframework/drogon) - C++ HTTP 框架
- [Google Test](https://github.com/google/googletest)
- [CMake](https://cmake.org/)
- [xmake](https://xmake.io/)
