#include <gtest/gtest.h>
#include "student_attendance/db/DatabaseManager.h"
#include "student_attendance/models/Student.h"
#include "student_attendance/models/Attendance.h"
#include "student_attendance/models/DataStore.h"

using namespace student_attendance::db;
using namespace student_attendance::models;

class DataApiTest : public ::testing::Test
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

// GET /api/v1/data/export - 导出数据
TEST_F(DataApiTest, ExportStudents_ReturnsData)
{
    auto students = DataStore::getInstance().getAllStudents();
    EXPECT_GT(students.size(), 0u);
}

TEST_F(DataApiTest, ExportAttendances_ReturnsData)
{
    auto attendances = DataStore::getInstance().getAllAttendances();
    EXPECT_GT(attendances.size(), 0u);
}

TEST_F(DataApiTest, ExportStudents_JsonFormat)
{
    auto students = DataStore::getInstance().getAllStudents();
    for (const auto &student : students)
    {
        auto json = student.toJson();
        EXPECT_TRUE(json.isMember("student_id"));
        EXPECT_TRUE(json.isMember("name"));
        EXPECT_TRUE(json.isMember("class"));
    }
}

TEST_F(DataApiTest, ExportAttendances_JsonFormat)
{
    auto attendances = DataStore::getInstance().getAllAttendances();
    for (const auto &att : attendances)
    {
        auto json = att.toJson();
        EXPECT_TRUE(json.isMember("id"));
        EXPECT_TRUE(json.isMember("student_id"));
        EXPECT_TRUE(json.isMember("date"));
        EXPECT_TRUE(json.isMember("status"));
    }
}

// POST /api/v1/data/import - 导入数据
TEST_F(DataApiTest, ImportStudents_Success)
{
    std::vector<Student> newStudents = {
        Student("2024100", "导入学生1", "导入班级"),
        Student("2024101", "导入学生2", "导入班级")
    };

    DataStore::getInstance().importStudents(newStudents);

    auto student1 = DataStore::getInstance().getStudentById("2024100");
    auto student2 = DataStore::getInstance().getStudentById("2024101");

    EXPECT_TRUE(student1.has_value());
    EXPECT_TRUE(student2.has_value());
}

TEST_F(DataApiTest, ImportStudents_SkipsDuplicates)
{
    size_t initialCount = DataStore::getInstance().getAllStudents().size();

    std::vector<Student> duplicateStudents = {
        Student("2024001", "重复学生", "重复班级")  // Already exists
    };

    DataStore::getInstance().importStudents(duplicateStudents);

    size_t finalCount = DataStore::getInstance().getAllStudents().size();
    EXPECT_EQ(initialCount, finalCount);
}

TEST_F(DataApiTest, ClearAndImport)
{
    DataStore::getInstance().clear();

    EXPECT_EQ(DataStore::getInstance().getAllStudents().size(), 0u);
    EXPECT_EQ(DataStore::getInstance().getAllAttendances().size(), 0u);

    std::vector<Student> newStudents = {
        Student("NEW001", "新学生", "新班级")
    };

    DataStore::getInstance().importStudents(newStudents);
    EXPECT_EQ(DataStore::getInstance().getAllStudents().size(), 1u);
}

// ==================== 边界条件测试 ====================

TEST_F(DataApiTest, ImportStudents_EmptyList)
{
    size_t initialCount = DataStore::getInstance().getAllStudents().size();
    std::vector<Student> emptyList;
    DataStore::getInstance().importStudents(emptyList);
    size_t finalCount = DataStore::getInstance().getAllStudents().size();
    EXPECT_EQ(initialCount, finalCount);
}

TEST_F(DataApiTest, ImportStudents_LargeList)
{
    size_t initialCount = DataStore::getInstance().getAllStudents().size();

    std::vector<Student> largeList;
    for (int i = 0; i < 100; ++i)
    {
        largeList.emplace_back("BULK" + std::to_string(i), "批量学生" + std::to_string(i), "批量班级");
    }

    DataStore::getInstance().importStudents(largeList);

    size_t finalCount = DataStore::getInstance().getAllStudents().size();
    EXPECT_EQ(finalCount, initialCount + 100);
}

TEST_F(DataApiTest, ImportAttendances_EmptyList)
{
    size_t initialCount = DataStore::getInstance().getAllAttendances().size();
    std::vector<Attendance> emptyList;
    DataStore::getInstance().importAttendances(emptyList);
    size_t finalCount = DataStore::getInstance().getAllAttendances().size();
    EXPECT_EQ(initialCount, finalCount);
}

TEST_F(DataApiTest, ExportStudents_AfterClear)
{
    DataStore::getInstance().clear();
    auto students = DataStore::getInstance().getAllStudents();
    EXPECT_EQ(students.size(), 0u);
}

TEST_F(DataApiTest, ExportAttendances_AfterClear)
{
    DataStore::getInstance().clear();
    auto attendances = DataStore::getInstance().getAllAttendances();
    EXPECT_EQ(attendances.size(), 0u);
}

TEST_F(DataApiTest, ImportStudents_SpecialCharacters)
{
    std::vector<Student> specialStudents = {
        Student("SPEC001", "张三·李四", "人文2401班（A）"),
        Student("SPEC002", "王五/赵六", "人文2402班【B】")
    };

    DataStore::getInstance().importStudents(specialStudents);

    auto student1 = DataStore::getInstance().getStudentById("SPEC001");
    auto student2 = DataStore::getInstance().getStudentById("SPEC002");

    EXPECT_TRUE(student1.has_value());
    EXPECT_TRUE(student2.has_value());
    EXPECT_EQ(student1->name, "张三·李四");
    EXPECT_EQ(student2->name, "王五/赵六");
}

TEST_F(DataApiTest, Student_ToJsonFromJson_Consistency)
{
    auto students = DataStore::getInstance().getAllStudents();
    ASSERT_GT(students.size(), 0u);

    for (const auto &student : students)
    {
        auto json = student.toJson();
        Student restored = Student::fromJson(json);

        EXPECT_EQ(student.studentId, restored.studentId);
        EXPECT_EQ(student.name, restored.name);
        EXPECT_EQ(student.className, restored.className);
    }
}

TEST_F(DataApiTest, Attendance_ToJsonFromJson_Consistency)
{
    auto attendances = DataStore::getInstance().getAllAttendances();
    ASSERT_GT(attendances.size(), 0u);

    for (const auto &att : attendances)
    {
        auto json = att.toJson();
        Attendance restored = Attendance::fromJson(json);

        EXPECT_EQ(att.id, restored.id);
        EXPECT_EQ(att.studentId, restored.studentId);
        EXPECT_EQ(att.name, restored.name);
        EXPECT_EQ(att.className, restored.className);
        EXPECT_EQ(att.date, restored.date);
        EXPECT_EQ(att.status, restored.status);
        EXPECT_EQ(att.remark, restored.remark);
    }
}

