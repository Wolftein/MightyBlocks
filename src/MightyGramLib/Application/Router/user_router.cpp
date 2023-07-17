#include "user_router.hpp"

namespace MightyBlocks
{
    UserRouter::UserRouter(std::shared_ptr<UserRepository> userRepository)
        : userRepository_(std::move(userRepository))
    {
    }

    json::value UserRouter::Route(const http::method& type, const std::string& path, const json::value& input)
    {
        if (type == http::methods::POST)
        {
            if (path == "parenthood")
            {
                return Parenthood(input);
            }
        }
        else if (type == http::methods::GET)
        {
            if (path == "id")
            {
                return GetById(input);
            }
        }
        return MakeErrorResponse(kRestErrorCodeInvalidEndpoint, type + " is not supported for " + path);
    }

    json::value UserRouter::GetById(const json::value& input)
    {
        if (!input.has_number_field("user_id"))
        {
            return MakeErrorResponse(kRestErrorCodeInvalidParam, "invalid parameters");
        }

        const auto iUserId = input.at("user_id").as_number().to_uint64();
        const auto iUser   = userRepository_->Read(iUserId).get(); // TODO async + continuation fiber

        json::value object;
        object["id"]          = json::value(iUser.GetId());
        object["created_at"]  = json::value(iUser.GetCreatedAt());
        object["name"]        = json::value(iUser.GetName());

        json::value inner;
        if (const auto& parent = iUser.GetParent(); parent)
        {
            inner["id"]          = json::value(parent->GetId());
            inner["created_at"]  = json::value(parent->GetCreatedAt());
            inner["name"]        = json::value(parent->GetName());
        }
        object["parent"]         = inner;

        return MakeOkResponse(object);
    }

    json::value UserRouter::Parenthood(const json::value& input)
    {
        if (!input.has_number_field("child_id"))
        {
            return MakeErrorResponse(kRestErrorCodeInvalidParam, "Invalid Parameters");
        }
        if (!input.has_number_field("parent_id"))
        {
            return MakeErrorResponse(kRestErrorCodeInvalidParam, "Invalid Parameters");
        }

        const auto iChildId  = input.at("child_id").as_number().to_uint64();
        const auto iParentId = input.at("parent_id").as_number().to_uint64();

        if (iChildId == iParentId)
        {
            return MakeErrorResponse(1000, "Trying to parent itself");
        }

        User oChild = userRepository_->Read(iChildId).get(); // TODO async + continuation fiber
        if (oChild.GetId() == 0)
        {
            return MakeErrorResponse(1001, "Child doesn't exist");
        }
        else if (oChild.GetParent() != nullptr)
        {
            return MakeErrorResponse(1002, "Trying to parent an user that has already a parent set");
        }

        const User oParent = userRepository_->Read(iParentId).get(); // TODO async + continuation fiber
        if (oParent.GetId() == 0)
        {
            return MakeErrorResponse(1003, "Trying to use an invalid parent user");
        }

        if (oParent.GetParent() && oParent.GetParent()->GetId() == oChild.GetId())
        {
            return MakeErrorResponse(1004, "Trying to parent your parent");
        }

        oChild.SetParent(std::make_shared<User>(oParent)); // TODO remove this heap allocation

        if (userRepository_->Update(oChild).get()) // TODO async + continuation fiber
        {
            return MakeOkResponse();
        }
        return MakeErrorResponse(kRestErrorCodeInternalError, "Internal Error");
    }
}