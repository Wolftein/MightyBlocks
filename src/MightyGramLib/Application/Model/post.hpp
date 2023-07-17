
#pragma once

#include <string>

namespace MightyBlocks
{
    class Post final
    {
    public:

        Post() : id_(0), userId_(0), createdAt_(0), parentId_(0), parent_(nullptr)
        {
        }

        Post(const Post& other) = default;

        void SetId(uint64_t id)
        {
            id_ = id;
        }

        void SetUserId(uint64_t userId)
        {
            userId_ = userId;
        }

        void SetCreatedAt(uint64_t createdAt)
        {
            createdAt_ = createdAt;
        }

        void SetParentId(uint64_t parentId)
        {
            parentId_ = parentId;
        }

        void SetImageUrl(const std::string& imageUrl)
        {
            imageUrl_ = imageUrl;
        }

        void SetDescription(const std::string& description)
        {
            description_ = description;
        }

        void SetParent(const std::shared_ptr<Post>& parent)
        {
            parent_ = parent;
        }

        [[nodiscard]] uint64_t GetId() const
        {
            return id_;
        }

        [[nodiscard]] uint64_t GetUserId() const
        {
            return userId_;
        }

        [[nodiscard]] uint64_t GetCreatedAt() const
        {
            return createdAt_;
        }

        [[nodiscard]] uint64_t GetParentId() const
        {
            return parentId_;
        }

        [[nodiscard]] const std::string& GetImageUrl() const
        {
            return imageUrl_;
        }

        [[nodiscard]] const std::string& GetDescription() const
        {
            return description_;
        }

        [[nodiscard]] const std::shared_ptr<Post>& GetParent() const
        {
            return parent_;
        }

    private:

        uint64_t    id_;
        uint64_t    userId_;
        uint64_t    createdAt_;
        uint64_t    parentId_;
        std::string imageUrl_;
        std::string description_;
        std::shared_ptr<Post> parent_;  // NOTE: Maybe use a memory pool allocator?
    };
}