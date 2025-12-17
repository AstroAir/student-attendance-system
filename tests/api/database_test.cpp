#include <gtest/gtest.h>
#include "student_attendance/db/DatabaseManager.h"
#include "student_attendance/models/DataStore.h"

using namespace student_attendance::db;
using namespace student_attendance::models;

class DatabaseManagerTest : public ::testing::Test
{
protected:
    void SetUp() override
    {
        DatabaseManager::getInstance().reset();
    }

    void TearDown() override
    {
        DatabaseManager::getInstance().reset();
    }
};

// ==================== Singleton Tests ====================

TEST_F(DatabaseManagerTest, GetInstance_ReturnsSameInstance)
{
    auto &instance1 = DatabaseManager::getInstance();
    auto &instance2 = DatabaseManager::getInstance();
    EXPECT_EQ(&instance1, &instance2);
}

// ==================== Reset Tests ====================

TEST_F(DatabaseManagerTest, Reset_CanBeCalledMultipleTimes)
{
    EXPECT_NO_THROW(DatabaseManager::getInstance().reset());
    EXPECT_NO_THROW(DatabaseManager::getInstance().reset());
    EXPECT_NO_THROW(DatabaseManager::getInstance().reset());
}

// ==================== DataStore Integration Tests ====================

class DataStoreTest : public ::testing::Test
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

TEST_F(DataStoreTest, GetInstance_ReturnsSameInstance)
{
    auto &instance1 = DataStore::getInstance();
    auto &instance2 = DataStore::getInstance();
    EXPECT_EQ(&instance1, &instance2);
}

// ==================== Student Operations ====================

TEST_F(DataStoreTest, StudentExists_ExistingStudent)
{
    EXPECT_TRUE(DataStore::getInstance().studentExists("2024001"));
}

TEST_F(DataStoreTest, StudentExists_NonExistingStudent)
{
    EXPECT_FALSE(DataStore::getInstance().studentExists("9999999"));
}

TEST_F(DataStoreTest, StudentExists_EmptyId)
{
    EXPECT_FALSE(DataStore::getInstance().studentExists(""));
}

TEST_F(DataStoreTest, AddStudent_Success)
{
    Student newStudent("NEW001", "新学生", "新班级");
    bool added = DataStore::getInstance().addStudent(newStudent);
    EXPECT_TRUE(added);
    EXPECT_TRUE(DataStore::getInstance().studentExists("NEW001"));
}

TEST_F(DataStoreTest, AddStudent_Duplicate)
{
    Student existingStudent("2024001", "重复学生", "重复班级");
    bool added = DataStore::getInstance().addStudent(existingStudent);
    EXPECT_FALSE(added);
}

TEST_F(DataStoreTest, GetStudentById_Exists)
{
    auto student = DataStore::getInstance().getStudentById("2024001");
    EXPECT_TRUE(student.has_value());
    EXPECT_EQ(student->studentId, "2024001");
}

TEST_F(DataStoreTest, GetStudentById_NotExists)
{
    auto student = DataStore::getInstance().getStudentById("9999999");
    EXPECT_FALSE(student.has_value());
}

TEST_F(DataStoreTest, UpdateStudent_Success)
{
    Student updatedStudent("2024001", "更新后的名字", "更新后的班级");
    bool updated = DataStore::getInstance().updateStudent("2024001", updatedStudent);
    EXPECT_TRUE(updated);

    auto student = DataStore::getInstance().getStudentById("2024001");
    EXPECT_TRUE(student.has_value());
    EXPECT_EQ(student->name, "更新后的名字");
    EXPECT_EQ(student->className, "更新后的班级");
}

TEST_F(DataStoreTest, UpdateStudent_NotExists)
{
    Student updatedStudent("9999999", "不存在", "不存在");
    bool updated = DataStore::getInstance().updateStudent("9999999", updatedStudent);
    EXPECT_FALSE(updated);
}

TEST_F(DataStoreTest, DeleteStudent_Success)
{
    // First add a student to delete
    Student newStudent("DELETE001", "待删除", "测试班级");
    DataStore::getInstance().addStudent(newStudent);
    EXPECT_TRUE(DataStore::getInstance().studentExists("DELETE001"));

    bool deleted = DataStore::getInstance().deleteStudent("DELETE001");
    EXPECT_TRUE(deleted);
    EXPECT_FALSE(DataStore::getInstance().studentExists("DELETE001"));
}

TEST_F(DataStoreTest, DeleteStudent_NotExists)
{
    bool deleted = DataStore::getInstance().deleteStudent("9999999");
    EXPECT_FALSE(deleted);
}

TEST_F(DataStoreTest, GetAllStudents_ReturnsData)
{
    auto students = DataStore::getInstance().getAllStudents();
    EXPECT_GT(students.size(), 0u);
}

TEST_F(DataStoreTest, SearchStudents_ByKeyword)
{
    auto students = DataStore::getInstance().searchStudents("张", "");
    for (const auto &student : students)
    {
        bool found = student.name.find("张") != std::string::npos ||
                     student.studentId.find("张") != std::string::npos;
        EXPECT_TRUE(found);
    }
}

TEST_F(DataStoreTest, SearchStudents_ByClass)
{
    auto students = DataStore::getInstance().searchStudents("", "人文2401班");
    for (const auto &student : students)
    {
        EXPECT_EQ(student.className, "人文2401班");
    }
}

TEST_F(DataStoreTest, SearchStudents_ByKeywordAndClass)
{
    auto students = DataStore::getInstance().searchStudents("张", "人文2401班");
    for (const auto &student : students)
    {
        EXPECT_EQ(student.className, "人文2401班");
    }
}

TEST_F(DataStoreTest, SearchStudents_NoMatch)
{
    auto students = DataStore::getInstance().searchStudents("不存在的关键词", "");
    EXPECT_EQ(students.size(), 0u);
}

// ==================== Attendance Operations ====================

TEST_F(DataStoreTest, AddAttendance_ReturnsId)
{
    Attendance att(0, "2024001", "张三", "人文2401班", "12-20", "present", "");
    int id = DataStore::getInstance().addAttendance(att);
    EXPECT_GT(id, 0);
}

TEST_F(DataStoreTest, GetAttendanceById_Exists)
{
    auto att = DataStore::getInstance().getAttendanceById(1);
    EXPECT_TRUE(att.has_value());
    EXPECT_EQ(att->id, 1);
}

TEST_F(DataStoreTest, GetAttendanceById_NotExists)
{
    auto att = DataStore::getInstance().getAttendanceById(99999);
    EXPECT_FALSE(att.has_value());
}

TEST_F(DataStoreTest, UpdateAttendance_Success)
{
    // First get an existing attendance
    auto original = DataStore::getInstance().getAttendanceById(1);
    ASSERT_TRUE(original.has_value());

    Attendance updated = *original;
    updated.status = "late";
    updated.remark = "更新后的备注";

    bool result = DataStore::getInstance().updateAttendance(1, updated);
    EXPECT_TRUE(result);

    auto modified = DataStore::getInstance().getAttendanceById(1);
    EXPECT_TRUE(modified.has_value());
    EXPECT_EQ(modified->status, "late");
    EXPECT_EQ(modified->remark, "更新后的备注");
}

TEST_F(DataStoreTest, UpdateAttendance_NotExists)
{
    Attendance att(99999, "2024001", "张三", "人文2401班", "12-20", "present", "");
    bool result = DataStore::getInstance().updateAttendance(99999, att);
    EXPECT_FALSE(result);
}

TEST_F(DataStoreTest, DeleteAttendance_Success)
{
    // First add an attendance to delete
    Attendance att(0, "2024001", "张三", "人文2401班", "12-30", "present", "");
    int id = DataStore::getInstance().addAttendance(att);
    EXPECT_GT(id, 0);

    bool deleted = DataStore::getInstance().deleteAttendance(id);
    EXPECT_TRUE(deleted);

    auto result = DataStore::getInstance().getAttendanceById(id);
    EXPECT_FALSE(result.has_value());
}

TEST_F(DataStoreTest, DeleteAttendance_NotExists)
{
    bool deleted = DataStore::getInstance().deleteAttendance(99999);
    EXPECT_FALSE(deleted);
}

TEST_F(DataStoreTest, GetAllAttendances_ReturnsData)
{
    auto attendances = DataStore::getInstance().getAllAttendances();
    EXPECT_GT(attendances.size(), 0u);
}

TEST_F(DataStoreTest, SearchAttendances_ByStudentId)
{
    auto attendances = DataStore::getInstance().searchAttendances(
        "2024001", "", "", "", "", "", "");
    for (const auto &att : attendances)
    {
        EXPECT_EQ(att.studentId, "2024001");
    }
}

TEST_F(DataStoreTest, SearchAttendances_ByDate)
{
    auto attendances = DataStore::getInstance().searchAttendances(
        "", "", "", "12-15", "", "", "");
    for (const auto &att : attendances)
    {
        EXPECT_EQ(att.date, "12-15");
    }
}

TEST_F(DataStoreTest, SearchAttendances_ByStatus)
{
    auto attendances = DataStore::getInstance().searchAttendances(
        "", "", "", "", "", "", "present");
    for (const auto &att : attendances)
    {
        EXPECT_EQ(att.status, "present");
    }
}

TEST_F(DataStoreTest, SearchAttendances_ByClassName)
{
    auto attendances = DataStore::getInstance().searchAttendances(
        "", "", "人文2401班", "", "", "", "");
    for (const auto &att : attendances)
    {
        EXPECT_EQ(att.className, "人文2401班");
    }
}

TEST_F(DataStoreTest, SearchAttendances_ByDateRange)
{
    auto attendances = DataStore::getInstance().searchAttendances(
        "", "", "", "", "12-01", "12-31", "");
    for (const auto &att : attendances)
    {
        EXPECT_GE(att.date, "12-01");
        EXPECT_LE(att.date, "12-31");
    }
}

TEST_F(DataStoreTest, SearchAttendances_NoMatch)
{
    auto attendances = DataStore::getInstance().searchAttendances(
        "9999999", "", "", "", "", "", "");
    EXPECT_EQ(attendances.size(), 0u);
}

// ==================== Class Operations ====================

TEST_F(DataStoreTest, GetAllClasses_ReturnsData)
{
    auto classes = DataStore::getInstance().getAllClasses();
    EXPECT_GT(classes.size(), 0u);
}

TEST_F(DataStoreTest, GetStudentsByClass_ReturnsStudents)
{
    auto students = DataStore::getInstance().getStudentsByClass("人文2401班");
    EXPECT_GT(students.size(), 0u);
    for (const auto &student : students)
    {
        EXPECT_EQ(student.className, "人文2401班");
    }
}

TEST_F(DataStoreTest, GetStudentsByClass_EmptyForNonExistent)
{
    auto students = DataStore::getInstance().getStudentsByClass("不存在的班级");
    EXPECT_EQ(students.size(), 0u);
}

TEST_F(DataStoreTest, GetClassStudentCount_ReturnsCorrectCount)
{
    auto students = DataStore::getInstance().getStudentsByClass("人文2401班");
    int count = DataStore::getInstance().getClassStudentCount("人文2401班");
    EXPECT_EQ(count, static_cast<int>(students.size()));
}

TEST_F(DataStoreTest, GetClassStudentCount_ZeroForNonExistent)
{
    int count = DataStore::getInstance().getClassStudentCount("不存在的班级");
    EXPECT_EQ(count, 0);
}

// ==================== Import/Export Operations ====================

TEST_F(DataStoreTest, Clear_RemovesAllData)
{
    DataStore::getInstance().clear();
    EXPECT_EQ(DataStore::getInstance().getAllStudents().size(), 0u);
    EXPECT_EQ(DataStore::getInstance().getAllAttendances().size(), 0u);
}

TEST_F(DataStoreTest, ImportStudents_AddsNewStudents)
{
    size_t initialCount = DataStore::getInstance().getAllStudents().size();

    std::vector<Student> newStudents = {
        Student("IMPORT001", "导入学生1", "导入班级"),
        Student("IMPORT002", "导入学生2", "导入班级")
    };

    DataStore::getInstance().importStudents(newStudents);

    size_t finalCount = DataStore::getInstance().getAllStudents().size();
    EXPECT_EQ(finalCount, initialCount + 2);
}

TEST_F(DataStoreTest, ImportStudents_SkipsDuplicates)
{
    size_t initialCount = DataStore::getInstance().getAllStudents().size();

    std::vector<Student> duplicateStudents = {
        Student("2024001", "重复学生", "重复班级")
    };

    DataStore::getInstance().importStudents(duplicateStudents);

    size_t finalCount = DataStore::getInstance().getAllStudents().size();
    EXPECT_EQ(finalCount, initialCount);
}

TEST_F(DataStoreTest, ImportAttendances_AddsNewAttendances)
{
    size_t initialCount = DataStore::getInstance().getAllAttendances().size();

    std::vector<Attendance> newAttendances = {
        Attendance(0, "2024001", "张三", "人文2401班", "12-25", "present", ""),
        Attendance(0, "2024002", "李四", "人文2401班", "12-25", "late", "")
    };

    DataStore::getInstance().importAttendances(newAttendances);

    size_t finalCount = DataStore::getInstance().getAllAttendances().size();
    EXPECT_GE(finalCount, initialCount + 2);
}

TEST_F(DataStoreTest, Reset_RestoresSampleData)
{
    DataStore::getInstance().clear();
    EXPECT_EQ(DataStore::getInstance().getAllStudents().size(), 0u);

    DataStore::getInstance().reset();
    EXPECT_GT(DataStore::getInstance().getAllStudents().size(), 0u);
}

