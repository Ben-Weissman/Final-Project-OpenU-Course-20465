#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>


#include "../headers/preprocessor.h"
#include "../headers/constants.h"
#include "../headers/data_structures.h"
#include "../headers/error_handling.h"
#include "../headers/utilities.h"


#define MACRO_START "macr"
#define MACRO_END "endmacr"
#define MAX_MEMORY_MACRO_NAME 20
#define MAX_LINES_MACRO_DEF 10

static char *instructions[] = {".data", ".string", ".entry", ".extern"};
static char *commands[] = {
    "mov", "cmp", "add", "sub", "lea", "clr", "not", "inc", "dec", "jmp", "bne", "red", "prn", "jsr", "rts", "stop"
};


int is_macro_name_valid( char *name, int *macro_error) {
    int i;

    /* Test for instruction name */
    for (i = 0; i < INSTRUCTIONS_LIST_LENGTH; i++) {
        if (strcmp(name, instructions[i]) == EQUAL) {
            *macro_error = MACRO_NAME_IS_INSTRUCTION_NAME;
            return FALSE;
        }
    }

    /* Test for OP command name */
    for (i = 0; i < COMMANDS_LIST_LENGTH; i++) {
        if (strcmp(name, commands[i]) == EQUAL) {
            *macro_error = MACRO_NAME_IS_INSTRUCTION_NAME;
            return FALSE;
        }
    }

    /* Name is valid */
    return TRUE;
}

MacroNode *is_line_name_of_macro( char *input_string, int *macro_flag,  MacroList *list_of_macro) {
    MacroNode *temp = get_list_head(list_of_macro);
    char *string_copy = malloc(strlen(input_string) + 1);

    /* Make copy of input string and remove whitespaces */
    strcpy(string_copy, input_string);
    remove_spaces(string_copy);
    string_copy[strcspn(string_copy, DELIM_WHITESPACE)] = 0;

    /* Macro list empty -> no macros -> it's not a macro name */
    if (temp == NULL) {
        *macro_flag = IS_NOT_MACRO_NAME;
        return temp;
    }

    /* Test if the given input line is a name of macro */
    while (temp != NULL) {
        if (strncmp(string_copy, get_macro_name(temp),strlen(get_macro_name(temp))) == EQUAL) {
            /* Reset to original string & get other part */
            strcpy(string_copy,input_string);
            string_copy = strtok(string_copy,DELIM_WHITESPACE);
            string_copy = strtok(NULL,DELIM_NEWLINE);
            if (string_copy == NULL || is_empty(string_copy) == TRUE) {
                /* Rest of string is empty, so only a macro name was found */
                *macro_flag = IS_MACRO_NAME;
                return temp;
            }
            *macro_flag = IS_MACRO_NAME_WITH_EXTRA_TEXT;
            return temp;
        }
        /* Advance */
        temp = get_next_macro_node(temp);
    }
    /* No macro with the same name was found */
    *macro_flag = IS_NOT_MACRO_NAME;
    return NULL;
}


void is_line_start_of_macro_definition(char *name_of_macro,  char *input_string, int *macro_flag) {
    int result;
    char *first_word, *second_word, *rest_of_string;
    char *string_copy = malloc(strlen(input_string) + 1);

    /* save a copy of the string so strtok will not "damage" it */
    strcpy(string_copy, input_string);

    /* remove trailling of \n */
    string_copy[strcspn(string_copy, DELIM_NEWLINE)] = 0;
    /* get first word */
    first_word = strtok(string_copy, DELIM_WHITESPACE);

    if (is_empty(first_word)) {
        *macro_flag = IS_NOT_MACRO_START;
        return;
    }
    /* save comparsion of first word and "macr" */
    result = strcmp(first_word, MACRO_START);

    /* first word is not "macr" */
    if (result != EQUAL) {
        *macro_flag = IS_NOT_MACRO_START;
        return;
    }
    /* first word is "macr", token 2 more parts */
    second_word = strtok(NULL, DELIM_WHITESPACE);
    rest_of_string = strtok(NULL,DELIM_NEWLINE);

    /* check rest of string, should be whitespaces only */
    result = is_empty(rest_of_string);

    /* rest of string is empty -> valid macro start def */
    if (rest_of_string == NULL || result == 0) {
        /* Test macro name */
        result = is_macro_name_valid(second_word, macro_flag);
        if (result == TRUE) {
            /*the macro name is valid, update macro name to the second word */
            *macro_flag = IS_MACRO_START;
            strcpy(name_of_macro, second_word);
            /* free copy string */
            free(string_copy);
        }
        /* Name of macro is not valid */
        else {
            free(string_copy);
        }
    } else {
        /* rest of string is not empty, therefore not valid start of macro */
        *macro_flag = EXTRA_TEXT_AT_START_OF_MACRO;
        free(string_copy);
    }
}


void pre_proccesor(char *input_file_name, MacroList *macro_list, int *error_code) {
    int macro_flag = OFF; /* Used for various status's */
    char *macro_name, *macro_definition, *cur_line, *cur_line_copy, *new_file_name, *input_file_as_ending;

    MacroNode *temp_macro_node = NULL, *temp_node_pointer = NULL;
    FILE *input_file = NULL, *output_file = NULL;


    /* Memory allocation */
    cur_line = malloc(MAX_LENGTH_LINE); /* Current line */
    cur_line_copy = malloc(MAX_LENGTH_LINE);
    macro_name = malloc(MAX_MEMORY_MACRO_NAME);
    macro_definition = malloc(MAX_LENGTH_LINE * MAX_LINES_MACRO_DEF);
    new_file_name = malloc( strlen(input_file_name) + strlen(AM_FILE_ENDING));
    input_file_as_ending = malloc (strlen(input_file_name) + strlen(AS_FILE_ENDING));

    /* output file ending */
    strcpy(new_file_name,input_file_name);
    strcat(new_file_name, AM_FILE_ENDING); /* this wil create: ../io_files/filename.am */

    /* add .as ending to file name */
    strcat(input_file_as_ending,input_file_name);
    strcat(input_file_as_ending,AS_FILE_ENDING);

    input_file = open_file(input_file_as_ending, READ_MODE);
    output_file = open_file(new_file_name, APPEND_MODE);

    /* Read next line */
    while (fgets(cur_line, MAX_LENGTH_LINE, input_file) != NULL) {
        /* Reset macro flag */
        macro_flag = OFF;

        /* Check if current line is start of macro definition */
        is_line_start_of_macro_definition(macro_name, cur_line, &macro_flag);

        /* Current line is start of macro -> read all lines of definition */
        if (macro_flag == IS_MACRO_START) {
            while (fgets(cur_line, MAX_LENGTH_LINE, input_file) != NULL) {
                /* Duplicate current line & remove trailling of \n */
                strcpy(cur_line_copy, cur_line);
                cur_line_copy[strcspn(cur_line_copy, DELIM_NEWLINE)] = 0;

                /* Trim starting whitespace */
                while (isspace(*cur_line_copy)) {
                    cur_line_copy++;
                }

                /* Test if end of macro definition */
                if (strncmp(cur_line_copy,MACRO_END, sizeof(MACRO_END)) == EQUAL) {
                    break; /* Exit reading macro definition */
                }

                /* Not the end of definition, add the current line to the definition  */
                strcat(macro_definition, cur_line_copy);
                strcat(macro_definition, DELIM_NEWLINE);
            }

            /* Macro definition has been read, add it to the list */
            temp_macro_node = create_new_macro(macro_name, macro_definition);
            add_to_macro_list(macro_list, temp_macro_node);

            /* Reset strings */
            memset(macro_name, 0, strlen(macro_name));
            memset(macro_definition, 0, strlen(macro_definition));
        }
        /* Current line is not start of macro */
        else if (macro_flag == MACRO_NAME_IS_CMD_NAME || macro_flag == MACRO_NAME_IS_INSTRUCTION_NAME || macro_flag ==
                 EXTRA_TEXT_AT_START_OF_MACRO) {
            /* Macro error was found */
            free(macro_name);
            free(macro_definition);
            free(cur_line);
            free(new_file_name);
            free(input_file_as_ending);
            fclose(input_file);
            fclose(output_file);
            *error_code = macro_flag;
            return;
        } else {
            /* Test if current line = name of macro */
            temp_node_pointer = is_line_name_of_macro(cur_line, &macro_flag, macro_list);

            /* Current line starts with name of macro */
            if (macro_flag == IS_MACRO_NAME) {
                /* Add to the file the correspanding macro's definition */
                fprintf(output_file, "%s", get_macro_definition(temp_node_pointer));
            } else {
                /* Current line is NOT macro definition nor macro name */

                /* Make a copy of the line & "clean" it of starting whitespace */
                strcpy(cur_line_copy, cur_line);
                while (isspace(*cur_line_copy)) {
                    cur_line_copy++;
                }
                /* Add the line to the file */
                fprintf(output_file, "%s", cur_line);
            }
        }
    }
    /* End of pre_proccesor -> clean the function */
    free(macro_name);
    free(macro_definition);
    free(cur_line);
    free(new_file_name);
    free(input_file_as_ending);
    fclose(input_file);
    fclose(output_file);
}
