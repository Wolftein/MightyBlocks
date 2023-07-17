#include "service.hpp"

#include "Application/Repositories/like_repository.hpp"
#include "Application/Repositories/post_repository.hpp"
#include "Application/Repositories/user_repository.hpp"

#include "Foundation/Filesystem/Detail/raw_filesystem.hpp"

#include "Application/Router/post_router.hpp"
#include "Application/Router/user_router.hpp"

namespace MightyBlocks
{
    void Service::Start(const std::string& serviceInfo, const std::string& databaseInfo)
    {
        // Create all systems...
        const auto database   = std::make_shared<Database>(databaseInfo);
        const auto filesystem = std::make_shared<RawFilesystem>("images/");

        // Create all repositories...
        const auto likeRepository = std::make_shared<LikeRepository>(database);
        const auto postRepository = std::make_shared<PostRepository>(database);
        const auto userRepository = std::make_shared<UserRepository>(database);

        // Create controller and register all routers...
        controller_ = std::make_unique<Controller>();
        controller_->Register<PostRouter>("post", filesystem, likeRepository, postRepository, userRepository);
        controller_->Register<UserRouter>("user", userRepository);

        controller_->Start(serviceInfo).wait();
        std::cout << "Listening for requests at: " << controller_->GetEndpoint() << std::endl;
    }

    void Service::Stop()
    {
        controller_->Shutdown().wait();
        controller_ = nullptr;
    }
}