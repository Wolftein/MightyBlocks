#pragma once

#include "Foundation/Database/database.hpp"
#include "Foundation/Database/repository.hpp"
#include "Application/Model/user.hpp"

namespace MightyBlocks
{
    /**
     * \see Repository implementation for \see User.
     */
    class UserRepository final : public Repository<User>
    {
    public:

        explicit UserRepository(std::shared_ptr<Database> database);

        pplx::task<User> Create(User& object) override;
        pplx::task<bool> Update(User& object) override;
        pplx::task<User> Read(uint64_t id) override;
        pplx::task<bool> Delete(uint64_t id) override;

        pplx::task<std::vector<uint64_t>> GetChildren(uint64_t userParentId);

    private:

        static User Serialize(const tao::pq::row& row);

    private:

        std::shared_ptr<Database> database_;
    };
}