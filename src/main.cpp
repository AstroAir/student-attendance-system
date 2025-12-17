#include <iostream>
#include <string>

int main(int argc, char **argv)
{
  std::cout << "========================================" << std::endl;
  std::cout << "  学生考勤系统 - Student Attendance System" << std::endl;
  std::cout << "========================================" << std::endl;
  std::cout << std::endl;
  std::cout << "Usage:" << std::endl;
  std::cout << "  student_attendance_server  - Start the API server" << std::endl;
  std::cout << "  student_attendance_app     - Show this help" << std::endl;
  std::cout << std::endl;
  std::cout << "To start the server, run:" << std::endl;
  std::cout << "  ./student_attendance_server" << std::endl;
  std::cout << std::endl;
  std::cout << "API will be available at: http://localhost:8080/api/v1" << std::endl;

  return 0;
}
