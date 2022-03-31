#include <gtest/gtest.h>

extern "C" {
#include "mails.h"
}

#define BUF_SIZE 200

TEST(IsSameUsers, NullPointer) {
    int res = is_same_users(nullptr, nullptr);
    EXPECT_EQ(res, 0);
}

TEST(IsSameUsers, IsSame) {
    user_t user_first = {};
    strncpy(user_first.name, "name", USERNAME_SIZE);
    user_t user_second = {};
    strncpy(user_second.name, "name", USERNAME_SIZE);
    int res = is_same_users(&user_first, &user_second);
    EXPECT_EQ(res, 1);
}

TEST(IsSameUsers, IsNotSame) {
    user_t user_first = {};
    strncpy(user_first.name, "name", USERNAME_SIZE);
    user_t user_second = {};
    strncpy(user_second.name, "name!", USERNAME_SIZE);
    int res = is_same_users(&user_first, &user_second);
    EXPECT_EQ(res, 0);
}

TEST(ReadUsers, NullPointer) {
    char input[BUF_SIZE] = "something\n";
    FILE *fp_in = fmemopen(input, BUF_SIZE, "r");
    ASSERT_NE(fp_in, nullptr);

    mail_error_t err = read_users(fp_in, nullptr);
    EXPECT_EQ(err, ERROR_NULL_POINTER);

    fclose(fp_in);
}

TEST(ReadUsers, WrongSizeFormat) {
    char input[BUF_SIZE] = "-1\naaaa\n";
    FILE *fp_in = fmemopen(input, BUF_SIZE, "r");
    ASSERT_NE(fp_in, nullptr);

    users_t users = {};
    mail_error_t err = read_users(fp_in, &users);
    EXPECT_EQ(err, ERROR_FORMAT);

    fclose(fp_in);
}

TEST(ReadUsers, Correct) {
    char input[BUF_SIZE] = "2\naaaa\nbbbb\n";
    FILE *fp_in = fmemopen(input, BUF_SIZE, "r");
    ASSERT_NE(fp_in, nullptr);

    users_t users = {};
    mail_error_t err = read_users(fp_in, &users);
    fclose(fp_in);
    ASSERT_EQ(err, ERROR_SUCCESS);

    size_t size = users.size;
    if (size != 2) {
        clear_users(&users);
        ASSERT_EQ(size, 2);
    }
    EXPECT_STREQ(users.arr[0].name, "aaaa");
    EXPECT_STREQ(users.arr[1].name, "bbbb");

    clear_users(&users);
}

TEST(ReadMail, NullPointer) {
    char input[BUF_SIZE] = "something";
    FILE *fp_in = fmemopen(input, BUF_SIZE, "r");
    ASSERT_NE(fp_in, nullptr);

    mail_error_t err = read_mail(fp_in, nullptr);
    EXPECT_EQ(err, ERROR_NULL_POINTER);

    fclose(fp_in);
}

TEST(ReadMail, Correct) {
    char input[BUF_SIZE] = "sender\nsubject\nbody\n2\nrecipient1\nrecipient2\n2022-03-31\n";
    FILE *fp_in = fmemopen(input, BUF_SIZE, "r");
    ASSERT_NE(fp_in, nullptr);

    mail_t mail = {};
    mail_error_t err = read_mail(fp_in, &mail);
    fclose(fp_in);
    ASSERT_EQ(err, ERROR_SUCCESS);

    EXPECT_STREQ(mail.sender.name, "sender");
    EXPECT_STREQ(mail.subject, "subject");
    EXPECT_STREQ(mail.body, "body");

    size_t size = mail.recipients.size;
    if (size != 2) {
        clear_mail(&mail);
        ASSERT_EQ(size, 2);
    }

    EXPECT_STREQ(mail.recipients.arr[0].name, "recipient1");
    EXPECT_STREQ(mail.recipients.arr[1].name, "recipient2");

    clear_mail(&mail);
}

TEST(ReadMails, NullPointer) {
    char input[BUF_SIZE] = "something";
    FILE *fp_in = fmemopen(input, BUF_SIZE, "r");
    ASSERT_NE(fp_in, nullptr);

    mail_error_t err = read_mails(fp_in, nullptr);
    EXPECT_EQ(err, ERROR_NULL_POINTER);

    fclose(fp_in);
}

TEST(ReadMails, WrongSizeFormat) {
    char input[BUF_SIZE] = "-1\nsender\nsubject\nbody\n2\nrecipient1\nrecipient2\n2022-03-31\n";
    FILE *fp_in = fmemopen(input, BUF_SIZE, "r");
    ASSERT_NE(fp_in, nullptr);

    mails_t mails = {};
    mail_error_t err = read_mails(fp_in, &mails);
    EXPECT_EQ(err, ERROR_FORMAT);

    fclose(fp_in);
}

TEST(ReadMails, Correct) {
    char input[BUF_SIZE] =
        "2\n"
        "sender1\nsubject\nbody\n2\nrecipient1\nrecipient2\n2022-03-31\n"
        "sender2\nsubject\nbody\n1\nrecipient\n2022-03-30\n";

    FILE *fp_in = fmemopen(input, BUF_SIZE, "r");
    ASSERT_NE(fp_in, nullptr);

    mails_t mails = {};
    mail_error_t err = read_mails(fp_in, &mails);
    fclose(fp_in);
    ASSERT_EQ(err, ERROR_SUCCESS);

    size_t size = mails.size;
    if (size != 2) {
        clear_mails(&mails);
        ASSERT_EQ(size, 2);
    }

    EXPECT_STREQ(mails.arr[0].sender.name, "sender1");
    EXPECT_STREQ(mails.arr[1].sender.name, "sender2");

    clear_mails(&mails);
}

int main(int argc, char *argv[]) {
    ::testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}
