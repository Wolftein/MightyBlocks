
#pragma once

#include <string>

namespace MightyBlocks
{
    class Like final
    {
    public:

        Like() : id_(0), userId_(0), postId_(0)
        {
        }

        Like(const Like& other) = default;

        void SetId(uint64_t id)
        {
            id_ = id;
        }

        void SetUserId(uint64_t userId)
        {
            userId_ = userId;
        }

        void SetPostId(uint64_t postId)
        {
            postId_ = postId;
        }

        [[nodiscard]] uint64_t GetId() const
        {
            return id_;
        }

        [[nodiscard]] uint64_t GetUserId() const
        {
            return userId_;
        }

        [[nodiscard]] uint64_t GetPostId() const
        {
            return postId_;
        }

    private:

        uint64_t id_;
        uint64_t userId_;
        uint64_t postId_;
    };
}