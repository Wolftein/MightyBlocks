#pragma once

#include <pplx/pplxtasks.h>

namespace MightyBlocks
{
    /**
     * Basic interface for the filesystem service.
     */
    class Filesystem
    {
    public:

        /**
         * Uploads a file asynchronously.
         *
         * @param extension the extension of the new file
         * @param bytes     the content of the file
         *
         * @return a completion task that returns the name of the new file stored
         */
        virtual pplx::task<std::string> Upload(const std::string& extension, const std::vector<unsigned char>& bytes) = 0;
    };
}