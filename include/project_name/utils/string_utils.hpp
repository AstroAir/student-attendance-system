#pragma once

#include <string>
#include <string_view>
#include <vector>

namespace project_name::utils
{

std::string to_upper(std::string_view input);
std::string trim(std::string_view input);
std::vector<std::string> split(std::string_view input, char delimiter);

} // namespace project_name::utils
