#include <gtest/gtest.h>
#include <vix/registry/db/Database.hpp>

TEST(Database, CanConnectFromEnv)
{
    auto db = vix::registry::db::Database::fromEnvShared("REGISTRY_DB_");
    ASSERT_NE(db, nullptr);

    EXPECT_NO_THROW({
        db->testConnection();
    });
}
