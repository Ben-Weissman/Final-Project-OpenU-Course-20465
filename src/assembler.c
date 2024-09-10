#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>


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
        src_file = malloc(strlen(argv[i])+10);
        list_of_macro = new_macro_list();
        strcpy(src_file, argv[i]);
        pre_proccesor(argv[i], list_of_macro, &pre_proccesor_error_status);
        if (pre_proccesor_error_status != OFF) {
            continue;
        }
        strcpy(src_file, argv[i]);
        first_pass(src_file, list_of_macro);
        free(list_of_macro);
        free(src_file);
        printf("End of proccess for file named:  %s\n",argv[i]);
    }
    return 0;
}
