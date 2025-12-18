#pragma once

#include <stdexcept>
#include <string>

namespace vix::registry::domain
{
    class RegistryError : public std::runtime_error
    {
    public:
        explicit RegistryError(const std::string &msg) : std::runtime_error(msg) {}
    };

    class ValidationError : public RegistryError
    {
    public:
        explicit ValidationError(const std::string &msg) : RegistryError(msg) {}
    };

    class NotFoundError : public RegistryError
    {
    public:
        explicit NotFoundError(const std::string &msg) : RegistryError(msg) {}
    };

    class AuthError : public RegistryError
    {
    public:
        explicit AuthError(const std::string &msg) : RegistryError(msg) {}
    };

    class DbError : public RegistryError
    {
    public:
        explicit DbError(const std::string &msg) : RegistryError(msg) {}
    };
} // namespace vix::registry::domain
