#include <stdio.h>
#include <stdlib.h>

#include "contract.h"

#define HELP_MSG                                                 \
    "--------------------------------------------------------\n" \
    "| Input format:                                        |\n" \
    "|                                                      |\n" \
    "| 1. First enter the number of contracts               |\n" \
    "| 2. Then enter the contracts in the following format: |\n" \
    "|                                                      |\n" \
    "| TYPE                                                 |\n" \
    "| AMOUNT                                               |\n" \
    "| CONTRUCTOR NAME                                      |\n" \
    "| DATE as YYYY-MM-DD                                   |\n" \
    "| NUMBER OF ADDITIONAL AGREEMENTS                      |\n" \
    "| ADDITIONAL AGREEMENTS                                |\n" \
    "|                                                      |\n" \
    "| Where each agreement has the format:                 |\n" \
    "|                                                      |\n" \
    "| AMOUNT                                               |\n" \
    "| DATE as YYYY-MM-DD                                   |\n" \
    "|                                                      |\n" \
    "-------------------------------------------------------|\n\n"

int main() {
    fprintf(stdout, HELP_MSG);

    contracts_t contracts = {0};
    contract_error_t err = ERROR_SUCCESS;
    err = read_contracts(stdin, &contracts);
    if (err != ERROR_SUCCESS) {
        if (err == ERROR_READ_FORMAT) {
            fprintf(stderr, "Error: incorrect format\n");
        } else {
            fprintf(stderr, "Error: data could not be read\n");
        }
        return EXIT_FAILURE;
    }

    err = sort_and_print_key_contractors(stdout, &contracts);
    if (err != ERROR_SUCCESS) {
        fprintf(stderr, "Error: could not find key contractors\n");
        clear_contracts(&contracts);
        return EXIT_FAILURE;
    }

    clear_contracts(&contracts);

    return EXIT_SUCCESS;
}
