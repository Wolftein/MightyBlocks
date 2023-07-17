#include "main_environment.hpp"

TEST(TestPostRepository, CreateReadDelete)
{
    MightyBlocks::Post newObject;
    newObject.SetUserId(TEST_USER);
    newObject.SetDescription("This is a Test!");
    newObject.SetImageUrl("Placeholder.png");

    // Create an object
    newObject = gEnvironment->GetPostRepository()->Create(newObject).get();
    EXPECT_NE(newObject.GetId(), 0);
    EXPECT_EQ(newObject.GetParentId(), 0);
    EXPECT_EQ(newObject.GetUserId(), TEST_USER);
    EXPECT_EQ(newObject.GetDescription(), "This is a Test!");
    EXPECT_EQ(newObject.GetImageUrl(), "Placeholder.png");

    // Retrieve an object
    MightyBlocks::Post addedObject = gEnvironment->GetPostRepository()->Read(newObject.GetId()).get();
    EXPECT_EQ(newObject.GetId(), addedObject.GetId());
    EXPECT_EQ(newObject.GetUserId(), addedObject.GetUserId());
    EXPECT_EQ(newObject.GetDescription(), addedObject.GetDescription());
    EXPECT_EQ(newObject.GetImageUrl(), addedObject.GetImageUrl());

    // Delete an object
    EXPECT_TRUE(gEnvironment->GetPostRepository()->Delete(newObject.GetId()).get());
}

TEST(TestPostRepository, ReadInvalid)
{
    MightyBlocks::Post invalidObject = gEnvironment->GetPostRepository()->Read(1337L).get();
    EXPECT_EQ(invalidObject.GetId(), 0);
}
