#include "mails.h"

int is_same_users(const user_t *user_first, const user_t *user_second) {
    if (!user_first || !user_second) {
        return 0;
    }

    if (!strncmp(user_first->name, user_second->name, USERNAME_SIZE)) {
        return 1;
    }

    return 0;
}

mail_error_t read_users(FILE *fp_in, users_t *users) {
    if (!users) {
        return ERROR_NULL_POINTER;
    }

    char last_symb = 0;
    int size = 0;
    int res = fscanf(fp_in, "%9d%c", &size, &last_symb);
    if (res == EOF || res != 2 || last_symb != '\n' || size < 1) {
        return ERROR_FORMAT;
    }

    users_t tmp_users = {0};
    tmp_users.size = size;
    tmp_users.arr = malloc(size * sizeof(user_t));
    if (!tmp_users.arr) {
        return ERROR_MEMORY_ALLOCATION;
    }

    for (int i = 0; i < size; i++) {
        mail_error_t err = read_string(fp_in, tmp_users.arr[i].name, USERNAME_SIZE);
        if (err != ERROR_SUCCESS) {
            clear_users(&tmp_users);
            return err;
        }
    }

    *users = tmp_users;

    return ERROR_SUCCESS;
}

void clear_users(users_t *users) {
    if (!users) {
        return;
    }
    if (users->size > 0) {
        free(users->arr);
    }
}

mail_error_t read_mail(FILE *fp_in, mail_t *mail) {
    if (!mail) {
        return ERROR_NULL_POINTER;
    }

    mail_error_t err = ERROR_SUCCESS;

    user_t sender = {0};
    err = read_string(fp_in, sender.name, USERNAME_SIZE);
    if (err != ERROR_SUCCESS) {
        return err;
    }

    char subject[SUBJECT_SIZE];
    err = read_string(fp_in, subject, SUBJECT_SIZE);
    if (err != ERROR_SUCCESS) {
        return err;
    }

    char body[BODY_SIZE];
    err = read_string(fp_in, body, BODY_SIZE);
    if (err != ERROR_SUCCESS) {
        return err;
    }

    users_t recipients = {0};
    err = read_users(fp_in, &recipients);
    if (err != ERROR_SUCCESS) {
        return err;
    }

    time_t date = {0};
    err = read_date(fp_in, &date);
    if (err != ERROR_SUCCESS) {
        clear_users(&recipients);
        return err;
    }

    mail->sender = sender;
    strncpy(mail->subject, subject, SUBJECT_SIZE);
    strncpy(mail->body, body, BODY_SIZE);
    mail->recipients = recipients;
    mail->date = date;

    return ERROR_SUCCESS;
}

void clear_mail(mail_t *mail) {
    if (!mail) {
        return;
    }
    clear_users(&mail->recipients);
}

mail_error_t read_mails(FILE *fp_in, mails_t *mails) {
    if (!mails) {
        return ERROR_NULL_POINTER;
    }

    char last_symb = 0;
    int size = 0;
    int res = fscanf(fp_in, "%9d%c", &size, &last_symb);
    if (res == EOF || res != 2 || last_symb != '\n' || size < 0) {
        return ERROR_FORMAT;
    }

    mails_t tmp_mails = {0};
    tmp_mails.size = size;
    if (size != 0) {
        tmp_mails.arr = malloc(size * sizeof(mail_t));
        if (!tmp_mails.arr) {
            return ERROR_MEMORY_ALLOCATION;
        }
    } else {
        tmp_mails.arr = NULL;
    }
    for (size_t i = 0; i < tmp_mails.size; i++) {
        mail_error_t err = read_mail(fp_in, &tmp_mails.arr[i]);
        if (err != ERROR_SUCCESS) {
            for (size_t j = 0; j < i; j++) {
                clear_mail(&tmp_mails.arr[j]);
            }
            free(tmp_mails.arr);
            return err;
        }
    }

    *mails = tmp_mails;

    return ERROR_SUCCESS;
}

void clear_mails(mails_t *mails) {
    if (!mails) {
        return;
    }
    for (size_t i = 0; i < mails->size; i++) {
        clear_mail(&mails->arr[i]);
    }
    free(mails->arr);
}
