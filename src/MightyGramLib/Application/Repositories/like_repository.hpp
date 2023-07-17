#pragma once

#include "Foundation/Database/database.hpp"
#include "Foundation/Database/repository.hpp"
#include "Application/Model/like.hpp"

namespace MightyBlocks
{
    /**
     * \see Repository implementation for \see Like.
     */
    class LikeRepository final : public Repository<Like>
    {
    public:

        explicit LikeRepository(std::shared_ptr<Database> database);

        pplx::task<Like> Create(Like& object) override;
        pplx::task<bool> Update(Like& object) override;
        pplx::task<Like> Read(uint64_t id) override;
        pplx::task<bool> Delete(uint64_t id) override;

        pplx::task<uint64_t> CountLikesByPostId(uint64_t postId);
        pplx::task<bool> ExistByPostIdAndUserId(uint64_t postId, uint64_t userId);
        pplx::task<bool> DeleteByPostIdAndUserId(uint64_t postId, uint64_t userId);

    private:

        std::shared_ptr<Database> database_;
    };
}