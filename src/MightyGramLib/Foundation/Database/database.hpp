#pragma once

#include <tao/pq.hpp>
#include <tao/pq/internal/zsv.hpp>
#include <pplx/pplxtasks.h>

namespace MightyBlocks
{
    /**
     * Basic interface for the database service.
     */
    class Database final
    {
    public:

        explicit Database(const std::string& url);

        /**
         * Forward declaration to execute a statement.
         */
        template< typename... Params>
        auto Execute(const tao::pq::internal::zsv statement, Params&&... params)
        {
            return pool_->connection()->direct()->execute(statement, std::forward<Params>(params)...);
        }

    private:

        std::shared_ptr<tao::pq::connection_pool> pool_;
    };
}