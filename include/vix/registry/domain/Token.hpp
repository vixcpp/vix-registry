#pragma once

#include <cstdint>
#include <optional>
#include <string>
#include <utility>

namespace vix::registry::domain
{
    class Token
    {
    public:
        struct Builder;

    private:
        std::uint64_t id_{0};
        std::uint64_t userId_{0};

        std::string label_;
        std::string tokenHash_;
        std::optional<std::string> scopes_;
        bool revoked_{false};

        std::optional<std::string> createdAt_;

    public:
        Token() = default;

        std::uint64_t id() const noexcept { return id_; }
        std::uint64_t userId() const noexcept { return userId_; }
        const std::string &label() const noexcept { return label_; }
        const std::string &hash() const noexcept { return tokenHash_; }
        const std::optional<std::string> &scopes() const noexcept { return scopes_; }
        bool revoked() const noexcept { return revoked_; }
        const std::optional<std::string> &createdAt() const noexcept { return createdAt_; }

        void setId(std::uint64_t v) { id_ = v; }
        void setUserId(std::uint64_t v) { userId_ = v; }
        void setLabel(std::string v) { label_ = std::move(v); }
        void setHash(std::string v) { tokenHash_ = std::move(v); }
        void setScopes(std::string v) { scopes_ = std::move(v); }
        void setRevoked(bool v) { revoked_ = v; }
        void setCreatedAt(std::string v) { createdAt_ = std::move(v); }
    };

    struct Token::Builder
    {
        Token t;

        Builder &id(std::uint64_t v)
        {
            t.id_ = v;
            return *this;
        }
        Builder &userId(std::uint64_t v)
        {
            t.userId_ = v;
            return *this;
        }
        Builder &label(std::string v)
        {
            t.label_ = std::move(v);
            return *this;
        }
        Builder &hash(std::string v)
        {
            t.tokenHash_ = std::move(v);
            return *this;
        }
        Builder &scopes(std::string v)
        {
            t.scopes_ = std::move(v);
            return *this;
        }
        Builder &revoked(bool v)
        {
            t.revoked_ = v;
            return *this;
        }
        Builder &createdAt(std::string v)
        {
            t.createdAt_ = std::move(v);
            return *this;
        }

        Token build() { return t; }
    };
} // namespace vix::registry::domain
