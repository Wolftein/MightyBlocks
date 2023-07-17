#include "user_repository.hpp"

namespace MightyBlocks
{
    UserRepository::UserRepository(std::shared_ptr<Database> database)
        : database_(std::move(database))
    {
    }

    pplx::task<User> UserRepository::Create(User& object)
    {
        constexpr static auto k_CreateStatement
            = "INSERT INTO users (name, parent_id) VALUES ($1, $2) RETURNING id, created_at";

        return SafeWrap<User>([=]() {
            User newAddedUser = object;

            if (object.GetParent())
            {
                const auto query = database_->Execute(k_CreateStatement, object.GetName(), object.GetParent()->GetId());
                const auto data  = query.at(0);

                newAddedUser.SetId(data["id"].as<uint64_t>());
                newAddedUser.SetCreatedAt(data["created_at"].as<uint64_t>());
            }
            else
            {
                const auto query = database_->Execute(k_CreateStatement, object.GetName(), tao::pq::null);
                const auto data  = query.at(0);

                newAddedUser.SetId(data["id"].as<uint64_t>());
                newAddedUser.SetCreatedAt(data["created_at"].as<uint64_t>());
            }

            return newAddedUser;
        });
    }

    pplx::task<bool> UserRepository::Update(User& object)
    {
        constexpr static auto k_UpdateStatement
            = "UPDATE users SET name=$1, parent_id=$2 WHERE id=$3";

        return SafeWrap<bool>([=]() {
            if (object.GetParent())
            {
                database_->Execute(k_UpdateStatement, object.GetName(), object.GetParent()->GetId(), object.GetId());
            }
            else
            {
                database_->Execute(k_UpdateStatement, object.GetName(), tao::pq::null, object.GetId());
            }
            return true;
        });
    }

    pplx::task<User> UserRepository::Read(uint64_t id)
    {
        constexpr static auto k_SelectStatement = "SELECT child.*, "
                                                  "parent.name AS parent_name, "
                                                  "parent.created_at AS parent_created_at "
                                                  "FROM users AS child LEFT JOIN users AS parent ON parent.id = child.parent_id WHERE child.id = $1";

        return SafeWrap<User>([=]() {
            const auto query = database_->Execute(k_SelectStatement, id);
            const auto data  = query.at(0);

            return Serialize(data);
        });
    }

    pplx::task<bool> UserRepository::Delete(uint64_t id)
    {
        constexpr static auto k_DeleteStatement = "DELETE from users WHERE id = $1";

        return SafeWrap<bool>([=]() {
            database_->Execute(k_DeleteStatement, id);
            return true;
        });
    }

    pplx::task<std::vector<uint64_t>> UserRepository::GetChildren(uint64_t userParentId)
    {
        constexpr static auto k_SelectStatement = "SELECT id FROM users WHERE parent_id = $1";

        return SafeWrap<std::vector<uint64_t>>([=]() {
            std::vector<uint64_t> result;

            const auto query = database_->Execute(k_SelectStatement, userParentId);

            for (const auto& data : query)
            {
                result.emplace_back(data["id"].as<uint64_t>());
            }
            return result;
        });
    }

    User UserRepository::Serialize(const tao::pq::row& data)
    {
        User result;

        result.SetId(data["id"].as<uint64_t>());
        result.SetCreatedAt(data["created_at"].as<uint64_t>());
        result.SetName(data["name"].as<std::string>());

        if (!data["parent_id"].is_null())
        {
            const std::shared_ptr<User> parent = std::make_shared<User>(); // TODO remove this heap allocation
            parent->SetId(data["parent_id"].as<uint64_t>());
            parent->SetCreatedAt(data["parent_created_at"].as<uint64_t>());
            parent->SetName(data["parent_name"].as<std::string>());
            result.SetParent(parent);
        }
        return result;
    }
}