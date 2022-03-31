#include "sender_find.h"

mail_error_t find_sender(user_t *result, const mails_t *mails, const users_t *users, const period_t *period) {
    if (!result || !mails || !users || !period) {
        return ERROR_NULL_POINTER;
    }

    size_t max_num = 0;
    for (size_t i = 0; i < users->size; i++) {
        user_t cur_user = users->arr[i];
        size_t mails_num = 0;
        for (size_t j = 0; j < mails->size; j++) {
            mail_t cur_mail = mails->arr[j];
            if (((cmp_dates(&period->date_start, &cur_mail.date) >= 0) &&
                 (cmp_dates(&cur_mail.date, &period->date_end) >= 0)) &&
                is_same_users(&cur_user, &cur_mail.sender)) {
                mails_num++;
            }
        }
        if (mails_num > max_num) {
            strncpy(result->name, cur_user.name, USERNAME_SIZE);
            max_num = mails_num;
        }
    }

    return ERROR_SUCCESS;
}
