#include "post_router.hpp"

namespace MightyBlocks
{
    PostRouter::PostRouter(std::shared_ptr<Filesystem> filesystem,
                           std::shared_ptr<LikeRepository> likeRepository,
                           std::shared_ptr<PostRepository> postRepository,
                           std::shared_ptr<UserRepository> userRepository)
        : filesystem_(std::move(filesystem)),
          likeRepository_(std::move(likeRepository)),
          postRepository_(std::move(postRepository)),
          userRepository_(std::move(userRepository))
    {
    }

    json::value PostRouter::Route(const http::method& type, const std::string& path, const json::value& input)
    {
        if (type == http::methods::POST)
        {
            if (path == "new")
            {
                return Create(input);
            }
            else if (path == "like")
            {
                return Like(input);
            }
            else if (path == "dislike")
            {
                return Dislike(input);
            }
        }
        else if (type == http::methods::GET)
        {
            if (path == "id")
            {
                return Get(input);
            }
            else if (path == "all")
            {
                return GetAll(input);
            }
        }
        return MakeErrorResponse(kRestErrorCodeInvalidEndpoint, type + " is not supported for " + path);
    }

    json::value PostRouter::Create(const json::value& input)
    {
        if (!input.has_number_field("user_id"))
        {
            return MakeErrorResponse(kRestErrorCodeInvalidParam, "Invalid Parameters");
        }
        if (!input.has_string_field("image"))
        {
            return MakeErrorResponse(kRestErrorCodeInvalidParam, "Invalid Parameters");
        }
        if (!input.has_string_field("description"))
        {
            return MakeErrorResponse(kRestErrorCodeInvalidParam, "Invalid Parameters");
        }

        const auto  iUserId      = input.at("user_id").as_number().to_uint64();
        const auto& iImage       = input.at("image").as_string(); // Base64
        const auto& iDescription = input.at("description").as_string();

        if (iDescription.empty())
        {
            return MakeErrorResponse(1000, "Description must not be empty");
        }

        // NOTE: for the simplicity of this exercise, all images are PNG encoded.
        std::string iImageUrl;
        try
        {
            iImageUrl = filesystem_->Upload("png", utility::conversions::from_base64(iImage)).get(); // TODO async + continuation fiber
            if (iImageUrl.empty())
            {
                return MakeErrorResponse(1001, "Something went wrong trying to upload the image");
            }
        } catch (...)
        {
            return MakeErrorResponse(1002, "Image is not correctly encoded as Base64");
        }

        MightyBlocks::Post newPostObject;
        newPostObject.SetUserId(iUserId);
        newPostObject.SetDescription(iDescription);
        newPostObject.SetImageUrl(iImageUrl);

        if (const Post& newPost = postRepository_->Create(newPostObject).get(); newPost.GetId() != 0) // TODO async + continuation fiber
        {
            const std::vector<uint64_t> children = userRepository_->GetChildren(iUserId).get(); // TODO async + continuation fiber

            for (const uint64_t id : children)
            {
                // NOTE: Reuse object to prevent any memory allocation / copy
                newPostObject.SetUserId(id);
                newPostObject.SetParentId(newPost.GetId());

                postRepository_->Create(newPostObject); // NOTE: We don't need to wait for this one :D
            }

            json::value object;
            object["id"]          = json::value(newPost.GetId());
            object["created_at"]  = json::value(newPost.GetCreatedAt());
            object["description"] = json::value(newPost.GetDescription());
            object["image_url"]   = json::value(newPost.GetImageUrl());
            object["user_id"]     = json::value(newPost.GetUserId());
            return MakeOkResponse(object);
        }
        return MakeErrorResponse(1004, "Something went wrong creating the post");
    }

    json::value PostRouter::Get(const json::value& input)
    {
        if (!input.has_number_field("post_id"))
        {
            return MakeErrorResponse(kRestErrorCodeInvalidParam, "Invalid Parameters");
        }

        const auto iPostId = input.at("post_id").as_number().to_uint64();
        const auto iPost   = postRepository_->Read(iPostId).get(); // TODO async + continuation fiber

        json::value object;
        object["id"]          = json::value(iPost.GetId());
        object["created_at"]  = json::value(iPost.GetCreatedAt());
        object["description"] = json::value(iPost.GetDescription());
        object["image_url"]   = json::value(iPost.GetImageUrl());
        object["user_id"]     = json::value(iPost.GetUserId());  // TODO Change for user data
        object["likes"]       = json::value(likeRepository_->CountLikesByPostId(iPost.GetId()).get()); // TODO async + continuation fiber

        json::value inner;
        if (const auto& parent = iPost.GetParent(); parent)
        {
            inner["id"]          = json::value(parent->GetId());
            inner["created_at"]  = json::value(parent->GetCreatedAt());
            inner["description"] = json::value(parent->GetDescription());
            inner["image_url"]   = json::value(parent->GetImageUrl());
            inner["user_id"]     = json::value(parent->GetUserId());  // TODO Change for user data
            inner["likes"]       = json::value(likeRepository_->CountLikesByPostId(parent->GetId()).get()); // TODO async + continuation fiber

            object["parent"]     = inner;
        }

        return MakeOkResponse(object);
    }

    json::value PostRouter::GetAll(const json::value& input)
    {
        const auto iPage = input.has_number_field("page") ? input.at("page").as_number().to_int64() : 1;
        auto iSize       = input.has_number_field("size") ? input.at("size").as_number().to_int64() : kPagingMinSize;
        iSize = std::min(kPagingMaxSize, std::max(kPagingMinSize, iSize));

        json::value result;

        uint64_t iPostCount = postRepository_->CountPosts().get(); // TODO async + continuation fiber
        if (iPostCount > 0)
        {
            const std::vector<Post> posts = postRepository_->ReadAll(iPage, iSize).get(); // TODO async + continuation fiber

            json::value  resultArray     = json::value::array(posts.size());
            json::array& resultArrayView = resultArray.as_array();

            for (size_t i = 0, j = posts.size(); i < j; ++i)
            {
                const Post& post = posts[i];

                json::value object;
                object["id"]          = json::value(post.GetId());
                object["created_at"]  = json::value(post.GetCreatedAt());
                object["description"] = json::value(post.GetDescription());
                object["image_url"]   = json::value(post.GetImageUrl());
                object["user_id"]     = json::value(post.GetUserId());  // TODO Change for user data
                object["likes"]       = json::value(likeRepository_->CountLikesByPostId(post.GetId()).get()); // TODO async + continuation fiber

                json::value inner;
                if (const auto& parent = post.GetParent(); parent)
                {
                    inner["id"]          = json::value(parent->GetId());
                    inner["created_at"]  = json::value(parent->GetCreatedAt());
                    inner["description"] = json::value(parent->GetDescription());
                    inner["image_url"]   = json::value(parent->GetImageUrl());
                    inner["user_id"]     = json::value(parent->GetUserId());  // TODO Change for user data
                    inner["likes"]       = json::value(likeRepository_->CountLikesByPostId(parent->GetId()).get()); // TODO async + continuation fiber

                    object["parent"]     = inner;
                }
                resultArrayView[i] = object;
            }

            result["content"] = resultArray;
        }

        result["page"]  = json::value(iPage);
        result["size"]  = json::value(iSize);
        result["total"] = iPostCount;

        return MakeOkResponse(result);
    }

    json::value PostRouter::Like(const json::value& input)
    {
        if (!input.has_number_field("post_id"))
        {
            return MakeErrorResponse(kRestErrorCodeInvalidParam, "Invalid Parameters");
        }
        if (!input.has_number_field("user_id"))
        {
            return MakeErrorResponse(kRestErrorCodeInvalidParam, "Invalid Parameters");
        }

        const auto iPostId = input.at("post_id").as_number().to_uint64();
        const auto iUserId = input.at("user_id").as_number().to_uint64();

        if (!likeRepository_->ExistByPostIdAndUserId(iPostId, iUserId).get()) // TODO async + continuation fiber
        {
            MightyBlocks::Like newLikeObject;
            newLikeObject.SetPostId(iPostId);
            newLikeObject.SetUserId(iUserId);

            if (likeRepository_->Create(newLikeObject).get().GetId() != 0)  // TODO async + continuation fiber
            {
                return MakeOkResponse();
            }
            else
            {
                return MakeErrorResponse(1000, "Either user or post doesn't exist");
            }
        }
        else
        {
            return MakeErrorResponse(1001, "You can't like the post twice!");
        }
    }

    json::value PostRouter::Dislike(const json::value& input)
    {
        if (!input.has_number_field("post_id"))
        {
            return MakeErrorResponse(kRestErrorCodeInvalidParam, "Invalid Parameters");
        }
        if (!input.has_number_field("user_id"))
        {
            return MakeErrorResponse(kRestErrorCodeInvalidParam, "Invalid Parameters");
        }

        const auto iPostId = input.at("post_id").as_number().to_uint64();
        const auto iUserId = input.at("user_id").as_number().to_uint64();

        if (likeRepository_->ExistByPostIdAndUserId(iPostId, iUserId).get())        // TODO async + continuation fiber
        {
            if (likeRepository_->DeleteByPostIdAndUserId(iPostId, iUserId).get())   // TODO async + continuation fiber
            {
                return MakeOkResponse();
            }
            else
            {
                return MakeErrorResponse(1000, "Either user or post doesn't exist");
            }
        }
        else
        {
            return MakeErrorResponse(1002, "You can't dislike a post you never liked");
        }
    }

}