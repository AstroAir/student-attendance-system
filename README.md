# Student Attendance System

[![CI](https://github.com/AstroAir/student-attendance-system/actions/workflows/ci.yml/badge.svg)](https://github.com/AstroAir/student-attendance-system/actions/workflows/ci.yml)
[![License: MIT](https://img.shields.io/badge/License-MIT-yellow.svg)](https://opensource.org/licenses/MIT)
[![C++20](https://img.shields.io/badge/C%2B%2B-20-blue.svg)](https://isocpp.org/std/the-standard)

[English](README.md) | [中文](README_CN.md)

A modern C++20 student attendance management system with RESTful API, built on the Drogon framework.

## ✨ Features

- **RESTful API** - Complete CRUD operations for students and attendance records
- **Authentication** - Session-based user authentication
- **Reports** - Daily, summary, abnormal, and leave reports
- **Data Import/Export** - Support for JSON and CSV formats
- **SQLite Database** - Persistent data storage
- **Modern C++20** - Leverages latest C++ standard features
- **Multiple Build Systems** - CMake, xmake support
- **Testing** - Google Test for unit and API tests
- **Code Quality** - clang-format, clang-tidy, cppcheck
- **CI/CD** - GitHub Actions workflows
- **Documentation** - Doxygen + MkDocs

## 📁 Project Structure

```text
student-attendance-system/
├── .github/                    # GitHub configurations
│   ├── ISSUE_TEMPLATE/         # Issue templates
│   └── workflows/              # CI/CD workflows
├── cmake/                      # CMake modules
│   ├── CompilerWarnings.cmake
│   ├── Doxygen.cmake
│   ├── Sanitizers.cmake
│   └── StaticAnalyzers.cmake
├── docs/                       # Documentation
│   ├── api/                    # API documentation
│   │   ├── REST-API.md         # RESTful API reference
│   │   └── openapi.yaml        # OpenAPI specification
│   ├── guides/                 # User guides
│   └── server/                 # Server documentation
├── include/student_attendance/ # Public headers
│   ├── controllers/            # API controllers
│   ├── services/               # Business logic
│   ├── models/                 # Data models
│   ├── filters/                # Request filters
│   └── utils/                  # Utility classes
├── src/                        # Source files
│   ├── controllers/            # Controller implementations
│   ├── services/               # Service implementations
│   ├── models/                 # Model implementations
│   ├── filters/                # Filter implementations
│   ├── db/                     # Database management
│   └── server_main.cpp         # Server entry point
├── tests/                      # Test files
│   └── api/                    # API tests
├── db/                         # Database files
├── config.json                 # Server configuration
├── CMakeLists.txt              # CMake build configuration
├── CMakePresets.json           # CMake presets
├── vcpkg.json                  # vcpkg dependencies
└── xmake.lua                   # xmake build configuration
```

## 🚀 Quick Start

### Prerequisites

- C++20 compatible compiler (GCC 10+, Clang 10+, MSVC 2019+)
- CMake 3.21+ or xmake
- (Optional) vcpkg for package management

### Build with CMake

```bash
# Configure (using presets)
cmake --preset ninja-debug

# Build
cmake --build --preset ninja-debug

# Run the server
./build/student_attendance_server

# Or manually
mkdir build && cd build
cmake ..
cmake --build .
./student_attendance_server
```

### Build with xmake

```bash
# Configure and build with server enabled
xmake config --build_server=y
xmake build student_attendance_server

# Run the server
xmake run student_attendance_server
```

The server will start at `http://localhost:8080`.

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
| `STUDENT_ATTENDANCE_BUILD_SERVER` | ON | Build attendance server |
| `STUDENT_ATTENDANCE_BUILD_TESTS` | ON | Build tests |
| `STUDENT_ATTENDANCE_BUILD_BENCHMARKS` | OFF | Build benchmarks |
| `STUDENT_ATTENDANCE_ENABLE_WARNINGS` | ON | Enable compiler warnings |
| `STUDENT_ATTENDANCE_WARNINGS_AS_ERRORS` | OFF | Treat warnings as errors |
| `STUDENT_ATTENDANCE_ENABLE_SANITIZERS` | OFF | Enable sanitizers |
| `STUDENT_ATTENDANCE_ENABLE_CLANG_TIDY` | OFF | Enable clang-tidy |
| `STUDENT_ATTENDANCE_ENABLE_CPPCHECK` | OFF | Enable cppcheck |
| `STUDENT_ATTENDANCE_BUILD_DOCS` | OFF | Build documentation |

## 🌐 API Endpoints

### Authentication (3 endpoints)

| Method | Path | Description |
|--------|------|-------------|
| POST | `/api/v1/auth/login` | User login |
| POST | `/api/v1/auth/logout` | User logout |
| GET | `/api/v1/auth/me` | Get current user info |

### Students (5 endpoints)

| Method | Path | Description |
|--------|------|-------------|
| GET | `/api/v1/students` | Get student list |
| POST | `/api/v1/students` | Create student |
| GET | `/api/v1/students/{student_id}` | Get student by ID |
| PUT | `/api/v1/students/{student_id}` | Update student |
| DELETE | `/api/v1/students/{student_id}` | Delete student |

### Attendances (6 endpoints)

| Method | Path | Description |
|--------|------|-------------|
| GET | `/api/v1/attendances` | Get attendance list |
| POST | `/api/v1/attendances` | Create attendance record |
| POST | `/api/v1/attendances/batch` | Batch create attendances |
| GET | `/api/v1/attendances/{id}` | Get attendance by ID |
| PUT | `/api/v1/attendances/{id}` | Update attendance |
| DELETE | `/api/v1/attendances/{id}` | Delete attendance |

### Reports (5 endpoints)

| Method | Path | Description |
|--------|------|-------------|
| GET | `/api/v1/reports/details` | Attendance details report |
| GET | `/api/v1/reports/daily` | Daily attendance report |
| GET | `/api/v1/reports/summary` | Attendance summary report |
| GET | `/api/v1/reports/abnormal` | Abnormal attendance report |
| GET | `/api/v1/reports/leave` | Leave summary report |

### Data Import/Export (2 endpoints)

| Method | Path | Description |
|--------|------|-------------|
| GET | `/api/v1/data/export` | Export data (JSON/CSV) |
| POST | `/api/v1/data/import` | Import data |

### Classes (2 endpoints)

| Method | Path | Description |
|--------|------|-------------|
| GET | `/api/v1/classes` | Get class list |
| GET | `/api/v1/classes/{class_name}/students` | Get students in class |

## 🧪 Testing

```bash
# Run all tests with CTest
ctest --test-dir build --output-on-failure

# Or run test executables directly
./build/tests/api_tests
```

## 🔍 Code Quality

### Format Code

```bash
# Using clang-format
find include src tests -name '*.cpp' -o -name '*.hpp' -o -name '*.h' -o -name '*.cc' | xargs clang-format -i

# Or use the script
./scripts/format.sh
```

### Static Analysis

```bash
# Enable clang-tidy
cmake -B build -S . -DSTUDENT_ATTENDANCE_ENABLE_CLANG_TIDY=ON

# Enable cppcheck
cmake -B build -S . -DSTUDENT_ATTENDANCE_ENABLE_CPPCHECK=ON
```

## 📚 Documentation

```bash
# Build documentation
cmake -B build -S . -DSTUDENT_ATTENDANCE_BUILD_DOCS=ON
cmake --build build --target docs

# Serve MkDocs locally
mkdocs serve
```

For detailed API documentation, see [docs/api/REST-API.md](docs/api/REST-API.md).

## 🤝 Contributing

Contributions are welcome! Please read [CONTRIBUTING.md](CONTRIBUTING.md) for guidelines.

## 📄 License

This project is licensed under the MIT License - see the [LICENSE](LICENSE) file for details.

## 🙏 Acknowledgments

- [Drogon](https://github.com/drogonframework/drogon) - C++ HTTP framework
- [Google Test](https://github.com/google/googletest)
- [CMake](https://cmake.org/)
- [xmake](https://xmake.io/)
