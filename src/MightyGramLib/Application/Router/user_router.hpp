#pragma once

#include "Foundation/Network/router.hpp"
#include "Application/Repositories/post_repository.hpp"
#include "Application/Repositories/user_repository.hpp"

namespace MightyBlocks
{
    class UserRouter final : public Router
    {
    public:

        UserRouter(std::shared_ptr<UserRepository> userRepository);

        json::value Route(const http::method& type, const std::string& path, const json::value& input) override;

    private:

        json::value GetById(const json::value& input);

        json::value Parenthood(const json::value& input);

    private:

        std::shared_ptr<UserRepository> userRepository_;
        std::shared_ptr<PostRepository> postRepository_;
    };
}