#pragma once

#include <cstdint>
#include <optional>
#include <string>
#include <utility>

namespace vix::registry::domain
{
    class Version
    {
    public:
        struct Builder;

    private:
        std::uint64_t id_{0};
        std::uint64_t packageId_{0};

        std::string semver_;
        std::string artifactPath_;
        std::string sha256_;
        std::uint64_t sizeBytes_{0};

        bool yanked_{false};
        std::optional<std::string> createdAt_;

    public:
        Version() = default;

        std::uint64_t id() const noexcept { return id_; }
        std::uint64_t packageId() const noexcept { return packageId_; }
        const std::string &semver() const noexcept { return semver_; }
        const std::string &artifactPath() const noexcept { return artifactPath_; }
        const std::string &sha256() const noexcept { return sha256_; }
        std::uint64_t sizeBytes() const noexcept { return sizeBytes_; }
        bool yanked() const noexcept { return yanked_; }
        const std::optional<std::string> &createdAt() const noexcept { return createdAt_; }

        void setId(std::uint64_t v) { id_ = v; }
        void setPackageId(std::uint64_t v) { packageId_ = v; }
        void setSemver(std::string v) { semver_ = std::move(v); }
        void setArtifactPath(std::string v) { artifactPath_ = std::move(v); }
        void setSha256(std::string v) { sha256_ = std::move(v); }
        void setSizeBytes(std::uint64_t v) { sizeBytes_ = v; }
        void setYanked(bool v) { yanked_ = v; }
        void setCreatedAt(std::string v) { createdAt_ = std::move(v); }
    };

    struct Version::Builder
    {
        Version v;

        Builder &id(std::uint64_t x)
        {
            v.id_ = x;
            return *this;
        }
        Builder &packageId(std::uint64_t x)
        {
            v.packageId_ = x;
            return *this;
        }
        Builder &semver(std::string x)
        {
            v.semver_ = std::move(x);
            return *this;
        }
        Builder &artifactPath(std::string x)
        {
            v.artifactPath_ = std::move(x);
            return *this;
        }
        Builder &sha256(std::string x)
        {
            v.sha256_ = std::move(x);
            return *this;
        }
        Builder &sizeBytes(std::uint64_t x)
        {
            v.sizeBytes_ = x;
            return *this;
        }
        Builder &yanked(bool x)
        {
            v.yanked_ = x;
            return *this;
        }
        Builder &createdAt(std::string x)
        {
            v.createdAt_ = std::move(x);
            return *this;
        }

        Version build() { return v; }
    };
} // namespace vix::registry::domain
