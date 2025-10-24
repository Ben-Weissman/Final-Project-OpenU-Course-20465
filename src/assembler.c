#include <ctype.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>


#include "../headers/constants.h"
#include "../headers/preprocessor.h"
#include "../headers/first_pass.h"


int main(int argc, char *argv[]) {
    char *src_file;
    int pre_proccesor_error_status = OFF, i;
    MacroList *list_of_macro;
    /* No input */
    if (argc < 2) {
        printf("No files were provided.\n");
        exit(1);
    }

    /* Get next string in the command line */
    for (i = 1; i < argc; i++) {
        printf("Start proccess of file named:  %s\n",argv[i]);
        pre_proccesor_error_status = OFF;
        src_file = malloc(strlen(argv[i]) + 1);
        list_of_macro = new_macro_list();
        if (src_file == NULL || list_of_macro == NULL) {
            free(src_file);
            free_macro_list(list_of_macro);
            fprintf(stderr, "Failed to allocate resources for file %s\n", argv[i]);
            continue;
        }
        strcpy(src_file, argv[i]);
        pre_proccesor(argv[i], list_of_macro, &pre_proccesor_error_status);
        if (pre_proccesor_error_status != OFF) {
            free_macro_list(list_of_macro);
            free(src_file);
            continue;
        }
        strcpy(src_file, argv[i]);
        first_pass(src_file, list_of_macro);
        free_macro_list(list_of_macro);
        free(src_file);
        printf("End of proccess for file named:  %s\n",argv[i]);
    }
    return 0;
}
