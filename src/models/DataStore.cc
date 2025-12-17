#include "student_attendance/models/DataStore.h"

namespace student_attendance
{
namespace models
{

DataStore::DataStore() : nextAttendanceId_(1)
{
    initSampleData();
}

void DataStore::initSampleData()
{
    // Add sample students
    std::vector<Student> sampleStudents = {
        {"2024001", "张三", "人文2401班"},
        {"2024002", "李四", "人文2401班"},
        {"2024003", "王五", "人文2401班"},
        {"2024004", "赵六", "人文2402班"},
        {"2024005", "钱七", "人文2402班"},
        {"2024006", "孙八", "人文2402班"},
        {"2024007", "周九", "人文2403班"},
        {"2024008", "吴十", "人文2403班"}
    };

    for (const auto &student : sampleStudents)
    {
        students_[student.studentId] = student;
    }

    // Add sample attendances
    std::vector<std::pair<std::string, std::string>> sampleAttendances = {
        {"2024001", "present"},
        {"2024002", "present"},
        {"2024003", "late"},
        {"2024004", "absent"},
        {"2024005", "present"},
        {"2024006", "sick_leave"},
        {"2024007", "present"},
        {"2024008", "personal_leave"}
    };

    for (const auto &[studentId, status] : sampleAttendances)
    {
        auto it = students_.find(studentId);
        if (it != students_.end())
        {
            Attendance att;
            att.id = nextAttendanceId_++;
            att.studentId = studentId;
            att.name = it->second.name;
            att.className = it->second.className;
            att.date = "12-15";
            att.status = status;
            att.remark = "";
            attendances_[att.id] = att;
        }
    }
}

std::vector<Student> DataStore::getAllStudents() const
{
    std::lock_guard<std::mutex> lock(studentMutex_);
    std::vector<Student> result;
    result.reserve(students_.size());
    for (const auto &[id, student] : students_)
    {
        result.push_back(student);
    }
    return result;
}

std::optional<Student> DataStore::getStudentById(const std::string &studentId) const
{
    std::lock_guard<std::mutex> lock(studentMutex_);
    auto it = students_.find(studentId);
    if (it != students_.end())
    {
        return it->second;
    }
    return std::nullopt;
}

bool DataStore::addStudent(const Student &student)
{
    std::lock_guard<std::mutex> lock(studentMutex_);
    if (students_.find(student.studentId) != students_.end())
    {
        return false;  // Already exists
    }
    students_[student.studentId] = student;
    return true;
}

bool DataStore::updateStudent(const std::string &studentId, const Student &student)
{
    std::lock_guard<std::mutex> lock(studentMutex_);
    auto it = students_.find(studentId);
    if (it == students_.end())
    {
        return false;
    }
    it->second.name = student.name.empty() ? it->second.name : student.name;
    it->second.className = student.className.empty() ? it->second.className : student.className;
    return true;
}

bool DataStore::deleteStudent(const std::string &studentId)
{
    std::lock_guard<std::mutex> lock(studentMutex_);
    return students_.erase(studentId) > 0;
}

bool DataStore::studentExists(const std::string &studentId) const
{
    std::lock_guard<std::mutex> lock(studentMutex_);
    return students_.find(studentId) != students_.end();
}

std::vector<Student> DataStore::searchStudents(const std::string &keyword,
                                               const std::string &className) const
{
    std::lock_guard<std::mutex> lock(studentMutex_);
    std::vector<Student> result;

    for (const auto &[id, student] : students_)
    {
        bool match = true;

        if (!className.empty() && student.className != className)
        {
            match = false;
        }

        if (!keyword.empty())
        {
            bool keywordMatch = (student.studentId.find(keyword) != std::string::npos) ||
                               (student.name.find(keyword) != std::string::npos);
            if (!keywordMatch)
            {
                match = false;
            }
        }

        if (match)
        {
            result.push_back(student);
        }
    }

    return result;
}

std::vector<Attendance> DataStore::getAllAttendances() const
{
    std::lock_guard<std::mutex> lock(attendanceMutex_);
    std::vector<Attendance> result;
    result.reserve(attendances_.size());
    for (const auto &[id, att] : attendances_)
    {
        result.push_back(att);
    }
    return result;
}

std::optional<Attendance> DataStore::getAttendanceById(int id) const
{
    std::lock_guard<std::mutex> lock(attendanceMutex_);
    auto it = attendances_.find(id);
    if (it != attendances_.end())
    {
        return it->second;
    }
    return std::nullopt;
}

int DataStore::addAttendance(const Attendance &attendance)
{
    std::lock_guard<std::mutex> lock(attendanceMutex_);
    Attendance att = attendance;
    att.id = nextAttendanceId_++;
    attendances_[att.id] = att;
    return att.id;
}

bool DataStore::updateAttendance(int id, const Attendance &attendance)
{
    std::lock_guard<std::mutex> lock(attendanceMutex_);
    auto it = attendances_.find(id);
    if (it == attendances_.end())
    {
        return false;
    }
    if (!attendance.status.empty())
    {
        it->second.status = attendance.status;
    }
    it->second.remark = attendance.remark;
    return true;
}

bool DataStore::deleteAttendance(int id)
{
    std::lock_guard<std::mutex> lock(attendanceMutex_);
    return attendances_.erase(id) > 0;
}

std::vector<Attendance> DataStore::searchAttendances(
    const std::string &studentId,
    const std::string &name,
    const std::string &className,
    const std::string &date,
    const std::string &startDate,
    const std::string &endDate,
    const std::string &status) const
{
    std::lock_guard<std::mutex> lock(attendanceMutex_);
    std::vector<Attendance> result;

    for (const auto &[id, att] : attendances_)
    {
        bool match = true;

        if (!studentId.empty() && att.studentId != studentId)
            match = false;
        if (!name.empty() && att.name.find(name) == std::string::npos)
            match = false;
        if (!className.empty() && att.className != className)
            match = false;
        if (!date.empty() && att.date != date)
            match = false;
        if (!startDate.empty() && att.date < startDate)
            match = false;
        if (!endDate.empty() && att.date > endDate)
            match = false;
        if (!status.empty() && att.status != status)
            match = false;

        if (match)
        {
            result.push_back(att);
        }
    }

    return result;
}

std::vector<std::string> DataStore::getAllClasses() const
{
    std::lock_guard<std::mutex> lock(studentMutex_);
    std::unordered_map<std::string, bool> classSet;
    for (const auto &[id, student] : students_)
    {
        classSet[student.className] = true;
    }

    std::vector<std::string> result;
    for (const auto &[className, _] : classSet)
    {
        result.push_back(className);
    }
    std::sort(result.begin(), result.end());
    return result;
}

std::vector<Student> DataStore::getStudentsByClass(const std::string &className) const
{
    std::lock_guard<std::mutex> lock(studentMutex_);
    std::vector<Student> result;
    for (const auto &[id, student] : students_)
    {
        if (student.className == className)
        {
            result.push_back(student);
        }
    }
    return result;
}

int DataStore::getClassStudentCount(const std::string &className) const
{
    std::lock_guard<std::mutex> lock(studentMutex_);
    int count = 0;
    for (const auto &[id, student] : students_)
    {
        if (student.className == className)
        {
            count++;
        }
    }
    return count;
}

void DataStore::clear()
{
    {
        std::lock_guard<std::mutex> lock(studentMutex_);
        students_.clear();
    }
    {
        std::lock_guard<std::mutex> lock(attendanceMutex_);
        attendances_.clear();
        nextAttendanceId_ = 1;
    }
}

void DataStore::importStudents(const std::vector<Student> &students)
{
    std::lock_guard<std::mutex> lock(studentMutex_);
    for (const auto &student : students)
    {
        students_[student.studentId] = student;
    }
}

void DataStore::importAttendances(const std::vector<Attendance> &attendances)
{
    std::lock_guard<std::mutex> lock(attendanceMutex_);
    for (const auto &att : attendances)
    {
        Attendance newAtt = att;
        newAtt.id = nextAttendanceId_++;
        attendances_[newAtt.id] = newAtt;
    }
}

void DataStore::reset()
{
    std::scoped_lock lock(studentMutex_, attendanceMutex_);
    students_.clear();
    attendances_.clear();
    nextAttendanceId_ = 1;
    initSampleData();
}

}  // namespace models
}  // namespace student_attendance
