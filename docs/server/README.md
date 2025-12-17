# 学生考勤系统 API 服务器

基于 [Drogon](https://github.com/drogonframework/drogon) 框架实现的学生考勤系统 RESTful API 服务器。

## 功能特性

- **学生管理**: 增删改查学生信息
- **考勤记录**: 单条/批量考勤记录管理
- **统计报表**: 考勤明细、日报、汇总、异常、请假报表
- **数据导入导出**: 支持 JSON/CSV 格式
- **班级管理**: 班级列表和班级学生查询

## 项目结构

```
include/student_attendance/    # 头文件
├── controllers/               # 控制器层 (API 路由)
│   ├── StudentController.h
│   ├── AttendanceController.h
│   ├── ReportController.h
│   ├── DataController.h
│   └── ClassController.h
├── services/                  # 服务层 (业务逻辑)
│   ├── StudentService.h
│   ├── AttendanceService.h
│   └── ReportService.h
├── models/                    # 数据模型层
│   ├── Student.h
│   ├── Attendance.h
│   └── DataStore.h
└── utils/                     # 工具类
    ├── JsonResponse.h
    └── AttendanceStatus.h

src/                           # 源文件
├── controllers/               # 控制器实现
├── services/                  # 服务层实现
├── models/                    # 模型实现
└── server_main.cpp            # 服务器入口

tests/api/                     # API 测试
├── student_api_test.cpp
├── attendance_api_test.cpp
├── report_api_test.cpp
├── data_api_test.cpp
└── class_api_test.cpp
```

## 依赖项

- C++20 编译器 (GCC 10+, Clang 10+, MSVC 2019+)
- CMake 3.21+
- Drogon 框架 (自动通过 FetchContent 获取)

## 构建方式

### 方式一：CMake + FetchContent (推荐)

自动下载 Drogon 依赖：

```bash
cmake -B build -S .
cmake --build build
```

### 方式二：CMake + vcpkg

```bash
# 安装依赖
vcpkg install drogon jsoncpp gtest benchmark

# 构建
cmake -B build -S . -DCMAKE_TOOLCHAIN_FILE=[vcpkg root]/scripts/buildsystems/vcpkg.cmake
cmake --build build
```

### 方式三：xmake

```bash
xmake config --build_server=y
xmake build student_attendance_server
```

## 运行

```bash
cd build
./student_attendance_server
```

服务器默认监听 `http://localhost:8080`

## API 端点

### 认证 (3个)

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
| GET | `/api/v1/data/export` | 导出数据 |
| POST | `/api/v1/data/import` | 导入数据 |

### 班级管理 (2个)

| 方法 | 路径 | 描述 |
|------|------|------|
| GET | `/api/v1/classes` | 获取班级列表 |
| GET | `/api/v1/classes/{class_name}/students` | 获取班级学生 |

## 考勤状态

| 符号 | 状态 | 标识 |
|------|------|------|
| √ | 出勤 | `present` |
| X | 旷课 | `absent` |
| △ | 事假 | `personal_leave` |
| ○ | 病假 | `sick_leave` |
| + | 迟到 | `late` |
| – | 早退 | `early_leave` |

## API 示例

### 获取学生列表

```bash
curl http://localhost:8080/api/v1/students?page=1&page_size=20
```

### 新增学生

```bash
curl -X POST http://localhost:8080/api/v1/students \
  -H "Content-Type: application/json" \
  -d '{"student_id": "2024001", "name": "张三", "class": "人文2401班"}'
```

### 新增考勤记录

```bash
curl -X POST http://localhost:8080/api/v1/attendances \
  -H "Content-Type: application/json" \
  -d '{"student_id": "2024001", "date": "12-15", "status": "present"}'
```

### 批量新增考勤

```bash
curl -X POST http://localhost:8080/api/v1/attendances/batch \
  -H "Content-Type: application/json" \
  -d '{
    "date": "12-15",
    "records": [
      {"student_id": "2024001", "status": "present"},
      {"student_id": "2024002", "status": "late"}
    ]
  }'
```

### 获取考勤日报表

```bash
curl "http://localhost:8080/api/v1/reports/daily?date=12-15"
```

### 导出数据

```bash
curl "http://localhost:8080/api/v1/data/export?type=all&format=json"
```

## 响应格式

### 成功响应

```json
{
  "code": 200,
  "message": "success",
  "data": { ... }
}
```

### 错误响应

```json
{
  "code": 400,
  "message": "错误描述",
  "errors": [ ... ]
}
```

## 配置

编辑 `config.json` 修改服务器配置：

```json
{
  "listeners": [
    {
      "address": "0.0.0.0",
      "port": 8080,
      "https": false
    }
  ],
  "app": {
    "threads_num": 4,
    "log": {
      "log_path": "./logs",
      "log_level": "DEBUG"
    }
  }
}
```

## 许可证

MIT License
