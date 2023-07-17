#pragma once

#include "Foundation/Network/controller.hpp"

namespace MightyBlocks
{
    class Service final
    {
    public:

        void Start(const std::string& serviceInfo, const std::string& databaseInfo);
        void Stop();

    private:

        std::unique_ptr<Controller> controller_;
    };
}
