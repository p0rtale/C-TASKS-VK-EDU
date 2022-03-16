#include <gtest/gtest.h>
#include <math.h>

extern "C" {
#include "contract.h"
}

#define BUF_SIZE 200

TEST(ReadAgreement, NullPointer) {
    char input[BUF_SIZE] = "something";
    FILE *fp_in = fmemopen(input, BUF_SIZE, "r");
    ASSERT_NE(fp_in, nullptr);

    contract_error_t err = read_agreement(fp_in, nullptr, nullptr);
    EXPECT_EQ(err, ERROR_NULL_POINTER);

    fclose(fp_in);
}

TEST(ReadAgreement, WrongDateFormat) {
    char input[BUF_SIZE] = "100\n2022.03.16\n";
    FILE *fp_in = fmemopen(input, BUF_SIZE, "r");
    ASSERT_NE(fp_in, nullptr);

    agreement_t agreement = {};
    time_t contract_date = 0;
    contract_error_t err = read_agreement(fp_in, &agreement, &contract_date);
    EXPECT_EQ(err, ERROR_READ_FORMAT);

    fclose(fp_in);
}

TEST(ReadAgreement, WrongAmountFormat) {
    char input[BUF_SIZE] = "10..0\n2022-03-16\n";
    FILE *fp_in = fmemopen(input, BUF_SIZE, "r");
    ASSERT_NE(fp_in, nullptr);

    agreement_t agreement = {};
    time_t contract_date = 0;
    contract_error_t err = read_agreement(fp_in, &agreement, &contract_date);
    EXPECT_EQ(err, ERROR_READ_FORMAT);

    fclose(fp_in);
}

TEST(ReadAgreement, WrongContractDateFormat) {
    char input[BUF_SIZE] = "100\n2022-03-16\n";
    FILE *fp_in = fmemopen(input, BUF_SIZE, "r");
    ASSERT_NE(fp_in, nullptr);

    struct tm tm_contract_date = {};
    char str_contract_date[] = "2030-03-16\n";
    char *res = strptime(str_contract_date, "%Y-%m-%d", &tm_contract_date);
    if (!res) {
        fclose(fp_in);
        ASSERT_NE(res, nullptr);
    }
    time_t contract_date = mktime(&tm_contract_date);

    agreement_t agreement = {};
    contract_error_t err = read_agreement(fp_in, &agreement, &contract_date);
    EXPECT_EQ(err, ERROR_READ_FORMAT);

    fclose(fp_in);
}

TEST(ReadAgreement, Correct) {
    char input[BUF_SIZE] = "100.5\n2022-03-16\n";
    FILE *fp_in = fmemopen(input, BUF_SIZE, "r");
    ASSERT_NE(fp_in, nullptr);

    struct tm tm_contract_date = {};
    char str_contract_date[] = "2022-03-15\n";
    char *res = strptime(str_contract_date, "%Y-%m-%d", &tm_contract_date);
    if (!res) {
        fclose(fp_in);
        ASSERT_NE(res, nullptr);
    }
    time_t contract_date = mktime(&tm_contract_date);

    agreement_t agreement = {};
    contract_error_t err = read_agreement(fp_in, &agreement, &contract_date);
    EXPECT_EQ(err, ERROR_SUCCESS);

    fclose(fp_in);
}

TEST(ReadAgreements, NullPointer) {
    char input[BUF_SIZE] = "something";
    FILE *fp_in = fmemopen(input, BUF_SIZE, "r");
    ASSERT_NE(fp_in, nullptr);

    contract_error_t err = read_agreements(fp_in, nullptr, nullptr);
    EXPECT_EQ(err, ERROR_NULL_POINTER);

    fclose(fp_in);
}

TEST(ReadAgreements, WrongSizeFormat) {
    char input[BUF_SIZE] = "2b1\n10.0\n2022-03-16\n";
    FILE *fp_in = fmemopen(input, BUF_SIZE, "r");
    ASSERT_NE(fp_in, nullptr);

    agreements_t agreements = {};
    time_t contract_date = 0;
    contract_error_t err = read_agreements(fp_in, &agreements, &contract_date);
    EXPECT_EQ(err, ERROR_READ_FORMAT);

    fclose(fp_in);
}

TEST(ReadAgreements, WrongAgreementFormat) {
    char input[BUF_SIZE] = "1\n10.0\n2022.03.16\n";
    FILE *fp_in = fmemopen(input, BUF_SIZE, "r");
    ASSERT_NE(fp_in, nullptr);

    agreements_t agreements = {};
    time_t contract_date = 0;
    contract_error_t err = read_agreements(fp_in, &agreements, &contract_date);
    EXPECT_EQ(err, ERROR_READ_FORMAT);

    fclose(fp_in);
}

TEST(ReadAgreements, Correct) {
    char input[BUF_SIZE] = "1\n10.0\n2022-03-16\n";
    FILE *fp_in = fmemopen(input, BUF_SIZE, "r");
    ASSERT_NE(fp_in, nullptr);

    agreements_t agreements = {};
    time_t contract_date = 0;
    contract_error_t err = read_agreements(fp_in, &agreements, &contract_date);
    EXPECT_EQ(err, ERROR_SUCCESS);

    if (agreements.size != 1) {
        size_t size = agreements.size;
        clear_agreements(&agreements);
        fclose(fp_in);
        ASSERT_EQ(size, 1);
    }
    EXPECT_TRUE(fabs(agreements.arr[0].amount - 10.0) < 1e-2);

    struct tm tm_date = {};
    char str_date[] = "2022-03-16\n";
    char *res = strptime(str_date, "%Y-%m-%d", &tm_date);
    if (!res) {
        clear_agreements(&agreements);
        fclose(fp_in);
        ASSERT_NE(res, nullptr);
    }
    time_t date = mktime(&tm_date);
    EXPECT_EQ(agreements.arr[0].date, date);

    clear_agreements(&agreements);

    fclose(fp_in);
}

TEST(ReadContract, NullPointer) {
    char input[BUF_SIZE] = "something";
    FILE *fp_in = fmemopen(input, BUF_SIZE, "r");
    ASSERT_NE(fp_in, nullptr);

    contract_error_t err = read_contract(fp_in, nullptr);
    EXPECT_EQ(err, ERROR_NULL_POINTER);

    fclose(fp_in);
}

TEST(ReadContracts, NullPointer) {
    char input[BUF_SIZE] = "something";
    FILE *fp_in = fmemopen(input, BUF_SIZE, "r");
    ASSERT_NE(fp_in, nullptr);

    contract_error_t err = read_contracts(fp_in, nullptr);
    EXPECT_EQ(err, ERROR_NULL_POINTER);

    fclose(fp_in);
}

TEST(KeyContractors, Correct) {
    char input[BUF_SIZE] =
        "3\n"
        "type1\n"
        "1\n"
        "name1\n"
        "2000-01-01\n"
        "1\n"
        "10\n"
        "2002-01-01\n"
        "type2\n"
        "100\n"
        "name2\n"
        "2001-01-01\n"
        "0\n"
        "type3\n"
        "0.5\n"
        "name3\n"
        "2003-01-01\n"
        "0\n";

    FILE *fp_in = fmemopen(input, BUF_SIZE, "r");
    ASSERT_NE(fp_in, nullptr);

    char output[BUF_SIZE];
    memset(output, 0, BUF_SIZE);
    FILE *fp_out = fmemopen(output, BUF_SIZE, "w");
    if (!fp_out) {
        fclose(fp_in);
        ASSERT_NE(fp_out, nullptr);
    }

    contracts_t contracts = {};
    contract_error_t err = ERROR_SUCCESS;
    err = read_contracts(fp_in, &contracts);
    if (err != ERROR_SUCCESS) {
        fclose(fp_in);
        fclose(fp_out);
        ASSERT_EQ(err, ERROR_SUCCESS);
    }

    err = sort_and_print_key_contractors(fp_out, &contracts);
    EXPECT_EQ(err, ERROR_SUCCESS);

    fflush(fp_out);
    EXPECT_STREQ(output,
                 "Key contractors:\n"
                 "First  - name2\n"
                 "Second - name1\n"
                 "Third  - name3\n");

    clear_contracts(&contracts);

    fclose(fp_in);
    fclose(fp_out);
}

int main(int argc, char *argv[]) {
    ::testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}
