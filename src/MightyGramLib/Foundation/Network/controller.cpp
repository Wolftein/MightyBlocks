
#include "controller.hpp"

namespace MightyBlocks
{
    pplx::task<void> Controller::Start(const std::string& endpoint)
    {
        listener_ = http_listener(endpoint);
        listener_.support(methods::GET,  [this](http_request message) { Handle(http::methods::GET, message); });
        listener_.support(methods::PUT,  [this](http_request message) { Handle(http::methods::PUT, message); });
        listener_.support(methods::POST, [this](http_request message) { Handle(http::methods::POST,message); });
        return listener_.open(); // ASIO ftw
    }

    pplx::task<void> Controller::Shutdown()
    {
        return listener_.close();
    }

    std::string Controller::GetEndpoint() const
    {
        return listener_.uri().to_string();
    }

    void Controller::Handle(const http::method& type, http_request& message)
    {
        auto paths = http::uri::split_path(http::uri::decode(message.relative_uri().path()));

        // NOTE: We only support router/endpoint for simplicity...
        if (paths.size() == 2)
        {
            if (const auto iterator = routers_.find(paths[0]); iterator != routers_.end())
            {
                const auto endpoint = paths[1];
                const auto router   = iterator->second;

                // @NOTE: extract_json() will crash internally if a malformed JSON is supplied. In-order to fix it,
                // I manually extract the body to UTF8 encoded string and parse the JSON within an exception block.
                //
                // I have reported this issue to the owner of the library (Microsoft)
                // https://github.com/microsoft/cpprestsdk/issues/1691
                message.extract_utf8string().then([type, endpoint, router, message](const utf8string& value) {
                    try
                    {
                        const auto json = json::value::parse(utility::conversions::to_string_t(value));
                        const auto& response = router->Route(type, endpoint, json);
                        message.reply(http::status_codes::OK, response);
                    }
                    catch (...)
                    {
                        // TODO Logging
                        message.reply(status_codes::BadRequest);
                    }
                });
            }
            else
            {
                message.reply(status_codes::NotImplemented);
            }
        }
        else
        {
            message.reply(http::status_codes::BadGateway);
        }
    }
}