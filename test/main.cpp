#include "main_environment.hpp"

TestEnvironment* gEnvironment = nullptr;

int main(int argc, char* argv[])
{
    ::testing::InitGoogleTest(&argc, argv);

    gEnvironment = new TestEnvironment();
    ::testing::AddGlobalTestEnvironment(gEnvironment);

    return RUN_ALL_TESTS();
}