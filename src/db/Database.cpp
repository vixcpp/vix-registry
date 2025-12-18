#include "vix/registry/db/Database.hpp"

#include <cstdlib>
#include <stdexcept>
#include <string>

#include <vix/orm/ConnectionPool.hpp>
#include <vix/orm/Errors.hpp>
#include <vix/orm/Drivers.hpp>
#include <vix/orm/Transaction.hpp>
#include <vix/orm/UnitOfWork.hpp>
#include <vix/orm/MySQLDriver.hpp>

namespace vix::registry::db
{
    namespace
    {
        std::string getEnvOrThrow(const std::string &key)
        {
            if (const char *val = std::getenv(key.c_str()))
                return std::string{val};
            throw std::runtime_error("Missing required environment variable: " + key);
        }

        std::string getEnvOrDefault(const std::string &key,
                                    const std::string &fallback)
        {
            if (const char *val = std::getenv(key.c_str()))
                return std::string{val};
            return fallback;
        }

        std::size_t getEnvSizeOrDefault(const std::string &key,
                                        std::size_t fallback)
        {
            if (const char *val = std::getenv(key.c_str()))
            {
                char *end = nullptr;
                auto v = std::strtoul(val, &end, 10);
                if (end != val)
                    return static_cast<std::size_t>(v);
            }
            return fallback;
        }
    } // namespace

    DatabaseConfig Database::loadFromEnv(const std::string &prefix)
    {
        DatabaseConfig cfg{};

        const std::string hostKey = prefix + "HOST";
        const std::string userKey = prefix + "USER";
        const std::string passKey = prefix + "PASSWORD";
        const std::string nameKey = prefix + "NAME";
        const std::string minKey = prefix + "POOL_MIN";
        const std::string maxKey = prefix + "POOL_MAX";

        cfg.host = getEnvOrThrow(hostKey);
        cfg.user = getEnvOrThrow(userKey);
        cfg.password = getEnvOrDefault(passKey, "");
        cfg.database = getEnvOrThrow(nameKey);

        cfg.poolMin = getEnvSizeOrDefault(minKey, 1);
        cfg.poolMax = getEnvSizeOrDefault(maxKey, 8);

        if (cfg.poolMin == 0)
            cfg.poolMin = 1;
        if (cfg.poolMax < cfg.poolMin)
            cfg.poolMax = cfg.poolMin;

        return cfg;
    }

    Database::Database(const DatabaseConfig &config)
        : config_(config),
          pool_(
              vix::orm::make_mysql_factory(
                  config.host,
                  config.user,
                  config.password,
                  config.database),
              vix::orm::PoolConfig{
                  .min = config.poolMin,
                  .max = config.poolMax})
    {
        pool_.warmup();
    }

    Database::UnitOfWork Database::makeUnitOfWork()
    {
        return UnitOfWork{pool_};
    }

    Database::Transaction Database::makeTransaction()
    {
        return Transaction{pool_};
    }

    void Database::testConnection()
    {
        vix::orm::PooledConn pc(pool_);
        auto &conn = pc.get();
        auto st = conn.prepare("SELECT 1");
        st->exec();
    }

} // namespace vix::registry::db
