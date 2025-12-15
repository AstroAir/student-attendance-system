# Security Policy

## Supported Versions

| Version | Supported          |
| ------- | ------------------ |
| 0.1.x   | :white_check_mark: |

## Reporting a Vulnerability

We take security vulnerabilities seriously. If you discover a security issue, please report it responsibly.

### How to Report

1. **Do NOT** open a public issue for security vulnerabilities
2. Send a detailed report to the project maintainers via:
   - GitHub Security Advisories (preferred)
   - Direct email to maintainers

### What to Include

- Description of the vulnerability
- Steps to reproduce
- Potential impact
- Suggested fix (if any)

### Response Timeline

- **Initial Response**: Within 48 hours
- **Status Update**: Within 7 days
- **Resolution Target**: Within 30 days (depending on severity)

### After Reporting

1. We will acknowledge receipt of your report
2. We will investigate and validate the issue
3. We will work on a fix and coordinate disclosure
4. We will credit you in the security advisory (unless you prefer anonymity)

## Security Best Practices

When using this template for your projects:

- Keep dependencies up to date
- Enable compiler warnings and treat them as errors in CI
- Use sanitizers (ASan, UBSan, TSan) during development
- Run static analysis tools (clang-tidy, cppcheck)
- Review code changes carefully
- Follow secure coding guidelines

## Disclosure Policy

We follow a coordinated disclosure process:

1. Security issues are fixed in private
2. A new release is prepared with the fix
3. Security advisory is published after the release
4. Credit is given to the reporter

Thank you for helping keep this project secure!
