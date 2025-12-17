#include "student_attendance/db/DatabaseManager.h"
#include <drogon/drogon.h>
#include <drogon/utils/Utilities.h>
#include <fstream>
#include <sstream>

namespace student_attendance
{
namespace db
{

void DatabaseManager::initialize(const std::string &dbPath)
{
    dbPath_ = dbPath;

    if (!dbClient_)
    {
        try
        {
            dbClient_ = drogon::app().getDbClient("default");
        }
        catch (const std::exception &)
        {
            dbClient_.reset();
        }
    }

    if (!dbClient_)
    {
        // Create SQLite database client with connection pool
        dbClient_ = drogon::orm::DbClient::newSqlite3Client(
            "filename=" + dbPath_,
            4  // Connection pool size
        );
    }

    // Initialize schema
    initializeSchema();
}

void DatabaseManager::initializeSchema()
{
    if (!dbClient_)
        return;

    // Create tables if not exist
    const char *createUsersTable = R"(
        CREATE TABLE IF NOT EXISTS users (
            id INTEGER PRIMARY KEY AUTOINCREMENT,
            username TEXT NOT NULL UNIQUE,
            role TEXT NOT NULL DEFAULT 'user',
            password_hash TEXT NOT NULL,
            salt TEXT NOT NULL,
            created_at DATETIME DEFAULT CURRENT_TIMESTAMP,
            updated_at DATETIME DEFAULT CURRENT_TIMESTAMP
        )
    )";

    const char *createStudentsTable = R"(
        CREATE TABLE IF NOT EXISTS students (
            student_id TEXT PRIMARY KEY NOT NULL,
            name TEXT NOT NULL,
            class_name TEXT NOT NULL,
            created_at DATETIME DEFAULT CURRENT_TIMESTAMP,
            updated_at DATETIME DEFAULT CURRENT_TIMESTAMP
        )
    )";

    const char *createAttendancesTable = R"(
        CREATE TABLE IF NOT EXISTS attendances (
            id INTEGER PRIMARY KEY AUTOINCREMENT,
            student_id TEXT NOT NULL,
            date TEXT NOT NULL,
            status TEXT NOT NULL CHECK(status IN ('present', 'absent', 'late', 'early_leave', 'sick_leave', 'personal_leave')),
            remark TEXT DEFAULT '',
            created_at DATETIME DEFAULT CURRENT_TIMESTAMP,
            updated_at DATETIME DEFAULT CURRENT_TIMESTAMP,
            FOREIGN KEY (student_id) REFERENCES students(student_id) ON DELETE CASCADE
        )
    )";

    const char *createIndexes[] = {
        "CREATE INDEX IF NOT EXISTS idx_users_username ON users(username)",
        "CREATE INDEX IF NOT EXISTS idx_students_class ON students(class_name)",
        "CREATE INDEX IF NOT EXISTS idx_students_name ON students(name)",
        "CREATE INDEX IF NOT EXISTS idx_attendances_student ON attendances(student_id)",
        "CREATE INDEX IF NOT EXISTS idx_attendances_date ON attendances(date)",
        "CREATE INDEX IF NOT EXISTS idx_attendances_status ON attendances(status)",
        "CREATE INDEX IF NOT EXISTS idx_attendances_student_date ON attendances(student_id, date)"
    };

    // Execute schema creation synchronously
    try
    {
        dbClient_->execSqlSync(createUsersTable);
        dbClient_->execSqlSync(createStudentsTable);
        dbClient_->execSqlSync(createAttendancesTable);

        for (const auto &indexSql : createIndexes)
        {
            dbClient_->execSqlSync(indexSql);
        }

        auto exists = dbClient_->execSqlSync(
            "SELECT 1 FROM users WHERE username = ? LIMIT 1",
            std::string("admin"));
        if (exists.empty())
        {
            const std::string username = "admin";
            const std::string role = "admin";
            const std::string password = "admin123";
            const std::string salt = drogon::utils::secureRandomString(16);
            const std::string passwordHash = drogon::utils::getSha256(salt + password);
            dbClient_->execSqlSync(
                "INSERT INTO users (username, role, password_hash, salt) VALUES (?, ?, ?, ?)",
                username,
                role,
                passwordHash,
                salt);
        }
    }
    catch (const drogon::orm::DrogonDbException &e)
    {
        LOG_ERROR << "Database schema initialization failed: " << e.base().what();
    }
}

void DatabaseManager::reset()
{
    if (!dbClient_)
    {
        initialize(dbPath_.empty() ? "./student_attendance.db" : dbPath_);
    }

    if (!dbClient_)
        return;

    try
    {
        // Clear all data
        dbClient_->execSqlSync("DELETE FROM attendances");
        dbClient_->execSqlSync("DELETE FROM students");
        dbClient_->execSqlSync("DELETE FROM users");

        // Reset autoincrement
        dbClient_->execSqlSync("DELETE FROM sqlite_sequence WHERE name='attendances'");
        dbClient_->execSqlSync("DELETE FROM sqlite_sequence WHERE name='users'");

        // Insert default admin user
        {
            const std::string username = "admin";
            const std::string role = "admin";
            const std::string password = "admin123";
            const std::string salt = drogon::utils::secureRandomString(16);
            const std::string passwordHash = drogon::utils::getSha256(salt + password);
            dbClient_->execSqlSync(
                "INSERT INTO users (id, username, role, password_hash, salt) VALUES (1, ?, ?, ?, ?)",
                username,
                role,
                passwordHash,
                salt);
        }

        // Insert sample data
        const char *insertStudents = R"(
            INSERT OR IGNORE INTO students (student_id, name, class_name) VALUES
            ('2024001', '张三', '人文2401班'),
            ('2024002', '李四', '人文2401班'),
            ('2024003', '王五', '人文2401班'),
            ('2024004', '赵六', '人文2402班'),
            ('2024005', '钱七', '人文2402班'),
            ('2024006', '孙八', '人文2402班'),
            ('2024007', '周九', '人文2403班'),
            ('2024008', '吴十', '人文2403班')
        )";

        const char *insertAttendances = R"(
            INSERT INTO attendances (student_id, date, status, remark) VALUES
            ('2024001', '12-15', 'present', ''),
            ('2024002', '12-15', 'present', ''),
            ('2024003', '12-15', 'late', '迟到5分钟'),
            ('2024004', '12-15', 'absent', ''),
            ('2024005', '12-15', 'present', ''),
            ('2024006', '12-15', 'sick_leave', '感冒'),
            ('2024007', '12-15', 'present', ''),
            ('2024008', '12-15', 'personal_leave', '家中有事')
        )";

        dbClient_->execSqlSync(insertStudents);
        dbClient_->execSqlSync(insertAttendances);
    }
    catch (const drogon::orm::DrogonDbException &e)
    {
        LOG_ERROR << "Database reset failed: " << e.base().what();
    }
}

}  // namespace db
}  // namespace student_attendance
