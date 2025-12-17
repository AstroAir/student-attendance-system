#include <gtest/gtest.h>
#include <set>
#include "student_attendance/db/DatabaseManager.h"
#include "student_attendance/models/DataStore.h"

using namespace student_attendance::models;
using namespace student_attendance::db;

class ClassApiTest : public ::testing::Test
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

// GET /api/v1/classes - 获取班级列表
TEST_F(ClassApiTest, GetClasses_ReturnsAll)
{
    auto classes = DataStore::getInstance().getAllClasses();
    EXPECT_GT(classes.size(), 0u);
}

TEST_F(ClassApiTest, GetClasses_ContainsExpectedClasses)
{
    auto classes = DataStore::getInstance().getAllClasses();

    bool found人文2401 = false;
    bool found人文2402 = false;

    for (const auto &cls : classes)
    {
        if (cls == "人文2401班") found人文2401 = true;
        if (cls == "人文2402班") found人文2402 = true;
    }

    EXPECT_TRUE(found人文2401);
    EXPECT_TRUE(found人文2402);
}

TEST_F(ClassApiTest, GetClassStudentCount_ReturnsCorrectCount)
{
    int count = DataStore::getInstance().getClassStudentCount("人文2401班");
    EXPECT_GT(count, 0);
}

// GET /api/v1/classes/{class_name}/students - 获取班级学生
TEST_F(ClassApiTest, GetClassStudents_ReturnsStudents)
{
    auto students = DataStore::getInstance().getStudentsByClass("人文2401班");
    EXPECT_GT(students.size(), 0u);

    for (const auto &student : students)
    {
        EXPECT_EQ(student.className, "人文2401班");
    }
}

TEST_F(ClassApiTest, GetClassStudents_EmptyForNonExistentClass)
{
    auto students = DataStore::getInstance().getStudentsByClass("不存在的班级");
    EXPECT_EQ(students.size(), 0u);
}

TEST_F(ClassApiTest, GetClassStudents_StudentHasRequiredFields)
{
    auto students = DataStore::getInstance().getStudentsByClass("人文2401班");

    for (const auto &student : students)
    {
        EXPECT_FALSE(student.studentId.empty());
        EXPECT_FALSE(student.name.empty());
        EXPECT_FALSE(student.className.empty());
    }
}

TEST_F(ClassApiTest, GetClassStudents_ToBasicJson)
{
    auto students = DataStore::getInstance().getStudentsByClass("人文2401班");

    for (const auto &student : students)
    {
        auto json = student.toBasicJson();
        EXPECT_TRUE(json.isMember("student_id"));
        EXPECT_TRUE(json.isMember("name"));
        EXPECT_FALSE(json.isMember("class"));  // Basic JSON should not have class
    }
}

// ==================== 边界条件测试 ====================

TEST_F(ClassApiTest, GetClasses_NotEmpty)
{
    auto classes = DataStore::getInstance().getAllClasses();
    EXPECT_FALSE(classes.empty());
}

TEST_F(ClassApiTest, GetClasses_UniqueClasses)
{
    auto classes = DataStore::getInstance().getAllClasses();
    std::set<std::string> uniqueClasses(classes.begin(), classes.end());
    EXPECT_EQ(classes.size(), uniqueClasses.size());
}

TEST_F(ClassApiTest, GetClassStudentCount_EmptyClassName)
{
    int count = DataStore::getInstance().getClassStudentCount("");
    EXPECT_EQ(count, 0);
}

TEST_F(ClassApiTest, GetClassStudentCount_MatchesStudentList)
{
    auto classes = DataStore::getInstance().getAllClasses();
    for (const auto &className : classes)
    {
        int count = DataStore::getInstance().getClassStudentCount(className);
        auto students = DataStore::getInstance().getStudentsByClass(className);
        EXPECT_EQ(count, static_cast<int>(students.size()))
            << "Mismatch for class: " << className;
    }
}

TEST_F(ClassApiTest, GetClassStudents_EmptyClassName)
{
    auto students = DataStore::getInstance().getStudentsByClass("");
    EXPECT_EQ(students.size(), 0u);
}

TEST_F(ClassApiTest, GetClassStudents_SpecialCharactersInClassName)
{
    auto students = DataStore::getInstance().getStudentsByClass("人文2401班（A）");
    // Should handle special characters without crashing
    EXPECT_GE(students.size(), 0u);
}

TEST_F(ClassApiTest, GetClassStudents_AllStudentsHaveCorrectClass)
{
    auto classes = DataStore::getInstance().getAllClasses();
    for (const auto &className : classes)
    {
        auto students = DataStore::getInstance().getStudentsByClass(className);
        for (const auto &student : students)
        {
            EXPECT_EQ(student.className, className)
                << "Student " << student.studentId << " has wrong class";
        }
    }
}

TEST_F(ClassApiTest, GetClassStudents_ToJson)
{
    auto students = DataStore::getInstance().getStudentsByClass("人文2401班");

    for (const auto &student : students)
    {
        auto json = student.toJson();
        EXPECT_TRUE(json.isMember("student_id"));
        EXPECT_TRUE(json.isMember("name"));
        EXPECT_TRUE(json.isMember("class"));
        EXPECT_EQ(json["class"].asString(), "人文2401班");
    }
}

TEST_F(ClassApiTest, GetClasses_AfterAddingStudent)
{
    auto initialClasses = DataStore::getInstance().getAllClasses();
    size_t initialCount = initialClasses.size();

    // Add a student with a new class
    Student newStudent("NEWCLASS001", "新班级学生", "全新班级");
    DataStore::getInstance().addStudent(newStudent);

    auto finalClasses = DataStore::getInstance().getAllClasses();
    EXPECT_EQ(finalClasses.size(), initialCount + 1);

    bool found = false;
    for (const auto &cls : finalClasses)
    {
        if (cls == "全新班级")
        {
            found = true;
            break;
        }
    }
    EXPECT_TRUE(found);
}

TEST_F(ClassApiTest, GetClassStudentCount_AfterAddingStudent)
{
    int initialCount = DataStore::getInstance().getClassStudentCount("人文2401班");

    Student newStudent("ADDCOUNT001", "新增学生", "人文2401班");
    DataStore::getInstance().addStudent(newStudent);

    int finalCount = DataStore::getInstance().getClassStudentCount("人文2401班");
    EXPECT_EQ(finalCount, initialCount + 1);
}

