#include "data_read.h"

mail_error_t read_data(FILE *fp_in, users_t *users, mails_t *mails, period_t *period) {
    mail_error_t err = {0};

    users_t tmp_users = {0};
    err = read_users(fp_in, &tmp_users);
    if (err != ERROR_SUCCESS) {
        return err;
    }

    mails_t tmp_mails = {0};
    err = read_mails(fp_in, &tmp_mails);
    if (err != ERROR_SUCCESS) {
        clear_users(&tmp_users);
        return err;
    }

    period_t tmp_period = {0};
    err = read_date(fp_in, &tmp_period.date_start);
    if (err != ERROR_SUCCESS) {
        clear_mails(&tmp_mails);
        clear_users(&tmp_users);
        return err;
    }

    err = read_date(fp_in, &tmp_period.date_end);
    if (err != ERROR_SUCCESS) {
        clear_mails(&tmp_mails);
        clear_users(&tmp_users);
        return err;
    }

    *users = tmp_users;
    *mails = tmp_mails;
    *period = tmp_period;

    return ERROR_SUCCESS;
}
