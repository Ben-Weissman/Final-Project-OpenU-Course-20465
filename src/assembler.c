#include <ctype.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>


#include "../headers/constants.h"
#include "../headers/preprocessor.h"
#include "../headers/first_pass.h"


int main(int argc, char *argv[]) {
    char *source_file;
    int preprocessor_error_status = OFF, i;
    MacroList *macro_list;
    /* No input */
    if (argc < 2) {
        printf("No files were provided.\n");
        exit(1);
    }

    /* Get next string in the command line */
    for (i = 1; i < argc; i++) {
        printf("Start process of file named:  %s\n", argv[i]);
        preprocessor_error_status = OFF;
        source_file = malloc(strlen(argv[i]) + 1);
        macro_list = new_macro_list();
        if (source_file == NULL || macro_list == NULL) {
            free(source_file);
            free_macro_list(macro_list);
            fprintf(stderr, "Failed to allocate resources for file %s\n", argv[i]);
            continue;
        }
        strcpy(source_file, argv[i]);
        run_preprocessor(argv[i], macro_list, &preprocessor_error_status);
        if (preprocessor_error_status != OFF) {
            free_macro_list(macro_list);
            free(source_file);
            continue;
        }
        strcpy(source_file, argv[i]);
        first_pass(source_file, macro_list);
        free_macro_list(macro_list);
        free(source_file);
        printf("End of process for file named:  %s\n", argv[i]);
    }
    return 0;
}
