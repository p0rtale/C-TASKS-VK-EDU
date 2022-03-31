#include <gtest/gtest.h>

extern "C" {
#include "generators.h"
}

TEST(GenerateUsers, NullPointer) {
    mail_error_t err = generate_users(nullptr, 1);
    EXPECT_EQ(err, ERROR_NULL_POINTER);
}

TEST(GenerateUsers, Correct) {
    users_t users = {};
    mail_error_t err = generate_users(&users, 3);
    EXPECT_EQ(err, ERROR_SUCCESS);

    EXPECT_EQ(users.size, 3);

    clear_users(&users);
}

TEST(GenerateMails, NullPointer) {
    mail_error_t err = generate_mails(nullptr, 1, nullptr, 1);
    EXPECT_EQ(err, ERROR_NULL_POINTER);
}

TEST(GenerateMails, Correct) {
    users_t users = {};
    mail_error_t err = generate_users(&users, 3);
    ASSERT_EQ(err, ERROR_SUCCESS);

    mails_t mails = {};
    err = generate_mails(&mails, 5, &users, 3);
    clear_users(&users);
    ASSERT_EQ(err, ERROR_SUCCESS);

    EXPECT_EQ(mails.size, 5);

    clear_mails(&mails);
}

int main(int argc, char* argv[]) {
    ::testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}
