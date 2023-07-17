#include "database.hpp"

namespace MightyBlocks
{
    Database::Database(const std::string& url)
        : pool_(tao::pq::connection_pool::create(url))
    {
    }
}