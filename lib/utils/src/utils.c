#define _XOPEN_SOURCE 700

#include "utils.h"

#include <string.h>
#include <time.h>

#include "errors.h"

int cmp_strings(const string_t *str_first, const string_t *str_second) {
    if (!str_first || !str_second) {
        return 0;
    }

    if (str_first->size == str_second->size) {
        return strncmp(str_first->arr, str_second->arr, str_first->size);
    } else if (str_first->size > str_second->size) {
        return 1;
    }
    return -1;
}

void clear_string(string_t *str) {
    if (!str) {
        return;
    }
    free(str->arr);
}

contract_error_t read_string(FILE *fp_in, string_t *str) {
    if (!str) {
        return ERROR_NULL_POINTER;
    }

    char *input = NULL;
    size_t input_len = 0;

    char buf[CHUNK_SIZE];
    size_t temp_len = 0;
    do {
        if (!fgets(buf, CHUNK_SIZE, fp_in)) {
            if (ferror(fp_in)) {
                free(input);
                return ERROR_STREAM;
            }

            str->arr = input;
            str->size = input_len + 1;

            return ERROR_SUCCESS;
        }
        buf[strcspn(buf, "\n")] = 0;
        temp_len = strnlen(buf, CHUNK_SIZE);

        char *new_input = realloc(input, input_len + temp_len + 1);
        if (!new_input) {
            free(input);
            return ERROR_MEMORY_ALLOCATION;
        }
        input = new_input;

        strncpy(input + input_len, buf, temp_len + 1);
        input_len += temp_len;
    } while (temp_len == (CHUNK_SIZE - 1) && buf[CHUNK_SIZE - 2] != '\n');

    str->arr = input;
    str->size = input_len + 1;

    return ERROR_SUCCESS;
}

contract_error_t read_double(FILE *fp_in, double *d) {
    if (!d) {
        return ERROR_NULL_POINTER;
    }

    char str_double[MAX_DOUBLE_LEN + 1];
    memset(str_double, 0, MAX_DOUBLE_LEN + 1);

    char last_symb = 0;

    int res = fscanf(fp_in, DOUBLE_SCANF_FORMAT, str_double, &last_symb);
    if (res == EOF || res != 2 || last_symb != '\n') {
        return ERROR_READ_FORMAT;
    }
    char *endptr = NULL;
    *d = strtod(str_double, &endptr);
    if (*endptr) {
        return ERROR_READ_FORMAT;
    }

    return ERROR_SUCCESS;
}

contract_error_t read_date(FILE *fp_in, time_t *date) {
    if (!date) {
        return ERROR_NULL_POINTER;
    }

    char str_date[DATE_LEN + 1];
    memset(str_date, 0, DATE_LEN + 1);

    char last_symb = 0;

    int res = fscanf(fp_in, DATE_SCANF_FORMAT, str_date, &last_symb);
    if (res == EOF || res != 2 || last_symb != '\n') {
        return ERROR_READ_FORMAT;
    }

    struct tm tm_date = {0};
    if (!strptime(str_date, "%Y-%m-%d", &tm_date)) {
        return ERROR_READ_FORMAT;
    }
    time_t temp_date = mktime(&tm_date);

    char new_str_date[DATE_LEN + 1];
    memset(new_str_date, 0, DATE_LEN + 1);
    strftime(new_str_date, DATE_LEN + 1, "%Y-%m-%d", &tm_date);

    if (strncmp(str_date, new_str_date, DATE_LEN) != 0) {
        return ERROR_READ_FORMAT;
    }

    *date = temp_date;

    return ERROR_SUCCESS;
}
