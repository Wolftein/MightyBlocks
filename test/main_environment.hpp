#pragma once

#include <gtest/gtest.h>

#include <Application/Repositories/like_repository.hpp>
#include <Application/Repositories/post_repository.hpp>
#include <Application/Repositories/user_repository.hpp>

constexpr static inline auto TEST_USER      = 4;
constexpr static inline auto TEST_USER_NAME = "Testing";

class TestEnvironment final : public ::testing::Environment
{
public:

    void SetUp() override;
    void TearDown() override;

    [[nodiscard]] auto GetDatabase()
    {
        return database_;
    }

    [[nodiscard]] auto GetLikeRepository()
    {
        return likeRepository_;
    }

    [[nodiscard]] auto GetPostRepository()
    {
        return postRepository_;
    }

    [[nodiscard]] auto GetUserRepository()
    {
        return userRepository_;
    }

private:

    std::shared_ptr<MightyBlocks::Database>       database_;
    std::shared_ptr<MightyBlocks::LikeRepository> likeRepository_;
    std::shared_ptr<MightyBlocks::PostRepository> postRepository_;
    std::shared_ptr<MightyBlocks::UserRepository> userRepository_;
};

extern TestEnvironment* gEnvironment;