#include <gtest/gtest.h>
#include <math.h>

extern "C" {
#include "utils.h"
}

#define BUF_SIZE 100

TEST(CmpStrings, Correct) {
    char arr_first[] = "aaaaa";
    string_t str_first = {arr_first, 5};

    char arr_second[] = "bbbbb";
    string_t str_second = {arr_second, 5};

    int res = cmp_strings(&str_first, &str_second);

    EXPECT_EQ(res, -1);
}

TEST(CmpStrings, NullPointer) {
    int res = cmp_strings(nullptr, nullptr);
    EXPECT_EQ(res, 0);
}

TEST(ReadString, CorrectWithoutNewline) {
    char input[BUF_SIZE] = "something";
    FILE *fp_in = fmemopen(input, BUF_SIZE, "r");
    ASSERT_NE(fp_in, nullptr);

    string_t str = {};

    contract_error_t err = read_string(fp_in, &str);
    EXPECT_EQ(err, ERROR_SUCCESS);

    clear_string(&str);

    fclose(fp_in);
}

TEST(ReadString, CorrectWithNewline) {
    char input[BUF_SIZE] = "something\n";
    FILE *fp_in = fmemopen(input, BUF_SIZE, "r");
    ASSERT_NE(fp_in, nullptr);

    string_t str = {};

    contract_error_t err = read_string(fp_in, &str);
    EXPECT_EQ(err, ERROR_SUCCESS);

    clear_string(&str);

    fclose(fp_in);
}

TEST(ReadString, NullPointer) {
    char input[BUF_SIZE] = "something\n";
    FILE *fp_in = fmemopen(input, BUF_SIZE, "r");
    ASSERT_NE(fp_in, nullptr);

    contract_error_t err = read_string(fp_in, nullptr);
    EXPECT_EQ(err, ERROR_NULL_POINTER);

    fclose(fp_in);
}

TEST(ReadDouble, WithoutNewline) {
    char input[BUF_SIZE] = "123.22";
    FILE *fp_in = fmemopen(input, BUF_SIZE, "r");
    ASSERT_NE(fp_in, nullptr);

    double d = 0.0;
    contract_error_t err = read_double(fp_in, &d);
    EXPECT_EQ(err, ERROR_READ_FORMAT);

    fclose(fp_in);
}

TEST(ReadDouble, Correct) {
    char input[BUF_SIZE] = "123.22\n";
    FILE *fp_in = fmemopen(input, BUF_SIZE, "r");
    ASSERT_NE(fp_in, nullptr);

    double d = 0.0;
    contract_error_t err = read_double(fp_in, &d);
    EXPECT_EQ(err, ERROR_SUCCESS);
    EXPECT_TRUE(fabs(d - 123.22) < 1e-2);

    fclose(fp_in);
}

TEST(ReadDouble, NullPointer) {
    char input[BUF_SIZE] = "something\n";
    FILE *fp_in = fmemopen(input, BUF_SIZE, "r");
    ASSERT_NE(fp_in, nullptr);

    contract_error_t err = read_double(fp_in, nullptr);
    EXPECT_EQ(err, ERROR_NULL_POINTER);

    fclose(fp_in);
}

TEST(ReadDate, WithoutNewline) {
    char input[BUF_SIZE] = "2022-03-16";
    FILE *fp_in = fmemopen(input, BUF_SIZE, "r");
    ASSERT_NE(fp_in, nullptr);

    time_t date = {};
    contract_error_t err = read_date(fp_in, &date);
    EXPECT_EQ(err, ERROR_READ_FORMAT);

    fclose(fp_in);
}

TEST(ReadDate, CorrectFormat) {
    char input[BUF_SIZE] = "2022-03-16\n";
    FILE *fp_in = fmemopen(input, BUF_SIZE, "r");
    ASSERT_NE(fp_in, nullptr);

    time_t date = {};
    contract_error_t err = read_date(fp_in, &date);
    EXPECT_EQ(err, ERROR_SUCCESS);

    fclose(fp_in);
}

TEST(ReadDate, WrongFormat) {
    char input[BUF_SIZE] = "2022.03.16\n";
    FILE *fp_in = fmemopen(input, BUF_SIZE, "r");
    ASSERT_NE(fp_in, nullptr);

    time_t date = {};
    contract_error_t err = read_date(fp_in, &date);
    EXPECT_EQ(err, ERROR_READ_FORMAT);

    fclose(fp_in);
}

TEST(ReadDate, NullPointer) {
    char input[BUF_SIZE] = "something\n";
    FILE *fp_in = fmemopen(input, BUF_SIZE, "r");
    ASSERT_NE(fp_in, nullptr);

    contract_error_t err = read_date(fp_in, nullptr);
    EXPECT_EQ(err, ERROR_NULL_POINTER);

    fclose(fp_in);
}

int main(int argc, char *argv[]) {
    ::testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}
