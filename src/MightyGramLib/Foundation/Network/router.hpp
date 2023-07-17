#pragma once

#include <cpprest/http_msg.h>

using namespace web;

namespace MightyBlocks
{
    /**
     * Basic interface for the web service router.
     */
    class Router
    {
    public:

        static constexpr uint32_t kRestErrorCodeInternalError   = 1;
        static constexpr uint32_t kRestErrorCodeInvalidParam    = 2;
        static constexpr uint32_t kRestErrorCodeInvalidEndpoint = 3;
        static constexpr uint32_t kRestErrorCodeInvalidData     = 4;

        static constexpr int64_t kPagingMinSize                 = 5;
        static constexpr int64_t kPagingMaxSize                 = 100;  // @NOTE: to prevent attackers

    public:

        /**
         * Handle HTTP request to route them internally.
         *
         * @param path  the path of the endpoint
         * @param input the user input
         */
        [[maybe_unused]] virtual json::value Route(const http::method& type, const std::string& path, const json::value& input)
        {
            return MakeErrorResponse(kRestErrorCodeInvalidEndpoint, "Routing is not implemented");
        }

    protected:

        /**
         * Creates an OK response.
         *
         * @param aggregate any extra data to propagate to the user
         */
        static json::value MakeOkResponse(const json::value &aggregate = {})
        {
            json::value Response;
            Response["success"] = true;

            if (!aggregate.is_null())
            {
                Response["data"]  = aggregate;
            }

            return Response;
        }

        /**
         * Creates an ERROR response.
         *
         * @param code the error code
         * @param description the error description (optional)
         */
        static json::value MakeErrorResponse(uint32_t code, const std::string& description = "")
        {
            json::value Response;
            Response["success"] = false;
            Response["code"]    = json::value(code);

            if (!description.empty())
            {
                Response["description"]  = json::value(description);
            }

            return Response;
        }
    };
}