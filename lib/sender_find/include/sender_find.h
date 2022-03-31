#pragma once

#include <stdio.h>

#include "errors.h"
#include "mails.h"

typedef struct {
    time_t date_start;
    time_t date_end;
} period_t;

mail_error_t find_sender(user_t *result, const mails_t *mails, const users_t *users, const period_t *period);
