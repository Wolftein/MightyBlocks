#include "main_environment.hpp"

void TestEnvironment::SetUp()
{
    const char* dbConnectionInfo = "postgres://postgres:postgres@127.0.0.1:5432/postgres";
    if (const char* env = std::getenv("MG_DB_CONNECTION_STRING"))
    {
        dbConnectionInfo = env;
    }

    database_       = std::make_shared<MightyBlocks::Database>(dbConnectionInfo);
    likeRepository_ = std::make_shared<MightyBlocks::LikeRepository>(database_);
    postRepository_ = std::make_shared<MightyBlocks::PostRepository>(database_);
    userRepository_ = std::make_shared<MightyBlocks::UserRepository>(database_);
}

void TestEnvironment::TearDown()
{
    database_       = nullptr;
    likeRepository_ = nullptr;
    postRepository_ = nullptr;
    userRepository_ = nullptr;
}
