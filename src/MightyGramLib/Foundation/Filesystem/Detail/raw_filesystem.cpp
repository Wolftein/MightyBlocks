
#include "raw_filesystem.hpp"

#include <boost/lexical_cast.hpp>
#include <boost/uuid/uuid.hpp>
#include <boost/uuid/uuid_generators.hpp>
#include <boost/uuid/uuid_io.hpp>
#include <boost/filesystem.hpp>
#include <cpprest/filestream.h>
#include <cpprest/rawptrstream.h>

using namespace concurrency::streams;

namespace MightyBlocks
{
    RawFilesystem::RawFilesystem(const std::string& folder)
        : folder_(folder)
    {
        boost::filesystem::create_directory(folder);
    }

    pplx::task<std::string> RawFilesystem::Upload(const std::string& extension, const std::vector<unsigned char>& bytes)
    {
        auto filename = GenerateRandomFilename(extension);

        auto task = fstream::open_ostream(folder_ + filename).then([filename, bytes](const ostream& outFile) {
            if (bytes.empty())
            {
                return std::string { };
            }

            rawptr_buffer<unsigned char> buffer(bytes.data(), bytes.size());

            outFile.write(buffer, bytes.size()).get();
            outFile.close().get();

            return filename;
        });
        return task;
    }

    std::string RawFilesystem::GenerateRandomFilename(const std::string& extension)
    {
        boost::uuids::uuid uuid = (boost::uuids::random_generator())();
        return boost::uuids::to_string(uuid) + "." + extension; // For simplicity, append the extension of the file
    }
}