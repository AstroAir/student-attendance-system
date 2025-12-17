#pragma once

#include <drogon/orm/DbClient.h>
#include <string>

namespace student_attendance
{
namespace db
{

class DatabaseManager
{
public:
    static DatabaseManager &getInstance()
    {
        static DatabaseManager instance;
        return instance;
    }

    // Initialize database with schema
    void initialize(const std::string &dbPath = "./student_attendance.db");

    // Get database client
    drogon::orm::DbClientPtr getClient() const { return dbClient_; }

    // Execute schema initialization
    void initializeSchema();

    // Check if database is ready
    bool isReady() const { return dbClient_ != nullptr; }

    // Reset database (for testing)
    void reset();

private:
    DatabaseManager() = default;
    ~DatabaseManager() = default;
    DatabaseManager(const DatabaseManager &) = delete;
    DatabaseManager &operator=(const DatabaseManager &) = delete;

    drogon::orm::DbClientPtr dbClient_;
    std::string dbPath_;
};

}  // namespace db
}  // namespace student_attendance
