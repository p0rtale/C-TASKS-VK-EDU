#pragma once

#include <stdio.h>
#include <stdlib.h>

#include "errors.h"

#define READ_STRING_BUF_SIZE 100

#define DATE_LEN 10
#define DATE_SCANF_FORMAT "%10[0-9-]%c"

int cmp_dates(const time_t *date_first, const time_t *date_second);

mail_error_t read_string(FILE *fp_in, char *str, size_t max_size);
mail_error_t read_date(FILE *fp_in, time_t *date);
mail_error_t str_to_date(const char *str_date, time_t *date);
