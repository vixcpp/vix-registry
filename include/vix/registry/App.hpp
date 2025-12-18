#pragma once

#include <memory>
#include <cstdint>

#include <vix/config/Config.hpp>
#include <vix/registry/http/HttpServer.hpp>
#include <vix/registry/db/Database.hpp>

namespace vix::registry
{
    class App
    {
    public:
        App();
        ~App();

        int run();

    private:
        static const char *resolveConfigPath();
        static std::shared_ptr<db::Database> initDatabase(const vix::config::Config &cfg);

    private:
        vix::config::Config config_;
        std::uint16_t port_{8080};

        std::shared_ptr<db::Database> db_;
        std::unique_ptr<http::HttpServer> server_;
    };
}
