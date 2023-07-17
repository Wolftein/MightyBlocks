#pragma once

#include "Foundation/Database/database.hpp"
#include "Foundation/Database/repository.hpp"
#include "Application/Model/post.hpp"

namespace MightyBlocks
{
    /**
     * \see Repository implementation for \see Post.
     */
    class PostRepository final : public Repository<Post>
    {
    public:

        explicit PostRepository(std::shared_ptr<Database> database);

        pplx::task<Post> Create(Post& object) override;
        pplx::task<bool> Update(Post& object) override;
        pplx::task<Post> Read(uint64_t id) override;
        pplx::task<bool> Delete(uint64_t id) override;

        pplx::task<uint64_t> CountPosts();
        pplx::task<std::vector<Post>> ReadAll(int64_t page, int64_t size);

    private:

        static Post Serialize(const tao::pq::row& row);

    private:

        std::shared_ptr<Database> database_;
    };
}