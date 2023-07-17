#pragma once

#include "router.hpp"
#include <cpprest/http_msg.h>
#include <cpprest/http_listener.h>
#include <pplx/pplxtasks.h>

using namespace web::http;
using namespace web::http::experimental::listener;

namespace MightyBlocks
{
    /**
     * Basic interface for the web service controller.
     */
    class Controller
    {
    public:

        /**
         * Register a new \see Router.
         */
        template<typename Type, typename... Params>
        void Register(const std::string& name, Params&&... params)
        {
            const std::shared_ptr<Type> inplace = std::make_shared<Type>(std::forward<Params>(params)...);
            routers_.emplace(name, inplace);
        }

        /**
         * Start accepting connections.
         *
         * @param endpoint Endpoint on which the controller is going to listen to.
         */
        pplx::task<void> Start(const std::string& endpoint);

        /**
         * Stop accepting connections.
         */
        pplx::task<void> Shutdown();

        /**
         * Get the endpoint on which the controller is listening to.
         *
         * @return the endpoint formatted as URI
         */
        [[nodiscard]] std::string GetEndpoint() const;

    protected:

        /**
         * Handle GET/PUT/POST HTTP method.
         *
         * \see https://developer.mozilla.org/en-US/docs/Web/HTTP/Methods/GET
         * \see https://developer.mozilla.org/en-US/docs/Web/HTTP/Methods/PUT
         * \see https://developer.mozilla.org/en-US/docs/Web/HTTP/Methods/POST
         *
         * @param message the user request
         */
        void Handle(const http::method& type, http_request& message);

    private:

        http_listener listener_;
        std::unordered_map<std::string, std::shared_ptr<Router>> routers_;
    };
}