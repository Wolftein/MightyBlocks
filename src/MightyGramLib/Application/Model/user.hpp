
#pragma once

#include <string>

namespace MightyBlocks
{
    class User final
    {
    public:

        User() : id_(0), createdAt_(0), parent_(nullptr)
        {
        }

        User(const User& other) = default;

        void SetId(uint64_t id)
        {
            id_ = id;
        }

        void SetCreatedAt(uint64_t createdAt)
        {
            createdAt_ = createdAt;
        }

        void SetName(const std::string& name)
        {
            name_ = name;
        }

        void SetParent(const std::shared_ptr<User>& parent)
        {
            parent_ = parent;
        }

        [[nodiscard]] uint64_t GetId() const
        {
            return id_;
        }

        [[nodiscard]] uint64_t GetCreatedAt() const
        {
            return createdAt_;
        }

        [[nodiscard]] const std::string& GetName() const
        {
            return name_;
        }

        [[nodiscard]] const std::shared_ptr<User>& GetParent() const
        {
            return parent_;
        }

    private:

        uint64_t              id_;
        uint64_t              createdAt_;
        std::string           name_;
        std::shared_ptr<User> parent_;  // NOTE: Maybe use a memory pool allocator?
    };
}