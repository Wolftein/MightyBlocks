#pragma once

#include "Foundation/Filesystem/filesystem.hpp"

namespace MightyBlocks
{
    /**
     * Simple \see Filesystem implementation that uses a folder inside the server to upload files.
     */
    class RawFilesystem final : public Filesystem
    {
    public:

        RawFilesystem(const std::string& folder);

        pplx::task<std::string> Upload(const std::string& extension, const std::vector<unsigned char>& bytes) override;

    private:

        [[nodiscard]] static std::string GenerateRandomFilename(const std::string& extension);

    private:

        const std::string folder_;
    };
}