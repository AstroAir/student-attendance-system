#include "project_name/core/greeting.hpp"
#include "project_name/utils/string_utils.hpp"

#include <iostream>

int main()
{
  std::cout << project_name::core::greeting("example") << '\n';

  const auto tokens = project_name::utils::split("a,b,c", ',');
  for (const auto &t : tokens)
  {
    std::cout << project_name::utils::trim(t) << '\n';
  }

  return 0;
}
