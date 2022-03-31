#pragma once

#include <stdio.h>
#include <string.h>
#include <time.h>

#include "errors.h"
#include "utils.h"

#define USERNAME_SIZE 32
#define SUBJECT_SIZE 128
#define BODY_SIZE 1024

typedef struct {
    char name[USERNAME_SIZE];
} user_t;

typedef struct {
    user_t *arr;
    size_t size;
} users_t;

typedef struct {
    user_t sender;
    char subject[SUBJECT_SIZE];
    char body[BODY_SIZE];
    users_t recipients;
    time_t date;
} mail_t;

typedef struct {
    mail_t *arr;
    size_t size;
} mails_t;

int is_same_users(const user_t *user_first, const user_t *user_second);
mail_error_t read_users(FILE *fp_in, users_t *users);
void clear_users(users_t *users);

mail_error_t read_mail(FILE *fp_in, mail_t *mail);
void clear_mail(mail_t *mail);

mail_error_t read_mails(FILE *fp_in, mails_t *mails);
void clear_mails(mails_t *mails);
