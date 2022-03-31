#include <gtest/gtest.h>

extern "C" {
#include "mails.h"
}

TEST(CompareReports, CompareReports) {
    FILE *fp_consistent = fopen("data_consistent/report.txt", "r");
    ASSERT_NE(fp_consistent, nullptr);

    FILE *fp_parallel = fopen("data_parallel/report.txt", "r");
    if (!fp_parallel) {
        fclose(fp_consistent);
        ASSERT_NE(fp_parallel, nullptr);
    }

    bool compare_result = true;
    char name_consistent[USERNAME_SIZE];
    char name_parallel[USERNAME_SIZE];
    while (fgets(name_consistent, USERNAME_SIZE, fp_consistent) && fgets(name_parallel, USERNAME_SIZE, fp_parallel)) {
        if (strncmp(name_consistent, name_parallel, USERNAME_SIZE) != 0) {
            compare_result = false;
            break;
        }
    }
    if (ferror(fp_consistent) || ferror(fp_parallel)) {
        fclose(fp_parallel);
        fclose(fp_consistent);
        ASSERT_EQ(ferror(fp_consistent), 0);
        ASSERT_EQ(ferror(fp_parallel), 0);
    }

    EXPECT_EQ(compare_result, true);

    fclose(fp_parallel);
    fclose(fp_consistent);
}

int main(int argc, char *argv[]) {
    ::testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}
