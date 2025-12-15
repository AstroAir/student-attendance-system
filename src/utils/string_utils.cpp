#include "project_name/utils/string_utils.hpp"

#include <algorithm>
#include <cctype>
#include <string>
#include <vector>

namespace project_name::utils
{

std::string to_upper(std::string_view input)
{
  std::string out{input};
  std::transform(out.begin(), out.end(), out.begin(),
                 [](unsigned char ch) { return static_cast<char>(std::toupper(ch)); });
  return out;
}

std::string trim(std::string_view input)
{
  const auto is_space = [](unsigned char ch) { return std::isspace(ch) != 0; };

  std::size_t begin = 0;
  while (begin < input.size() && is_space(static_cast<unsigned char>(input[begin])))
  {
    ++begin;
  }

  std::size_t end = input.size();
  while (end > begin && is_space(static_cast<unsigned char>(input[end - 1])))
  {
    --end;
  }

  return std::string{input.substr(begin, end - begin)};
}

std::vector<std::string> split(std::string_view input, char delimiter)
{
  std::vector<std::string> tokens;

  std::size_t start = 0;
  while (start <= input.size())
  {
    const std::size_t pos = input.find(delimiter, start);
    if (pos == std::string_view::npos)
    {
      tokens.emplace_back(input.substr(start));
      break;
    }
    tokens.emplace_back(input.substr(start, pos - start));
    start = pos + 1;
  }

  return tokens;
}

} // namespace project_name::utils
