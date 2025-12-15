#include "project_name/core/greeting.hpp"

#include <string>

namespace project_name::core
{

std::string greeting(std::string_view name)
{
  if (name.empty())
  {
    return "Hello!";
  }
  return "Hello, " + std::string{name} + "!";
}

} // namespace project_name::core
