#pragma once

#include "Foundation/Network/router.hpp"
#include "Foundation/Filesystem/filesystem.hpp"
#include "Application/Repositories/like_repository.hpp"
#include "Application/Repositories/post_repository.hpp"
#include "Application/Repositories/user_repository.hpp"

namespace MightyBlocks
{
    class PostRouter final : public Router
    {
    public:

        PostRouter(std::shared_ptr<Filesystem> filesystem,
                   std::shared_ptr<LikeRepository> likeRepository,
                   std::shared_ptr<PostRepository> postRepository,
                   std::shared_ptr<UserRepository> userRepository);

        json::value Route(const http::method& type, const std::string& path, const json::value& input) override;

    private:

        json::value Create(const json::value& input);
        json::value Get(const json::value& input);
        json::value GetAll(const json::value& input);
        json::value Like(const json::value& input);
        json::value Dislike(const json::value& input);

    private:

        std::shared_ptr<Filesystem>     filesystem_;
        std::shared_ptr<LikeRepository> likeRepository_;
        std::shared_ptr<PostRepository> postRepository_;
        std::shared_ptr<UserRepository> userRepository_;
    };
}