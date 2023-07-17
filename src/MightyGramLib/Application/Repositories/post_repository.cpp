#include "post_repository.hpp"

namespace MightyBlocks
{
    PostRepository::PostRepository(std::shared_ptr<Database> database)
        : database_(std::move(database))
    {
    }

    pplx::task<Post> PostRepository::Create(Post& object)
    {
        constexpr static auto k_CreateStatement
            = "INSERT INTO posts (description, image_url, owner_id, parent_id) VALUES ($1, $2, $3, $4) RETURNING id, created_at";

        return SafeWrap<Post>([=]() {
            Post newAddedPost = object;

            if (object.GetParentId() > 0)
            {
                const auto query = database_->Execute(k_CreateStatement,
                    object.GetDescription(),
                    object.GetImageUrl(),
                    object.GetUserId(),
                    object.GetParentId());
                const auto data  = query.at(0);

                newAddedPost.SetId(data["id"].as<uint64_t>());
                newAddedPost.SetCreatedAt(data["created_at"].as<uint64_t>());
            }
            else
            {
                const auto query = database_->Execute(k_CreateStatement,
                    object.GetDescription(),
                    object.GetImageUrl(),
                    object.GetUserId(),
                    tao::pq::null);
                const auto data  = query.at(0);

                newAddedPost.SetId(data["id"].as<uint64_t>());
                newAddedPost.SetCreatedAt(data["created_at"].as<uint64_t>());
            }
            return newAddedPost;
        });
    }

    pplx::task<bool> PostRepository::Update(Post& object)
    {
        constexpr static auto k_UpdateStatement
            = "UPDATE posts SET description=$1, image_url=$2, owner_id=$3, parent_id=$4 WHERE id=$5";

        return SafeWrap<bool>([=]() {
            if (object.GetParentId() > 0)
            {
                database_->Execute(k_UpdateStatement,
                    object.GetDescription(),
                    object.GetImageUrl(),
                    object.GetUserId(),
                    object.GetParentId(),
                    object.GetId());
            }
            else
            {
                database_->Execute(k_UpdateStatement,
                    object.GetDescription(),
                    object.GetImageUrl(),
                    object.GetUserId(),
                    tao::pq::null,
                    object.GetId());
            }
            return true;
        });
    }

    pplx::task<Post> PostRepository::Read(uint64_t id)
    {
        constexpr static auto k_SelectStatement = "SELECT child.*, "
                                                  "parent.created_at  AS parent_created_at, "
                                                  "parent.description AS parent_description, "
                                                  "parent.image_url   AS parent_image_url, "
                                                  "parent.owner_id    AS parent_owner_id "
                                                  "FROM posts AS child LEFT JOIN posts AS parent ON parent.id = child.parent_id WHERE child.id = $1";

        return SafeWrap<Post>([=]() {
            const auto query = database_->Execute(k_SelectStatement, id);
            const auto data  = query.at(0);
            return Serialize(data);
        });
    }

    pplx::task<bool> PostRepository::Delete(uint64_t id)
    {
        constexpr static auto k_DeleteStatement = "DELETE from posts WHERE id = $1";

        return SafeWrap<bool>([=]() {
            database_->Execute(k_DeleteStatement, id);
            return true;
        });
    }

    pplx::task<uint64_t> PostRepository::CountPosts()
    {
        constexpr static auto k_CountStatement = "SELECT COUNT(*) FROM posts";

        return SafeWrap<uint64_t>([=]() {
            const auto query = database_->Execute(k_CountStatement);
            return query.at(0)["count"].as<uint64_t>();
        });
    }

    pplx::task<std::vector<Post>> PostRepository::ReadAll(int64_t page, int64_t size)
    {
        constexpr static auto k_SelectStatement = "SELECT child.*, "
                                                  "parent.created_at  AS parent_created_at, "
                                                  "parent.description AS parent_description, "
                                                  "parent.image_url   AS parent_image_url, "
                                                  "parent.owner_id    AS parent_owner_id "
                                                  "FROM posts AS child LEFT JOIN posts AS parent ON parent.id = child.parent_id "
                                                  "ORDER BY created_at DESC LIMIT $1 OFFSET $2";

        return SafeWrap<std::vector<Post>>([=]() {
            std::vector<Post> result;

            const auto query = database_->Execute(k_SelectStatement, size, (page - 1) * size);

            for (const auto& data : query)
            {
                result.emplace_back(Serialize(data));
            }
            return result;
        });
    }

    Post PostRepository::Serialize(const tao::pq::row& data)
    {
        Post result;
        result.SetId(data["id"].as<uint64_t>());
        result.SetCreatedAt(data["created_at"].as<uint64_t>());
        result.SetDescription(data["description"].as<std::string>());
        result.SetImageUrl(data["image_url"].as<std::string>());
        result.SetUserId(data["owner_id"].as<uint64_t>());

        if (!data["parent_id"].is_null())
        {
            const std::shared_ptr<Post> parent = std::make_shared<Post>(); // TODO remove this heap allocation
            parent->SetId(data["parent_id"].as<uint64_t>());
            parent->SetCreatedAt(data["parent_created_at"].as<uint64_t>());
            parent->SetDescription(data["parent_description"].as<std::string>());
            parent->SetImageUrl(data["parent_image_url"].as<std::string>());
            parent->SetUserId(data["parent_owner_id"].as<uint64_t>());

            result.SetParent(parent);
        }

        return result;
    }
}