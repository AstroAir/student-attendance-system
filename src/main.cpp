#include "project_name/core/greeting.hpp"
#include "project_name/utils/string_utils.hpp"

#include <iostream>
#include <string>

int main(int argc, char **argv)
{
  const std::string name = (argc >= 2) ? argv[1] : "world";

  std::cout << project_name::core::greeting(name) << '\n';
  std::cout << project_name::utils::to_upper("cpp-quick-starter") << '\n';

  return 0;
}
