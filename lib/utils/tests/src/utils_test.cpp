#include <gtest/gtest.h>

extern "C" {
#include "utils.h"
}

#define BUF_SIZE 100

TEST(CmpDates, NullPointer) {
    int res = cmp_dates(nullptr, nullptr);
    EXPECT_EQ(res, 0);
}

TEST(CmpDates, Correct) {
    time_t date_first = {};
    mail_error_t err_first = str_to_date("2022-03-31", &date_first);
    ASSERT_EQ(err_first, ERROR_SUCCESS);

    time_t date_second = {};
    mail_error_t err_second = str_to_date("2022-04-05", &date_second);
    ASSERT_EQ(err_second, ERROR_SUCCESS);

    int res = cmp_dates(&date_first, &date_second);
    EXPECT_EQ(res, 1);
}

TEST(ReadString, NullPointer) {
    char input[BUF_SIZE] = "something\n";
    FILE *fp_in = fmemopen(input, BUF_SIZE, "r");
    ASSERT_NE(fp_in, nullptr);

    mail_error_t err = read_string(fp_in, nullptr, 0);
    EXPECT_EQ(err, ERROR_NULL_POINTER);

    fclose(fp_in);
}

TEST(ReadString, WrongFormat) {
    char input[BUF_SIZE] = "aaaaaaaaaaaaaaaaaa";
    FILE *fp_in = fmemopen(input, BUF_SIZE, "r");
    ASSERT_NE(fp_in, nullptr);

    char str[10];
    mail_error_t err = read_string(fp_in, str, 10);
    EXPECT_EQ(err, ERROR_FORMAT);

    fclose(fp_in);
}

TEST(ReadString, CorrectFormat) {
    char input[BUF_SIZE] = "correct\nformat\n";
    FILE *fp_in = fmemopen(input, BUF_SIZE, "r");
    ASSERT_NE(fp_in, nullptr);

    char str[10];
    mail_error_t err = read_string(fp_in, str, 10);
    EXPECT_EQ(err, ERROR_SUCCESS);

    EXPECT_STREQ(str, "correct");

    fclose(fp_in);
}

TEST(ReadDate, NullPointer) {
    char input[BUF_SIZE] = "something\n";
    FILE *fp_in = fmemopen(input, BUF_SIZE, "r");
    ASSERT_NE(fp_in, nullptr);

    mail_error_t err = read_date(fp_in, nullptr);
    EXPECT_EQ(err, ERROR_NULL_POINTER);

    fclose(fp_in);
}

TEST(ReadDate, WithoutNewline) {
    char input[BUF_SIZE] = "2022-03-31";
    FILE *fp_in = fmemopen(input, BUF_SIZE, "r");
    ASSERT_NE(fp_in, nullptr);

    time_t date = {};
    mail_error_t err = read_date(fp_in, &date);
    EXPECT_EQ(err, ERROR_FORMAT);

    fclose(fp_in);
}

TEST(ReadDate, CorrectFormat) {
    char input[BUF_SIZE] = "2022-03-31\n";
    FILE *fp_in = fmemopen(input, BUF_SIZE, "r");
    ASSERT_NE(fp_in, nullptr);

    time_t date = {};
    mail_error_t err = read_date(fp_in, &date);
    EXPECT_EQ(err, ERROR_SUCCESS);

    fclose(fp_in);
}

TEST(ReadDate, WrongFormat) {
    char input[BUF_SIZE] = "2022.03.31\n";
    FILE *fp_in = fmemopen(input, BUF_SIZE, "r");
    ASSERT_NE(fp_in, nullptr);

    time_t date = {};
    mail_error_t err = read_date(fp_in, &date);
    EXPECT_EQ(err, ERROR_FORMAT);

    fclose(fp_in);
}

TEST(StrToDate, NullPointer) {
    mail_error_t err = str_to_date(nullptr, nullptr);
    EXPECT_EQ(err, ERROR_NULL_POINTER);
}

TEST(StrToDate, WrongFormat) {
    time_t date = {};
    mail_error_t err = str_to_date("2022-03-32", &date);
    EXPECT_EQ(err, ERROR_FORMAT);
}

TEST(StrToDate, CorrectFormat) {
    time_t date = {};
    mail_error_t err = str_to_date("2022-03-31", &date);
    EXPECT_EQ(err, ERROR_SUCCESS);
}

int main(int argc, char *argv[]) {
    ::testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}
