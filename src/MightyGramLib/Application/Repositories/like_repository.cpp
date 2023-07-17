#include "like_repository.hpp"

namespace MightyBlocks
{
    LikeRepository::LikeRepository(std::shared_ptr<Database> database)
        : database_(std::move(database))
    {
    }

    pplx::task<Like> LikeRepository::Create(Like& object)
    {
        constexpr static auto k_CreateStatement
            = "INSERT INTO likes (user_id, post_id) VALUES ($1, $2) RETURNING id";

        return SafeWrap<Like>([=]() {
            const auto query = database_->Execute(k_CreateStatement, object.GetUserId(), object.GetPostId());

            Like newAddedLike = object;
            newAddedLike.SetId(query.at(0)["id"].as<uint64_t>());
            return newAddedLike;
        });
    }

    pplx::task<bool> LikeRepository::Update(Like& object)
    {
        constexpr static auto k_UpdateStatement
            = "UPDATE likes SET user_id=$1, post_id=$2 WHERE id=$3";

        return SafeWrap<bool>([=]() {
            database_->Execute(k_UpdateStatement, object.GetUserId(), object.GetPostId(), object.GetId());
            return true;
        });
    }

    pplx::task<Like> LikeRepository::Read(uint64_t id)
    {
        constexpr static auto k_SelectStatement = "SELECT * FROM likes WHERE id=$1";

        return SafeWrap<Like>([=]() {
            const auto query = database_->Execute(k_SelectStatement, id);
            const auto data  = query.at(0);

            Like result;
            result.SetId(id);
            result.SetUserId(data["user_id"].as<uint64_t>());
            result.SetPostId(data["post_id"].as<uint64_t>());

            return result;
        });
    }

    pplx::task<bool> LikeRepository::Delete(uint64_t id)
    {
        constexpr static auto k_DeleteStatement = "DELETE from likes WHERE id = $1";

        return SafeWrap<bool>([=]() {
            database_->Execute(k_DeleteStatement, id);
            return true;
        });
    }

    pplx::task<uint64_t> LikeRepository::CountLikesByPostId(uint64_t postId)
    {
        constexpr static auto k_CountStatement = "SELECT COUNT(*) FROM likes WHERE post_id = $1";

        return SafeWrap<uint64_t>([=]() {
            const auto query = database_->Execute(k_CountStatement, postId);
            return query.at(0)["count"].as<uint64_t>();
        });
    }

    pplx::task<bool> LikeRepository::ExistByPostIdAndUserId(uint64_t postId, uint64_t userId)
    {
        constexpr static auto k_ExistStatement = "SELECT 1 FROM likes WHERE post_id = $1 and user_id = $2";

        return SafeWrap<bool>([=]() {
            const auto query = database_->Execute(k_ExistStatement, postId, userId);
            return !query.empty();
        });
    }

    pplx::task<bool> LikeRepository::DeleteByPostIdAndUserId(uint64_t postId, uint64_t userId)
    {
        constexpr static auto k_DeleteStatement = "DELETE from likes WHERE post_id = $1 AND user_id = $2";

        return SafeWrap<bool>([=]() {
            database_->Execute(k_DeleteStatement, postId, userId);
            return true;
        });
    }

}