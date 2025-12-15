# Contributing to cpp-quick-starter

Thank you for your interest in contributing to cpp-quick-starter! This document provides guidelines and information about contributing.

## Table of Contents

- [Code of Conduct](#code-of-conduct)
- [Getting Started](#getting-started)
- [How to Contribute](#how-to-contribute)
- [Development Workflow](#development-workflow)
- [Coding Standards](#coding-standards)
- [Commit Guidelines](#commit-guidelines)
- [Pull Request Process](#pull-request-process)

## Code of Conduct

This project adheres to the [Code of Conduct](CODE_OF_CONDUCT.md). By participating, you are expected to uphold this code.

## Getting Started

1. Fork the repository
2. Clone your fork:

   ```bash
   git clone https://github.com/YOUR_USERNAME/cpp-quick-starter.git
   cd cpp-quick-starter
   ```

3. Add the upstream remote:

   ```bash
   git remote add upstream https://github.com/AstroAir/cpp-quick-starter.git
   ```

4. Create a branch for your changes:

   ```bash
   git checkout -b feature/your-feature-name
   ```

## How to Contribute

### Reporting Bugs

- Use the [bug report template](.github/ISSUE_TEMPLATE/bug_report.yml)
- Include detailed steps to reproduce
- Provide system information (OS, compiler version, etc.)
- Include relevant logs or error messages

### Suggesting Features

- Use the [feature request template](.github/ISSUE_TEMPLATE/feature_request.yml)
- Describe the problem you're trying to solve
- Explain your proposed solution
- Consider alternatives you've thought about

### Code Contributions

1. Check existing issues and PRs to avoid duplicates
2. For significant changes, open an issue first to discuss
3. Follow the coding standards and commit guidelines
4. Write tests for new functionality
5. Update documentation as needed

## Development Workflow

### Building the Project

```bash
# Using CMake
cmake --preset ninja-debug
cmake --build --preset ninja-debug

# Using xmake
xmake
```

### Running Tests

```bash
# CMake
ctest --preset ninja-debug

# xmake
xmake run unit_tests
xmake run integration_tests
```

### Code Formatting

Before submitting, format your code:

```bash
# Format all source files
./scripts/format.sh

# Or manually with clang-format
find include src tests -name '*.cpp' -o -name '*.hpp' | xargs clang-format -i
```

### Static Analysis

```bash
# Enable clang-tidy during build
cmake -B build -S . -DCPP_QUICK_STARTER_ENABLE_CLANG_TIDY=ON
cmake --build build
```

## Coding Standards

### C++ Style

- Follow the [C++ Core Guidelines](https://isocpp.github.io/CppCoreGuidelines/CppCoreGuidelines)
- Use C++20 features where appropriate
- Code must compile without warnings with `-Wall -Wextra -Wpedantic`

### Naming Conventions

- **Files**: `snake_case.cpp`, `snake_case.hpp`
- **Classes/Structs**: `PascalCase`
- **Functions/Methods**: `snake_case`
- **Variables**: `snake_case`
- **Constants**: `kPascalCase` or `SCREAMING_SNAKE_CASE`
- **Namespaces**: `snake_case`

### Code Organization

- Headers in `include/project_name/`
- Source files in `src/`
- Tests in `tests/unit/` or `tests/integration/`
- One class per file (generally)

### Documentation

- Use Doxygen-style comments for public APIs
- Include brief descriptions and parameter documentation
- Example:

  ```cpp
  /**
   * @brief Converts a string to uppercase.
   * @param input The input string to convert.
   * @return The uppercase version of the input string.
   */
  std::string to_upper(std::string_view input);
  ```

## Commit Guidelines

### Commit Message Format

Follow the [Conventional Commits](https://www.conventionalcommits.org/) specification:

```text
<type>(<scope>): <description>

[optional body]

[optional footer(s)]
```

### Types

- **feat**: New feature
- **fix**: Bug fix
- **docs**: Documentation changes
- **style**: Code style changes (formatting, etc.)
- **refactor**: Code refactoring
- **perf**: Performance improvements
- **test**: Adding or updating tests
- **build**: Build system changes
- **ci**: CI configuration changes
- **chore**: Other changes

### Examples

```text
feat(utils): add string split function

fix(core): handle empty input in greeting function

docs: update README with build instructions

test(utils): add unit tests for trim function
```

## Pull Request Process

1. **Update your branch** with the latest upstream changes:

   ```bash
   git fetch upstream
   git rebase upstream/main
   ```

2. **Ensure all tests pass**:

   ```bash
   cmake --preset ninja-ci
   cmake --build --preset ninja-ci
   ctest --preset ninja-ci
   ```

3. **Push your changes**:

   ```bash
   git push origin feature/your-feature-name
   ```

4. **Create a Pull Request**:
   - Use a clear, descriptive title
   - Fill out the PR template
   - Reference related issues
   - Request reviews from maintainers

5. **Address review feedback**:
   - Make requested changes
   - Push additional commits
   - Re-request review when ready

6. **Merge**:
   - PRs require at least one approval
   - CI must pass
   - Squash and merge is preferred

## Questions?

If you have questions, feel free to:

- Open a [Discussion](https://github.com/AstroAir/cpp-quick-starter/discussions)
- Ask in an issue

Thank you for contributing! 🎉
