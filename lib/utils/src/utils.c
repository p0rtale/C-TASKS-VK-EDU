#define _XOPEN_SOURCE 700

#include "utils.h"

#include <string.h>
#include <time.h>

#include "errors.h"

int cmp_dates(const time_t *date_first, const time_t *date_second) {
    if (!date_first || !date_second) {
        return 0;
    }

    double diff = difftime(*date_second, *date_first);
    if (diff > 0) {
        return 1;
    } else if (diff < 0) {
        return -1;
    }
    return 0;
}

mail_error_t read_string(FILE *fp_in, char *str, size_t max_size) {
    if (!str) {
        return ERROR_NULL_POINTER;
    }

    char buf[READ_STRING_BUF_SIZE];
    if (!fgets(buf, max_size, fp_in)) {
        if (ferror(fp_in)) {
            return ERROR_STREAM;
        }
    }
    size_t newline_pos = strcspn(buf, "\n");
    if (newline_pos == max_size - 1) {
        return ERROR_FORMAT;
    }

    size_t str_size = newline_pos + 1;
    buf[str_size - 1] = 0;
    strncpy(str, buf, str_size);

    return ERROR_SUCCESS;
}

mail_error_t read_date(FILE *fp_in, time_t *date) {
    if (!date) {
        return ERROR_NULL_POINTER;
    }

    char str_date[DATE_LEN + 1];
    memset(str_date, 0, DATE_LEN + 1);

    char last_symb = 0;

    int res = fscanf(fp_in, DATE_SCANF_FORMAT, str_date, &last_symb);
    if (res == EOF || res != 2 || last_symb != '\n') {
        return ERROR_FORMAT;
    }

    mail_error_t err = str_to_date(str_date, date);
    if (err != ERROR_SUCCESS) {
        return err;
    }

    return ERROR_SUCCESS;
}

mail_error_t str_to_date(const char *str_date, time_t *date) {
    if (!str_date || !date) {
        return ERROR_NULL_POINTER;
    }

    struct tm tm_date = {0};
    if (!strptime(str_date, "%Y-%m-%d", &tm_date)) {
        return ERROR_FORMAT;
    }
    time_t tmp_date = mktime(&tm_date);

    char new_str_date[DATE_LEN + 1];
    memset(new_str_date, 0, DATE_LEN + 1);
    strftime(new_str_date, DATE_LEN + 1, "%Y-%m-%d", &tm_date);

    if (strncmp(str_date, new_str_date, DATE_LEN) != 0) {
        return ERROR_FORMAT;
    }

    *date = tmp_date;

    return ERROR_SUCCESS;
}
