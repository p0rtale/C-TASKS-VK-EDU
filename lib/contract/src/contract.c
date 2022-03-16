#include "contract.h"

contract_error_t read_agreement(FILE *fp_in, agreement_t *agreement, const time_t *contract_date) {
    if (!agreement || !contract_date) {
        return ERROR_NULL_POINTER;
    }

    contract_error_t err = ERROR_SUCCESS;

    double amount = 0;
    err = read_double(fp_in, &amount);
    if (err != ERROR_SUCCESS) {
        return err;
    }
    if (amount < 0) {
        return ERROR_READ_FORMAT;
    }

    time_t date = {0};
    err = read_date(fp_in, &date);
    if (err != ERROR_SUCCESS) {
        return err;
    }

    if (difftime(*contract_date, date) > 0) {
        return ERROR_READ_FORMAT;
    }

    agreement->amount = amount;
    agreement->date = date;

    return ERROR_SUCCESS;
}

static int cmp_agreements(const void *agreement_first, const void *agreement_second) {
    time_t date_first = ((agreement_t *)agreement_first)->date;
    time_t date_second = ((agreement_t *)agreement_second)->date;

    double diff = difftime(date_second, date_first);
    if (diff > 0) {
        return 1;
    } else if (diff < 0) {
        return -1;
    }
    return 0;
}

contract_error_t read_agreements(FILE *fp_in, agreements_t *agreements, const time_t *contract_date) {
    if (!agreements || !contract_date) {
        return ERROR_NULL_POINTER;
    }

    char last_symb = 0;
    int size = 0;
    int res = fscanf(fp_in, "%9d%c", &size, &last_symb);
    if (res == EOF || res != 2 || last_symb != '\n' || size < 0) {
        return ERROR_READ_FORMAT;
    }

    agreements_t tmp_agreements = {0};
    tmp_agreements.size = size;
    if (size == 0) {
        agreements->size = 0;
        agreements->arr = NULL;
        return ERROR_SUCCESS;
    }
    tmp_agreements.arr = malloc(size * sizeof(agreement_t));
    if (!tmp_agreements.arr) {
        return ERROR_MEMORY_ALLOCATION;
    }
    for (int i = 0; i < size; i++) {
        contract_error_t err = read_agreement(fp_in, &tmp_agreements.arr[i], contract_date);
        if (err != ERROR_SUCCESS) {
            clear_agreements(&tmp_agreements);
            return err;
        }
    }

    *agreements = tmp_agreements;
    qsort(agreements->arr, agreements->size, sizeof(agreement_t), cmp_agreements);

    return ERROR_SUCCESS;
}

void clear_agreements(agreements_t *agreements) {
    if (!agreements) {
        return;
    }
    free(agreements->arr);
}

contract_error_t read_contract(FILE *fp_in, contract_t *contract) {
    if (!contract) {
        return ERROR_NULL_POINTER;
    }

    contract_error_t err = ERROR_SUCCESS;

    string_t type = {0};
    err = read_string(fp_in, &type);
    if (err != ERROR_SUCCESS) {
        return err;
    }

    double amount = 0;
    err = read_double(fp_in, &amount);
    if (err != ERROR_SUCCESS) {
        clear_string(&type);
        return err;
    }
    if (amount < 0) {
        clear_string(&type);
        return ERROR_READ_FORMAT;
    }

    string_t contractor_name = {0};
    err = read_string(fp_in, &contractor_name);
    if (err != ERROR_SUCCESS) {
        clear_string(&type);
        return err;
    }

    time_t date = {0};
    err = read_date(fp_in, &date);
    if (err != ERROR_SUCCESS) {
        free(type.arr);
        free(contractor_name.arr);
        return err;
    }

    agreements_t agreements = {0};
    err = read_agreements(fp_in, &agreements, &date);
    if (err != ERROR_SUCCESS) {
        free(type.arr);
        free(contractor_name.arr);
        return err;
    }

    contract->type = type;
    contract->amount = amount;
    contract->contractor_name = contractor_name;
    contract->date = date;
    contract->agreements = agreements;

    return ERROR_SUCCESS;
}

void clear_contract(contract_t *contract) {
    if (!contract) {
        return;
    }
    clear_string(&contract->type);
    clear_string(&contract->contractor_name);
    clear_agreements(&contract->agreements);
}

contract_error_t read_contracts(FILE *fp_in, contracts_t *contracts) {
    if (!contracts) {
        return ERROR_NULL_POINTER;
    }

    char last_symb = 0;
    int size = 0;
    int res = fscanf(fp_in, "%9d%c", &size, &last_symb);
    if (res == EOF || res != 2 || last_symb != '\n' || size < 0) {
        return ERROR_READ_FORMAT;
    }

    contracts_t tmp_contracts = {0};
    tmp_contracts.size = size;
    if (size != 0) {
        tmp_contracts.arr = malloc(size * sizeof(contract_t));
        if (!tmp_contracts.arr) {
            return ERROR_MEMORY_ALLOCATION;
        }
    } else {
        tmp_contracts.arr = NULL;
    }
    for (int i = 0; i < size; i++) {
        contract_error_t err = read_contract(fp_in, &tmp_contracts.arr[i]);
        if (err != ERROR_SUCCESS) {
            clear_contracts(&tmp_contracts);
            return err;
        }
    }

    *contracts = tmp_contracts;

    return ERROR_SUCCESS;
}

void clear_contracts(contracts_t *contracts) {
    if (!contracts) {
        return;
    }
    for (size_t i = 0; i < contracts->size; i++) {
        clear_contract(&contracts->arr[i]);
    }
    free(contracts->arr);
}

static int cmp_contracts(const void *contract_first, const void *contract_second) {
    string_t name_first = ((contract_t *)contract_first)->contractor_name;
    string_t name_second = ((contract_t *)contract_second)->contractor_name;

    return cmp_strings(&name_first, &name_second);
}

contract_error_t sort_and_print_key_contractors(FILE *fp_out, contracts_t *contracts) {
    qsort(contracts->arr, contracts->size, sizeof(contract_t), cmp_contracts);

    string_t first_name = {0};
    double first_sum = -1.0;

    string_t second_name = {0};
    double second_sum = -1.0;

    string_t third_name = {0};
    double third_sum = -1.0;

    size_t i = 0;
    while (i < contracts->size) {
        double cur_sum = 0.0;
        string_t cur_name = contracts->arr[i].contractor_name;
        while (i < contracts->size && cmp_strings(&cur_name, &contracts->arr[i].contractor_name) == 0) {
            if (contracts->arr[i].agreements.size > 0) {
                cur_sum += contracts->arr[i].agreements.arr[0].amount;
            } else {
                cur_sum += contracts->arr[i].amount;
            }
            i++;
        }

        if (cur_sum > first_sum) {
            third_sum = second_sum;
            third_name = second_name;
            second_sum = first_sum;
            second_name = first_name;
            first_sum = cur_sum;
            first_name = cur_name;
        } else if (cur_sum > second_sum) {
            third_sum = second_sum;
            third_name = second_name;
            second_sum = cur_sum;
            second_name = cur_name;
        } else if (cur_sum > third_sum) {
            third_sum = cur_sum;
            third_name = cur_name;
        }
    }

    fprintf(fp_out,
            "Key contractors:\n"
            "First  - %s\n"
            "Second - %s\n"
            "Third  - %s\n",
            first_name.arr, second_name.arr, third_name.arr);

    return ERROR_SUCCESS;
}
