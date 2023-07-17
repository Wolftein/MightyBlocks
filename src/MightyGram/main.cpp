#include <Application/service.hpp>

#include <unistd.h>

// C makes it easy to shoot yourself in the foot; C++ makes it harder, but when you do it blows your whole leg off
int main(int argc, const char * argv[])
{
    const char* cfgServiceEndpoint          = "http://0.0.0.0:8080/v1/";
    const char* cfgDatabaseConnectionString = "postgres://postgres:postgres@127.0.0.1:5432/postgres";

    if (const char* env = std::getenv("MG_SERVICE_ENDPOINT"))
    {
        cfgServiceEndpoint = env;
    }
    if (const char* env = std::getenv("MG_DB_CONNECTION_STRING"))
    {
        cfgDatabaseConnectionString = env;
    }

    // Create the service
    MightyBlocks::Service service;
    service.Start(cfgServiceEndpoint, cfgDatabaseConnectionString);

    pause();    // Wait until user presses CTRL+C

    service.Stop();

    return 0;
}