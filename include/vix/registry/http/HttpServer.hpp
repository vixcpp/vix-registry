#pragma once

#include <cstdint>
#include <memory>
#include <vix.hpp>
#include <vix/registry/db/Database.hpp>

namespace vix::registry::http
{
    class HttpServer
    {
    public:
        HttpServer(std::uint16_t port,
                   std::shared_ptr<vix::registry::db::Database> db);

        void run();
        vix::App &app() { return app_; }

    private:
        void setupRoutes();
        void initRoutes();

        std::uint16_t port_{8080};
        vix::App app_;

        std::shared_ptr<vix::registry::db::Database> db_;
        bool routesInitialized_{false};
    };
}
