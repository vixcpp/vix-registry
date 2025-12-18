#pragma once

#include <cstddef>
#include <string>

#include <vix/orm/ConnectionPool.hpp>
#include <vix/orm/Transaction.hpp>
#include <vix/orm/UnitOfWork.hpp>

namespace vix::registry::db
{
    struct DatabaseConfig
    {
        std::string host;
        std::string user;
        std::string password;
        std::string database;

        std::size_t poolMin = 1;
        std::size_t poolMax = 8;
    };

    class Database
    {
    public:
        using Pool = vix::orm::ConnectionPool;
        using PoolConfig = vix::orm::PoolConfig;
        using Transaction = vix::orm::Transaction;
        using UnitOfWork = vix::orm::UnitOfWork;

    private:
        DatabaseConfig config_;
        Pool pool_;

    public:
        explicit Database(const DatabaseConfig &config);
        Pool &pool() noexcept { return pool_; }
        const Pool &pool() const noexcept { return pool_; }
        DatabaseConfig config() const { return config_; }
        UnitOfWork makeUnitOfWork();
        Transaction makeTransaction();
        void testConnection();
        static DatabaseConfig loadFromEnv(const std::string &prefix = "REGISTRY_DB_");
        static std::shared_ptr<Database> fromEnvShared(const std::string &prefix = "REGISTRY_DB_")
        {
            return std::make_shared<Database>(loadFromEnv(prefix));
        }
    };

} // namespace softadastra::registry::db
