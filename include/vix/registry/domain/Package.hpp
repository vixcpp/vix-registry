#pragma once

#include <cstdint>
#include <optional>
#include <string>
#include <utility>

namespace vix::registry::domain
{
    enum class PackageVisibility
    {
        Public,
        Private
    };

    class Package
    {
    public:
        struct Builder;

    private:
        std::uint64_t id_{0};
        std::uint64_t ownerUserId_{0};
        std::string name_;
        std::optional<std::string> description_;
        PackageVisibility visibility_{PackageVisibility::Public};
        std::optional<std::string> createdAt_;
        std::optional<std::string> updatedAt_;

    public:
        Package() = default;

        std::uint64_t id() const noexcept { return id_; }
        std::uint64_t ownerUserId() const noexcept { return ownerUserId_; }
        const std::string &name() const noexcept { return name_; }
        const std::optional<std::string> &description() const noexcept { return description_; }
        PackageVisibility visibility() const noexcept { return visibility_; }
        const std::optional<std::string> &createdAt() const noexcept { return createdAt_; }
        const std::optional<std::string> &updatedAt() const noexcept { return updatedAt_; }

        void setId(std::uint64_t v) { id_ = v; }
        void setOwnerUserId(std::uint64_t v) { ownerUserId_ = v; }
        void setName(std::string v) { name_ = std::move(v); }
        void setDescription(std::string v) { description_ = std::move(v); }
        void setVisibility(PackageVisibility v) { visibility_ = v; }
        void setCreatedAt(std::string v) { createdAt_ = std::move(v); }
        void setUpdatedAt(std::string v) { updatedAt_ = std::move(v); }
    };

    struct Package::Builder
    {
        Package p;

        Builder &id(std::uint64_t v)
        {
            p.id_ = v;
            return *this;
        }
        Builder &ownerUserId(std::uint64_t v)
        {
            p.ownerUserId_ = v;
            return *this;
        }
        Builder &name(std::string v)
        {
            p.name_ = std::move(v);
            return *this;
        }
        Builder &description(std::string v)
        {
            p.description_ = std::move(v);
            return *this;
        }
        Builder &visibility(PackageVisibility v)
        {
            p.visibility_ = v;
            return *this;
        }
        Builder &createdAt(std::string v)
        {
            p.createdAt_ = std::move(v);
            return *this;
        }
        Builder &updatedAt(std::string v)
        {
            p.updatedAt_ = std::move(v);
            return *this;
        }

        Package build() { return p; }
    };

    inline std::string to_string(PackageVisibility v)
    {
        return (v == PackageVisibility::Private) ? "private" : "public";
    }
} // namespace vix::registry::domain
