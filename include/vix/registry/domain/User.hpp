#pragma once

#include <cstdint>
#include <optional>
#include <string>
#include <utility>

namespace vix::registry::domain
{
    class User
    {
    public:
        struct Builder;

    private:
        std::uint64_t id_{0};
        std::string username_;
        std::string email_;
        std::string passwordHash_;
        std::optional<std::string> createdAt_;
        std::optional<std::string> updatedAt_;

    public:
        User() = default;

        std::uint64_t id() const noexcept { return id_; }
        const std::string &username() const noexcept { return username_; }
        const std::string &email() const noexcept { return email_; }
        const std::string &passwordHash() const noexcept { return passwordHash_; }
        const std::optional<std::string> &createdAt() const noexcept { return createdAt_; }
        const std::optional<std::string> &updatedAt() const noexcept { return updatedAt_; }

        void setId(std::uint64_t v) { id_ = v; }
        void setUsername(std::string v) { username_ = std::move(v); }
        void setEmail(std::string v) { email_ = std::move(v); }
        void setPasswordHash(std::string v) { passwordHash_ = std::move(v); }
        void setCreatedAt(std::string v) { createdAt_ = std::move(v); }
        void setUpdatedAt(std::string v) { updatedAt_ = std::move(v); }
    };

    struct User::Builder
    {
        User u;

        Builder &id(std::uint64_t v)
        {
            u.id_ = v;
            return *this;
        }
        Builder &username(std::string v)
        {
            u.username_ = std::move(v);
            return *this;
        }
        Builder &email(std::string v)
        {
            u.email_ = std::move(v);
            return *this;
        }
        Builder &passwordHash(std::string v)
        {
            u.passwordHash_ = std::move(v);
            return *this;
        }
        Builder &createdAt(std::string v)
        {
            u.createdAt_ = std::move(v);
            return *this;
        }
        Builder &updatedAt(std::string v)
        {
            u.updatedAt_ = std::move(v);
            return *this;
        }

        User build() { return u; }
    };
} // namespace vix::registry::domain
