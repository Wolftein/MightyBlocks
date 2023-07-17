#include "main_environment.hpp"

TEST(TestLikeRepository, CreateReadDelete)
{
    MightyBlocks::Like newObject;
    newObject.SetUserId(TEST_USER);
    newObject.SetPostId(1);

    // Create an object
    newObject = gEnvironment->GetLikeRepository()->Create(newObject).get();
    EXPECT_NE(newObject.GetId(), 0);
    EXPECT_EQ(newObject.GetUserId(), TEST_USER);
    EXPECT_EQ(newObject.GetPostId(), 1);

    // Retrieve an object
    MightyBlocks::Like addedObject = gEnvironment->GetLikeRepository()->Read(newObject.GetId()).get();
    EXPECT_EQ(newObject.GetId(), addedObject.GetId());

    // Delete an object
    EXPECT_TRUE(gEnvironment->GetLikeRepository()->Delete(newObject.GetId()).get());
}

TEST(TestLikeRepository, CreateAlreadyExist)
{
    MightyBlocks::Like newObject;
    newObject.SetUserId(TEST_USER);
    newObject.SetPostId(1);

    // Create the object
    newObject = gEnvironment->GetLikeRepository()->Create(newObject).get();
    EXPECT_NE(newObject.GetId(), 0);

    // Create the object again
    MightyBlocks::Like addedObject = gEnvironment->GetLikeRepository()->Create(newObject).get();
    EXPECT_EQ(addedObject.GetId(), 0);

    // Clean object
    EXPECT_TRUE(gEnvironment->GetLikeRepository()->Delete(newObject.GetId()).get());
}

TEST(TestLikeRepository, CreateReadDeleteByUserIdOrPostId)
{
    MightyBlocks::Like newObject;
    newObject.SetUserId(TEST_USER);
    newObject.SetPostId(1);

    // Create an object
    newObject = gEnvironment->GetLikeRepository()->Create(newObject).get();
    EXPECT_NE(newObject.GetId(), 0);

    // Check if the post/user combination exist
    EXPECT_TRUE(gEnvironment->GetLikeRepository()->ExistByPostIdAndUserId(1, TEST_USER).get());

    // Delete by post/user combination
    EXPECT_TRUE(gEnvironment->GetLikeRepository()->DeleteByPostIdAndUserId(1, TEST_USER).get());
}

TEST(TestLikeRepository, ReadInvalid)
{
    MightyBlocks::Like invalidObject = gEnvironment->GetLikeRepository()->Read(1337L).get();
    EXPECT_EQ(invalidObject.GetId(), 0);
}
