#include <sys/mman.h>
#include <sys/wait.h>
#include <unistd.h>

#include "sender_find.h"

#define MAX_USERS_NUM 10000

typedef struct {
    size_t begin;
    size_t num;
    size_t mails_nums[MAX_USERS_NUM];
} data_t;

static void collect_data(user_t *result, const users_t *users, data_t *shared_data, size_t proc_num) {
    size_t max_num = 0;
    for (size_t i = 0; i < users->size; i++) {
        user_t cur_user = users->arr[i];
        size_t mails_num = 0;
        for (size_t j = 0; j < proc_num; j++) {
            mails_num += shared_data[j].mails_nums[i];
        }
        if (mails_num > max_num) {
            strncpy(result->name, cur_user.name, USERNAME_SIZE);
            max_num = mails_num;
        }
    }
}

static void process_mails(data_t *shared_data, const mails_t *mails, const users_t *users, const period_t *period) {
    for (size_t i = 0; i < users->size; i++) {
        user_t cur_user = users->arr[i];
        size_t mails_num = 0;
        for (size_t j = shared_data->begin; j < shared_data->begin + shared_data->num; j++) {
            mail_t cur_mail = mails->arr[j];
            if (((cmp_dates(&period->date_start, &cur_mail.date) >= 0) &&
                 (cmp_dates(&cur_mail.date, &period->date_end) >= 0)) &&
                is_same_users(&cur_user, &cur_mail.sender)) {
                mails_num++;
            }
        }
        shared_data->mails_nums[i] = mails_num;
    }
}

static void init_shared_data(data_t *shared_data, const mails_t *mails, size_t proc_num, size_t required_num) {
    size_t begin = 0;
    for (size_t i = 0; i < proc_num; i++) {
        size_t cur_num = required_num;
        if (i == proc_num - 1) {
            cur_num = mails->size - begin;
        }

        shared_data[i] = (data_t){.begin = begin, .num = cur_num};

        memset(shared_data[i].mails_nums, 0, MAX_USERS_NUM * sizeof(size_t));

        begin += cur_num;
    }
}

mail_error_t find_sender(user_t *result, const mails_t *mails, const users_t *users, const period_t *period) {
    if (!result || !mails || !users || !period) {
        return ERROR_NULL_POINTER;
    }

    long value = sysconf(_SC_NPROCESSORS_ONLN);
    if (value == -1) {
        return ERROR_SYSCONF;
    }
    size_t proc_num = value;
    if (proc_num > mails->size) {
        proc_num = mails->size;
    }
    size_t required_num = mails->size / proc_num;

    data_t *shared_data =
        mmap(NULL, proc_num * sizeof(data_t), PROT_READ | PROT_WRITE, MAP_SHARED | MAP_ANONYMOUS, -1, 0);
    if (shared_data == MAP_FAILED) {
        return ERROR_MAP;
    }

    pid_t *child_pids = malloc(proc_num * sizeof(pid_t));
    if (!child_pids) {
        munmap(shared_data, proc_num);
        return ERROR_MEMORY_ALLOCATION;
    }

    init_shared_data(shared_data, mails, proc_num, required_num);

    for (size_t i = 0; i < proc_num; i++) {
        child_pids[i] = fork();

        if (child_pids[i] == -1) {
            free(child_pids);
            munmap(shared_data, proc_num);
            return ERROR_FORK;
        }

        if (!child_pids[i]) {
            process_mails(&shared_data[i], mails, users, period);
            free(child_pids);
            exit(EXIT_SUCCESS);
        }
    }

    for (size_t i = 0; i < proc_num; i++) {
        waitpid(child_pids[i], NULL, 0);
    }
    free(child_pids);

    collect_data(result, users, shared_data, proc_num);

    munmap(shared_data, proc_num);

    return ERROR_SUCCESS;
}
