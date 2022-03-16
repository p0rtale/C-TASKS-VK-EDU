#pragma once

#include <stdio.h>
#include <string.h>
#include <time.h>

#include "errors.h"
#include "utils.h"

typedef struct {
    double amount;
    time_t date;
} agreement_t;

typedef struct {
    agreement_t *arr;
    size_t size;
} agreements_t;

typedef struct {
    string_t type;
    double amount;
    string_t contractor_name;
    time_t date;
    agreements_t agreements;
} contract_t;

typedef struct {
    contract_t *arr;
    size_t size;
} contracts_t;

contract_error_t sort_and_print_key_contractors(FILE *fp_out, contracts_t *contracts);

contract_error_t read_agreement(FILE *fp_in, agreement_t *agreement, const time_t *contract_date);
contract_error_t read_agreements(FILE *fp_in, agreements_t *agreements, const time_t *contract_date);
void clear_agreements(agreements_t *agreements);

contract_error_t read_contract(FILE *fp_in, contract_t *contract);
void clear_contract(contract_t *contract);

contract_error_t read_contracts(FILE *fp_in, contracts_t *contracts);
void clear_contracts(contracts_t *contracts);
