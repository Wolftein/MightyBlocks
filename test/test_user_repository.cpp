#include "main_environment.hpp"

TEST(TestUserRepository, CreateReadDelete)
{
    MightyBlocks::User newObject;
    newObject.SetName("Cacho");

    // Create an object
    newObject = gEnvironment->GetUserRepository()->Create(newObject).get();
    EXPECT_NE(newObject.GetId(), 0);
    EXPECT_EQ(newObject.GetName(), "Cacho");

    // Retrieve an object
    MightyBlocks::User addedObject = gEnvironment->GetUserRepository()->Read(newObject.GetId()).get();
    EXPECT_EQ(newObject.GetId(), addedObject.GetId());

    // Delete an object
    EXPECT_TRUE(gEnvironment->GetUserRepository()->Delete(newObject.GetId()).get());
}

TEST(TestUserRepository, CreateParenthood)
{
    MightyBlocks::User newObject;
    newObject.SetName("Cacho");

    // Create an object
    newObject = gEnvironment->GetUserRepository()->Create(newObject).get();
    EXPECT_NE(newObject.GetId(), 0);
    EXPECT_EQ(newObject.GetName(), "Cacho");

    // Parent it with the test user
    auto parentObject = std::make_shared<MightyBlocks::User>(gEnvironment->GetUserRepository()->Read(TEST_USER).get());
    newObject.SetParent(parentObject);
    EXPECT_TRUE(gEnvironment->GetUserRepository()->Update(newObject).get());

    // Check if is really the child of the test user
    auto parentChilds = gEnvironment->GetUserRepository()->GetChildren(TEST_USER).get();
    EXPECT_TRUE(parentChilds.size() == 1);
    EXPECT_TRUE(parentChilds[0] == newObject.GetId());

    // Delete an object
    EXPECT_TRUE(gEnvironment->GetUserRepository()->Delete(newObject.GetId()).get());
}

TEST(TestUserRepository, CreateAlreadyExist)
{
    MightyBlocks::User newObject;
    newObject.SetName(TEST_USER_NAME);

    // Create the object
    newObject = gEnvironment->GetUserRepository()->Create(newObject).get();
    EXPECT_EQ(newObject.GetId(), 0);
}

TEST(TestUserRepository, ReadInvalid)
{
    MightyBlocks::User invalidObject = gEnvironment->GetUserRepository()->Read(1337L).get();
    EXPECT_EQ(invalidObject.GetId(), 0);
}
