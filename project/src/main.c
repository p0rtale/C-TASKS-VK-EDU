#include <stdio.h>
#include <stdlib.h>

#include "data_read.h"
#include "mails.h"
#include "sender_find.h"

int main() {
    users_t users = {0};
    mails_t mails = {0};
    period_t period = {0};
    mail_error_t err = read_data(stdin, &users, &mails, &period);
    if (err != ERROR_SUCCESS) {
        if (err == ERROR_FORMAT) {
            fprintf(stderr, "Error: incorrect format\n");
        } else {
            fprintf(stderr, "Error: data could not be read\n");
        }
        return EXIT_FAILURE;
    }

    user_t result = {0};
    err = find_sender(&result, &mails, &users, &period);
    if (err != ERROR_SUCCESS) {
        fprintf(stderr, "Error: could not find sender\n");
        clear_users(&users);
        clear_mails(&mails);
        return EXIT_FAILURE;
    }

    fprintf(stdout, "Most of the mails came from: %s\n", result.name);

    clear_users(&users);
    clear_mails(&mails);

    return EXIT_SUCCESS;
}
