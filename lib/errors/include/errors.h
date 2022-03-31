#pragma once

typedef enum {
    ERROR_SUCCESS,
    ERROR_NULL_POINTER,
    ERROR_MEMORY_ALLOCATION,
    ERROR_STREAM,
    ERROR_FORMAT,
    ERROR_SYSCONF,
    ERROR_MAP,
    ERROR_FORK
} mail_error_t;
