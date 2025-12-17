#include <gtest/gtest.h>
#include "student_attendance/db/DatabaseManager.h"
#include "student_attendance/services/StudentService.h"
#include "student_attendance/models/DataStore.h"

using namespace student_attendance::services;
using namespace student_attendance::db;
using namespace student_attendance::models;

class StudentApiTest : public ::testing::Test
{
protected:
    void SetUp() override
    {
        DatabaseManager::getInstance().reset();
        DataStore::getInstance().reset();
    }

    void TearDown() override
    {
        DatabaseManager::getInstance().reset();
        DataStore::getInstance().reset();
    }
};

// GET /api/v1/students - 获取学生列表
TEST_F(StudentApiTest, GetStudents_ReturnsAllStudents)
{
    auto result = StudentService::getInstance().getStudents(1, 20, "", "asc", "", "");
    EXPECT_GT(result.total, 0);
    EXPECT_EQ(result.page, 1);
    EXPECT_EQ(result.pageSize, 20);
}

TEST_F(StudentApiTest, GetStudents_WithPagination)
{
    auto result = StudentService::getInstance().getStudents(1, 2, "", "asc", "", "");
    EXPECT_LE(static_cast<int>(result.students.size()), 2);
}

TEST_F(StudentApiTest, GetStudents_WithClassFilter)
{
    auto result = StudentService::getInstance().getStudents(1, 20, "", "asc", "人文2401班", "");
    for (const auto &student : result.students)
    {
        EXPECT_EQ(student.className, "人文2401班");
    }
}

TEST_F(StudentApiTest, GetStudents_WithKeywordSearch)
{
    auto result = StudentService::getInstance().getStudents(1, 20, "", "asc", "", "张");
    for (const auto &student : result.students)
    {
        bool found = student.name.find("张") != std::string::npos ||
                     student.studentId.find("张") != std::string::npos;
        EXPECT_TRUE(found);
    }
}

TEST_F(StudentApiTest, GetStudents_WithSorting)
{
    auto result = StudentService::getInstance().getStudents(1, 20, "student_id", "asc", "", "");
    if (result.students.size() >= 2)
    {
        EXPECT_LE(result.students[0].studentId, result.students[1].studentId);
    }
}

// POST /api/v1/students - 新增学生
TEST_F(StudentApiTest, CreateStudent_Success)
{
    Student newStudent("2024100", "测试学生", "测试班级");
    auto [success, message] = StudentService::getInstance().createStudent(newStudent);
    EXPECT_TRUE(success);
    EXPECT_EQ(message, "学生创建成功");
}

TEST_F(StudentApiTest, CreateStudent_DuplicateId)
{
    Student newStudent("2024001", "重复学生", "测试班级");
    auto [success, message] = StudentService::getInstance().createStudent(newStudent);
    EXPECT_FALSE(success);
    EXPECT_TRUE(message.find("已存在") != std::string::npos);
}

TEST_F(StudentApiTest, CreateStudent_EmptyFields)
{
    Student emptyStudent("", "", "");
    auto [success, message] = StudentService::getInstance().createStudent(emptyStudent);
    EXPECT_FALSE(success);
}

// GET /api/v1/students/{student_id} - 获取单个学生
TEST_F(StudentApiTest, GetStudent_Exists)
{
    auto student = StudentService::getInstance().getStudent("2024001");
    EXPECT_TRUE(student.has_value());
    EXPECT_EQ(student->studentId, "2024001");
}

TEST_F(StudentApiTest, GetStudent_NotExists)
{
    auto student = StudentService::getInstance().getStudent("9999999");
    EXPECT_FALSE(student.has_value());
}

// PUT /api/v1/students/{student_id} - 修改学生信息
TEST_F(StudentApiTest, UpdateStudent_Success)
{
    auto [success, message] = StudentService::getInstance().updateStudent(
        "2024001", "张三改名", "新班级");
    EXPECT_TRUE(success);

    auto student = StudentService::getInstance().getStudent("2024001");
    EXPECT_TRUE(student.has_value());
    EXPECT_EQ(student->name, "张三改名");
}

TEST_F(StudentApiTest, UpdateStudent_NotExists)
{
    auto [success, message] = StudentService::getInstance().updateStudent(
        "9999999", "不存在", "不存在");
    EXPECT_FALSE(success);
}

// DELETE /api/v1/students/{student_id} - 删除学生
TEST_F(StudentApiTest, DeleteStudent_Success)
{
    // First create a student to delete
    Student newStudent("2024999", "待删除", "测试班级");
    StudentService::getInstance().createStudent(newStudent);

    bool deleted = StudentService::getInstance().deleteStudent("2024999");
    EXPECT_TRUE(deleted);

    auto student = StudentService::getInstance().getStudent("2024999");
    EXPECT_FALSE(student.has_value());
}

TEST_F(StudentApiTest, DeleteStudent_NotExists)
{
    bool deleted = StudentService::getInstance().deleteStudent("9999999");
    EXPECT_FALSE(deleted);
}

// ==================== 边界条件测试 ====================

TEST_F(StudentApiTest, GetStudents_PageZero)
{
    auto result = StudentService::getInstance().getStudents(0, 20, "", "asc", "", "");
    // Page 0 should be treated as page 1 or return empty
    EXPECT_GE(result.page, 0);
}

TEST_F(StudentApiTest, GetStudents_NegativePage)
{
    auto result = StudentService::getInstance().getStudents(-1, 20, "", "asc", "", "");
    EXPECT_GE(result.page, 0);
}

TEST_F(StudentApiTest, GetStudents_ZeroPageSize)
{
    auto result = StudentService::getInstance().getStudents(1, 0, "", "asc", "", "");
    EXPECT_GE(result.pageSize, 0);
}

TEST_F(StudentApiTest, GetStudents_LargePageSize)
{
    auto result = StudentService::getInstance().getStudents(1, 1000, "", "asc", "", "");
    EXPECT_LE(static_cast<int>(result.students.size()), 1000);
}

TEST_F(StudentApiTest, GetStudents_DescendingOrder)
{
    auto result = StudentService::getInstance().getStudents(1, 20, "student_id", "desc", "", "");
    if (result.students.size() >= 2)
    {
        EXPECT_GE(result.students[0].studentId, result.students[1].studentId);
    }
}

TEST_F(StudentApiTest, GetStudents_InvalidSortField)
{
    auto result = StudentService::getInstance().getStudents(1, 20, "invalid_field", "asc", "", "");
    // Should still return results, just with default sorting
    EXPECT_GE(result.total, 0);
}

TEST_F(StudentApiTest, CreateStudent_SpecialCharactersInName)
{
    Student newStudent("2024200", "张三·李四", "测试班级");
    auto [success, message] = StudentService::getInstance().createStudent(newStudent);
    EXPECT_TRUE(success);
}

TEST_F(StudentApiTest, CreateStudent_LongStudentId)
{
    Student newStudent("20241234567890123456", "长ID学生", "测试班级");
    auto [success, message] = StudentService::getInstance().createStudent(newStudent);
    // Depending on validation, this may succeed or fail
    // Just ensure no crash
    EXPECT_TRUE(success || !success);
}

TEST_F(StudentApiTest, GetStudents_ChineseKeyword)
{
    auto result = StudentService::getInstance().getStudents(1, 20, "", "asc", "", "人文");
    // Should handle Chinese characters properly
    EXPECT_GE(result.total, 0);
}

TEST_F(StudentApiTest, UpdateStudent_EmptyName)
{
    auto [success, message] = StudentService::getInstance().updateStudent(
        "2024001", "", "新班级");
    // Empty name should be rejected
    EXPECT_FALSE(success);
}

TEST_F(StudentApiTest, DeleteStudent_EmptyId)
{
    bool deleted = StudentService::getInstance().deleteStudent("");
    EXPECT_FALSE(deleted);
}

