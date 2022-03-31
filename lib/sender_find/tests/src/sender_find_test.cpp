#include <gtest/gtest.h>
#include <time.h>

extern "C" {
#include "generators.h"
#include "sender_find.h"
#include "utils.h"
}

#ifndef TIME_ITERS
#define TIME_ITERS 3
#endif

#ifndef STRESS_ITERS
#define STRESS_ITERS 3
#endif

#define RANDOM_SEED 42
#define DOUBLE_NSEC 1000000000.0

#ifndef USERS_NUM
#define USERS_NUM 10000
#endif

#ifndef MAILS_NUM
#define MAILS_NUM 1000000
#endif

#define PERIOD_START "2022-04-01"
#define PERIOD_END "2022-10-01"

class Environment : public ::testing::Environment {
   public:
    void SetUp() override { srand(RANDOM_SEED); }
};

TEST(SenderFind, TimeMeasure) {
    mail_error_t err = {};

    users_t users = {};
    err = generate_users(&users, USERS_NUM);
    ASSERT_EQ(err, ERROR_SUCCESS);

    mails_t mails = {};
    err = generate_mails(&mails, MAILS_NUM, &users, USERS_NUM);
    if (err != ERROR_SUCCESS) {
        clear_users(&users);
        ASSERT_EQ(err, ERROR_SUCCESS);
    }

    time_t period_start = {};
    mail_error_t err_start = str_to_date(PERIOD_START, &period_start);
    time_t period_end = {};
    mail_error_t err_end = str_to_date(PERIOD_END, &period_end);
    if (err_start != ERROR_SUCCESS || err_end != ERROR_SUCCESS) {
        clear_mails(&mails);
        clear_users(&users);
        ASSERT_EQ(err, ERROR_SUCCESS);
    }
    period_t period = {period_start, period_end};

    FILE *fp = fopen("time.txt", "w");
    if (!fp) {
        clear_mails(&mails);
        clear_users(&users);
        ASSERT_NE(fp, nullptr);
    }

    struct timespec start = {};
    struct timespec finish = {};
    double elapsed_total = 0;
    for (int t = 0; t < TIME_ITERS; t++) {
        double elapsed = 0;

        user_t result = {};

        clock_gettime(CLOCK_MONOTONIC, &start);
        mail_error_t err = find_sender(&result, &mails, &users, &period);
        clock_gettime(CLOCK_MONOTONIC, &finish);

        EXPECT_EQ(err, ERROR_SUCCESS);
        elapsed = (finish.tv_sec - start.tv_sec);
        elapsed += (finish.tv_nsec - start.tv_nsec) / DOUBLE_NSEC;

        elapsed_total += elapsed;

        EXPECT_EQ(err, ERROR_SUCCESS);
    }

    fprintf(fp, "%lf\n", elapsed_total / TIME_ITERS);

    fclose(fp);
    clear_mails(&mails);
    clear_users(&users);
}

TEST(SenderFind, StressTest) {
    users_t results = {};
    results.size = STRESS_ITERS;
    results.arr = (user_t *)malloc(STRESS_ITERS * sizeof(user_t));
    ASSERT_NE(results.arr, nullptr);

    for (size_t i = 0; i < STRESS_ITERS; i++) {
        mail_error_t err = {};

        users_t users = {};
        err = generate_users(&users, USERS_NUM);
        if (err != ERROR_SUCCESS) {
            free(results.arr);
            ASSERT_EQ(err, ERROR_SUCCESS);
        }

        mails_t mails = {};
        err = generate_mails(&mails, MAILS_NUM, &users, USERS_NUM);
        if (err != ERROR_SUCCESS) {
            clear_users(&users);
            free(results.arr);
            ASSERT_EQ(err, ERROR_SUCCESS);
        }

        time_t period_start = {};
        mail_error_t err_start = str_to_date(PERIOD_START, &period_start);
        time_t period_end = {};
        mail_error_t err_end = str_to_date(PERIOD_END, &period_end);
        if (err_start != ERROR_SUCCESS || err_end != ERROR_SUCCESS) {
            clear_mails(&mails);
            clear_users(&users);
            free(results.arr);
            ASSERT_EQ(err, ERROR_SUCCESS);
        }
        period_t period = {period_start, period_end};

        user_t result = {};
        err = find_sender(&result, &mails, &users, &period);
        if (err != ERROR_SUCCESS) {
            clear_mails(&mails);
            clear_users(&users);
            free(results.arr);
            ASSERT_EQ(err, ERROR_SUCCESS);
        }

        results.arr[i] = result;

        clear_mails(&mails);
        clear_users(&users);
    }

    FILE *fp = fopen("report.txt", "w");
    if (!fp) {
        clear_users(&results);
        ASSERT_NE(fp, nullptr);
    }
    for (size_t i = 0; i < STRESS_ITERS; i++) {
        fprintf(fp, "%s\n", results.arr[i].name);
    }
    fclose(fp);

    clear_users(&results);
}

int main(int argc, char *argv[]) {
    ::testing::InitGoogleTest(&argc, argv);
    ::testing::AddGlobalTestEnvironment(new Environment);
    return RUN_ALL_TESTS();
}
