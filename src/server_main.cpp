#include <drogon/drogon.h>
#include <iostream>
#include "student_attendance/db/DatabaseManager.h"

int main()
{
    // Load configuration
    try
    {
        drogon::app().loadConfigFile("config.json");
    }
    catch (const std::exception &e)
    {
        std::cerr << "Failed to load config file: " << e.what() << std::endl;
        std::cerr << "Using default configuration..." << std::endl;

        // Default configuration
        drogon::app()
            .setLogPath("./logs")
            .setLogLevel(trantor::Logger::kDebug)
            .addListener("0.0.0.0", 8080)
            .setThreadNum(4);
    }

    // Register database initialization callback
    drogon::app().registerBeginningAdvice([]() {
        std::cout << "Initializing database..." << std::endl;
        student_attendance::db::DatabaseManager::getInstance().initialize(
            "./student_attendance.db"
        );
        std::cout << "Database initialized successfully." << std::endl;
    });

    // Print startup information
    std::cout << "========================================" << std::endl;
    std::cout << "  学生考勤系统 API 服务器" << std::endl;
    std::cout << "  Student Attendance System API Server" << std::endl;
    std::cout << "========================================" << std::endl;
    std::cout << std::endl;
    std::cout << "API Base URL: http://localhost:8080/api/v1" << std::endl;
    std::cout << std::endl;
    std::cout << "Available Endpoints:" << std::endl;
    std::cout << "  Students:" << std::endl;
    std::cout << "    GET    /api/v1/students" << std::endl;
    std::cout << "    POST   /api/v1/students" << std::endl;
    std::cout << "    GET    /api/v1/students/{student_id}" << std::endl;
    std::cout << "    PUT    /api/v1/students/{student_id}" << std::endl;
    std::cout << "    DELETE /api/v1/students/{student_id}" << std::endl;
    std::cout << std::endl;
    std::cout << "  Attendances:" << std::endl;
    std::cout << "    GET    /api/v1/attendances" << std::endl;
    std::cout << "    POST   /api/v1/attendances" << std::endl;
    std::cout << "    POST   /api/v1/attendances/batch" << std::endl;
    std::cout << "    GET    /api/v1/attendances/{id}" << std::endl;
    std::cout << "    PUT    /api/v1/attendances/{id}" << std::endl;
    std::cout << "    DELETE /api/v1/attendances/{id}" << std::endl;
    std::cout << std::endl;
    std::cout << "  Reports:" << std::endl;
    std::cout << "    GET    /api/v1/reports/details" << std::endl;
    std::cout << "    GET    /api/v1/reports/daily" << std::endl;
    std::cout << "    GET    /api/v1/reports/summary" << std::endl;
    std::cout << "    GET    /api/v1/reports/abnormal" << std::endl;
    std::cout << "    GET    /api/v1/reports/leave" << std::endl;
    std::cout << std::endl;
    std::cout << "  Data:" << std::endl;
    std::cout << "    GET    /api/v1/data/export" << std::endl;
    std::cout << "    POST   /api/v1/data/import" << std::endl;
    std::cout << std::endl;
    std::cout << "  Classes:" << std::endl;
    std::cout << "    GET    /api/v1/classes" << std::endl;
    std::cout << "    GET    /api/v1/classes/{class_name}/students" << std::endl;
    std::cout << std::endl;
    std::cout << "Server starting..." << std::endl;

    // Run the server
    drogon::app().run();

    return 0;
}
