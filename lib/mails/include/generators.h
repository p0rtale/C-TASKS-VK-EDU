#pragma once

#include <stdio.h>
#include <stdlib.h>

#include "errors.h"
#include "mails.h"

#define SUBJECT "subject"
#define BODY "body"
#define RECIPIENT_NAME "name"
#define MONTHS_NUM 12

#define DATE_FORMAT_ONE "2022-0%d-01"
#define DATE_FORMAT_TWO "2022-%d-01"

mail_error_t generate_users(users_t *users, size_t users_num);
mail_error_t generate_mails(mails_t *mails, size_t mails_num, const users_t *users, size_t users_num);
