#pragma once

#include <stdio.h>

#include "mails.h"
#include "sender_find.h"

mail_error_t read_data(FILE *fp_in, users_t *users, mails_t *mails, period_t *period);
