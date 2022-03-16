#pragma once

#include <stdio.h>
#include <stdlib.h>

#include "errors.h"

#define CHUNK_SIZE 64

#define MAX_DOUBLE_LEN 15
#define DOUBLE_SCANF_FORMAT "%15[.0-9-]%c"

#define DATE_LEN 10
#define DATE_SCANF_FORMAT "%10[0-9-]%c"

typedef struct {
    char *arr;
    size_t size;
} string_t;

int cmp_strings(const string_t *str_first, const string_t *str_second);
void clear_string(string_t *str);

contract_error_t read_string(FILE *fp_in, string_t *str);
contract_error_t read_double(FILE *fp_in, double *d);
contract_error_t read_date(FILE *fp_in, time_t *date);
