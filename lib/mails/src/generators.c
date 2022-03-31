#include "generators.h"

mail_error_t generate_users(users_t *users, size_t users_num) {
    if (!users) {
        return ERROR_NULL_POINTER;
    }

    users_t tmp_users = {0};
    tmp_users.size = users_num;
    tmp_users.arr = malloc(users_num * sizeof(user_t));
    if (!tmp_users.arr) {
        return ERROR_MEMORY_ALLOCATION;
    }

    char buf[USERNAME_SIZE];
    for (size_t i = 0; i < users_num; i++) {
        snprintf(buf, USERNAME_SIZE, "%zu", i);
        strncpy(tmp_users.arr[i].name, buf, USERNAME_SIZE);
    }

    *users = tmp_users;

    return ERROR_SUCCESS;
}

mail_error_t generate_mails(mails_t *mails, size_t mails_num, const users_t *users, size_t users_num) {
    if (!mails || !users) {
        return ERROR_NULL_POINTER;
    }

    mails_t tmp_mails = {0};
    tmp_mails.size = mails_num;
    tmp_mails.arr = malloc(mails_num * sizeof(mail_t));
    if (!tmp_mails.arr) {
        return ERROR_MEMORY_ALLOCATION;
    }

    for (size_t i = 0; i < mails_num; i++) {
        mail_t *cur_mail = &tmp_mails.arr[i];
        strncpy(cur_mail->sender.name, users->arr[rand() % users_num].name, USERNAME_SIZE);
        strncpy(cur_mail->subject, SUBJECT, SUBJECT_SIZE);
        strncpy(cur_mail->body, BODY, BODY_SIZE);

        cur_mail->recipients.size = 1;
        cur_mail->recipients.arr = malloc(sizeof(user_t));
        if (!cur_mail->recipients.arr) {
            for (size_t j = 0; j < i; j++) {
                clear_users(&tmp_mails.arr[j].recipients);
            }
            free(tmp_mails.arr);
            return ERROR_MEMORY_ALLOCATION;
        }
        strncpy(cur_mail->recipients.arr[0].name, RECIPIENT_NAME, USERNAME_SIZE);

        char buf[DATE_LEN + 2];
        int month = rand() % MONTHS_NUM + 1;
        if (month < 10) {
            snprintf(buf, DATE_LEN + 2, DATE_FORMAT_ONE, month);
        } else {
            snprintf(buf, DATE_LEN + 2, DATE_FORMAT_TWO, month);
        }
        mail_error_t err = str_to_date(buf, &cur_mail->date);
        if (err != ERROR_SUCCESS) {
            for (size_t j = 0; j <= i; j++) {
                clear_users(&tmp_mails.arr[j].recipients);
            }
            free(tmp_mails.arr);
            return err;
        }
    }

    *mails = tmp_mails;

    return ERROR_SUCCESS;
}
