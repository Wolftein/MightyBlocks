# mightygram-challenge

## Technologies

- C++17
- Boost
- PostgreSQL 14
- CMake

I have decided to use C++ since it is a language that is not used for this type of projects, which presents a great challenge, besides being my preferred language.

C++ is one of the most performant languages if used correctly, but it's also too easy to shoot yourself in the foot with.

One of the biggest challenges I faced was my lack of experience with web development and lack of framework support, so everything had to be designed from scratch.

## Dependencies

- OpenSSL 
- Boost
- CPPRestSDK
- PostgreSQL Driver
- TaoPQ

## Docker

Running 'docker-compose up' will build the project into an image, load it, and run it.

## Tests

I wasn't able to mock up the database with C++ and Google Test, so to run the tests you need to have access to a 
database and run MightyGramTest application.

## Api Documentation

### Creating a post

curl --location --request GET 'http://localhost:8080/v1/post/new'
--header 'Content-Type: application/json'
--data-raw '{ "image": "...", "description": "Lorem ipsum dolor sit amet", "user_id": 1 }'

_Image is embedded in the JSON request encoded as Base64._

**On Success**

```|
{
  "data": {
    "created_at": 1646833672666,
    "description": "Lorem ipsum dolor sit amet",
    "id": 6,
    "image_url": "c90e2de6-2592-43c4-bba3-a290ccde5a5b.png",
    "user_id": 3
  },
  "success": 1
}
```

**On Error**

```|
{
  "code": code,
  "description": "description",
  "success": 0
}
```

- 2: Invalid Parameters
- 1000: Description must not be empty
- 1001: Something went wrong trying to upload the image
- 1002: Image is not correctly encoded as Base64
- 1003: Something went wrong trying to upload the image
- 1004: Something went wrong creating the post

### Getting a post

curl --location --request GET 'http://localhost:8080/v1/post/id'
--header 'Content-Type: application/json'
--data-raw '{ "post_id": 1 }'

**On Success**

```|
{
  "data": {
    "created_at": 1646921799241,
    "description": "1st Post",
    "id": 1,
    "image_url": "TestImage.png",
    "likes": 1,
    "user_id": 2
  },
  "success": 1
}
```

**On Error**

```|
{
  "code": code,
  "description": "description",
  "success": 0
}
```

- 2: Invalid Parameters

### Getting posts

curl --location --request GET 'http://localhost:8080/v1/post/all'
--header 'Content-Type: application/json'
--data-raw '{ "page": 1, "size": 20 }'

_Constraint: Page must be bigger than 0 and size must be equal or less than 100._

_'page' and 'size' are the parameters passed by the user and 'total' is the amount of posts available._

**On Success**

```|
{
  "data": {
    "content": [
      {
        "created_at": 1646858156215,
        "description": "Lorem ipsum dolor sit amet",
        "id": 2,
        "image_url": "457c53cb-0f93-4b00-b501-f84aa01bb5ab.png",
        "likes": 0,
        "parent": {
          "created_at": 1646858156213,
          "description": "Lorem ipsum dolor sit amet",
          "id": 1,
          "image_url": "457c53cb-0f93-4b00-b501-f84aa01bb5ab.png",
          "likes": 0,
          "user_id": 1
        },
        "user_id": 3
      },
      {
        "created_at": 1646858156213,
        "description": "Lorem ipsum dolor sit amet",
        "id": 1,
        "image_url": "457c53cb-0f93-4b00-b501-f84aa01bb5ab.png",
        "likes": 0,
        "parent": null,
        "user_id": 1
      }
    ],
    "total": 2,
    "page": 1,
    "size": 50
  },
  "success": 1
}
```

### Liking and disliking a post

curl --location --request POST 'http://localhost:8080/v1/post/like'
--header 'Content-Type: application/json'
--data-raw '{ "user_id": 1, "post_id": 2 }'

curl --location --request POST 'http://localhost:8080/v1/post/dislike'
--header 'Content-Type: application/json'
--data-raw '{ "user_id": 1, "post_id": 2 }'

**On Success**

```|
{
  "success": 1
}
```

**On Error**

```|
{
  "code": code,
  "description": "description",
  "success": 0
}
```

- 2: Invalid Parameters
- 1000: Either user or post doesn't exist
- 1001: You can't like the post twice!
- 1002: You can't dislike a post you never liked

### Parenthood an user

curl --location --request POST 'http://localhost:8080/v1/user/parenthood'
--header 'Content-Type: application/json'
--data-raw '{ "parent_id": 1, "child_id": 3 }'

**On Success**

```|
{
  "success": 1
}
```

**On Error**

```|
{
  "code": code,
  "description": "description",
  "success": 0
}
```

- 1: Internal Error
- 2: Invalid Parameters
- 1000: Trying to parent itself
- 1001: Child doesn't exist
- 1002: Trying to parent an user that has already a parent set
- 1003: Trying to use an invalid parent user
- 1004: Trying to parent your parent

## Things to improve

- Add coroutines to make everything 100% async.
- Replace some heap allocations using pool allocators.
- Add more logging.
- Find a way to mock up the database for the tests in order to run the tests after compiling.