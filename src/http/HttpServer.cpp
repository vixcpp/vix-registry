#include <vix/registry/http/HttpServer.hpp>

namespace vix::registry::http
{
    HttpServer::HttpServer(std::uint16_t port,
                           std::shared_ptr<vix::registry::db::Database> db)
        : port_(port), db_(std::move(db))
    {
    }

    void HttpServer::setupRoutes()
    {
        app_.get("/health", [](auto &, auto &res)
                 { res.json({"status", "ok"}); });

        app_.get("/", [](auto &, auto &res)
                 { res.json({"message", "Vix Registry is running"}); });

        app_.get("/health/db", [this](auto &, auto &res)
                 {
            try {
                if (!db_) throw std::runtime_error("Database not configured");
                db_->testConnection();
                res.json({"db", "ok"});
            } catch (const std::exception &e) {
                res.status(500).json(vix::json::kv({{"db", "error"}, {"message", e.what()}}));
            } });
    }

    void HttpServer::initRoutes()
    {
        if (routesInitialized_)
            return;
        setupRoutes();
        routesInitialized_ = true;
    }

    void HttpServer::run()
    {
        initRoutes();
        app_.run(port_);
    }
}
