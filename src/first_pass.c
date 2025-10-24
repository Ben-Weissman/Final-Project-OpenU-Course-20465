#include "../headers/first_pass.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "../headers/constants.h"
#include "../headers/data_structures.h"
#include "../headers/error_handling.h"
#include "../headers/machine_code.h"
#include "../headers/parser.h"
#include "../headers/second_pass.h"
#include "../headers/utilities.h"

void first_pass(char *name_of_file, MacroList *list_of_macro) {
    int line_number = 1, label_value, cur_word_type, error_code, error_flag = OFF;
    int entries_flag = OFF, ext_flag = OFF;
    char *cur_line, *token, *label_name, *file_name_copy;

    FILE *src_file;
    Array *DC_array, *IC_array;
    LabelTable *label_table, *entries_table;
    ExtList *ext_list;

    /* Add .as file ending */
    file_name_copy = malloc(strlen(name_of_file) + strlen(AM_FILE_ENDING) + 1);
    if (file_name_copy == NULL) {
        return;
    }
    strcpy(file_name_copy, name_of_file);
    strcat(file_name_copy, AM_FILE_ENDING);
    error_code = OFF;
    label_table = new_label_table();
    entries_table = new_label_table();
    ext_list = new_ext_list();
    DC_array = new_array(START_SIZE);
    IC_array = new_array(START_SIZE);
    if (label_table == NULL || entries_table == NULL || ext_list == NULL || DC_array == NULL || IC_array == NULL) {
        free_label_table(label_table);
        free_label_table(entries_table);
        free_ext_list(ext_list);
        free_array(DC_array);
        free_array(IC_array);
        free(file_name_copy);
        return;
    }
    src_file = open_file(file_name_copy, READ_MODE);
    cur_line = malloc(MAX_LENGTH_LINE);
    if (src_file == NULL || cur_line == NULL) {
        free(cur_line);
        free(file_name_copy);
        end_cycle(src_file, IC_array, DC_array, label_table, entries_table, ext_list, NULL);
        return;
    }

    while (fgets(cur_line,MAX_LENGTH_LINE, src_file) != NULL) {
        error_code = OFF;
        /* Get first word type */
        token = strtok(cur_line, DELIM_WHITESPACE);
        cur_word_type = get_next_word_type(token);

        /* Label -> */
        if (cur_word_type == IS_LABEL) {
            /* Test label name */
            label_name = duplicate_string(token);
            if (label_name == NULL ||
                is_label_name_valid(label_name, list_of_macro, label_table, &error_code) == FALSE) {
                handle_error(&error_code, line_number);
                error_flag = ON;
                free(label_name);
                continue; /* to next line in file */
            }

            /* Get next word */
            token = strtok(NULL, DELIM_WHITESPACE);
            cur_word_type = get_next_word_type(token);

            /* Label -> Instruction */
            if (cur_word_type == IS_INSTRUCTION) {
                label_value = get_index_special_array(DC_array);
                switch (get_instruction(token)) {
                    case IS_DOT_DATA:
                        handle_dot_data_case(token, DC_array, &error_code);
                        if (error_code != OFF) {
                            handle_error(&error_code, line_number);
                            error_flag = ON;
                            free(label_name);
                            line_number++;
                            continue;
                        }
                        add_to_label_table(label_table, label_name, DATA, label_value, &error_code);
                        break;
                    case IS_DOT_STRING:
                        handle_dot_string_case(token, DC_array, &error_code);
                        if (error_code != OFF) {
                            handle_error(&error_code, line_number);
                            error_flag = ON;
                            free(label_name);
                            line_number++;
                            continue;
                        }
                        add_to_label_table(label_table, label_name, DATA, label_value, &error_code);
                        break;
                    case IS_DOT_ENTRY:
                        handle_dot_entry_case(token, entries_table, NIL, &error_code);
                        entries_flag = ON;
                        break;
                    case IS_DOT_EXTERN:
                        handle_dot_extern_case(token, label_table, ext_list, &error_code);
                        ext_flag = ON;
                        break;
                    default:
                        error_code = INVALID_ACTION;
                        break;
                }
            }
            /* Label -> command */
            else if (cur_word_type == IS_COMMAND) {
                label_value = get_index_special_array(IC_array);
                handle_command_case(token, IC_array, &error_code);
                if (error_code != OFF) {
                    handle_error(&error_code, line_number);
                    error_flag = ON;
                    free(label_name);
                    line_number++;
                    continue;
                }
                add_to_label_table(label_table, label_name, CODE, label_value, &error_code);
            } else {
                error_code = INVALID_ACTION;
            }
            free(label_name);
        }

        /* Instruction -> */
        else if (cur_word_type == IS_INSTRUCTION) {
            switch (get_instruction(token)) {
                case IS_DOT_DATA:
                    handle_dot_data_case(token, DC_array, &error_code);
                    break;
                case IS_DOT_STRING:
                    handle_dot_string_case(token, DC_array, &error_code);
                    break;
                case IS_DOT_EXTERN:
                    ext_flag = ON;
                    handle_dot_extern_case(token, label_table, ext_list, &error_code);
                    break;
                case IS_DOT_ENTRY:
                    entries_flag = ON;
                    handle_dot_entry_case(token, entries_table, NIL, &error_code);
                    break;
                default:
                    error_code = INVALID_ACTION;
                    break;
            }
        }
        /* Command -> */
        else if (cur_word_type == IS_COMMAND) {
            handle_command_case(token, IC_array, &error_code);
        }
        /* Comment or empty line */
        else if (cur_word_type == IS_EMPTY_LINE || cur_word_type == IS_COMMENT_LINE) {
            line_number++;
            continue;
        } else {
            error_code = INVALID_ACTION;
        }

        if (error_code != OFF) {
            handle_error(&error_code, line_number);
            error_flag = ON;
            line_number++;
            continue;
        }
        line_number++;
    }
    if (error_flag == ON) {
        /* A fault was found -> do not create files */
        printf("A fault was found at this file, cannot proceed with the assembler.\nTrying next file (if exists).\n");
    } else {
        update_label_table(label_table, (get_index_special_array(IC_array)));
        second_pass(IC_array, DC_array, label_table, entries_table, ext_list, entries_flag, ext_flag, name_of_file,
                    &error_code);
    }
    end_cycle(src_file, IC_array, DC_array, label_table, entries_table, ext_list, cur_line);
    free(file_name_copy);
}

void handle_dot_entry_case(char *string, LabelTable *table, short value, int *error_code) {
    string = strtok(NULL, DELIM_WHITESPACE);
    if (string == NULL) {
        *error_code = INVALID_ACTION;
        return;
    }
    remove_spaces(string);
    add_to_label_table(table, string, ENTRY, value, error_code);
}

void handle_dot_extern_case(char *string, LabelTable *label_table, ExtList *ext_list, int *error_code) {
    string = strtok(NULL, DELIM_WHITESPACE);
    if (string == NULL) {
        *error_code = INVALID_ACTION;
        return;
    }
    remove_spaces(string);
    add_to_label_table(label_table, string, EXTERNAL, NIL, error_code);
    add_to_ext_list(ext_list, string);
}

void handle_dot_string_case(char *string, Array *DC_arr, int *error_code) {
    int i, len_without_quotation;

    /* Get string part & clean it */
    string = strtok(NULL, DELIM_NEWLINE);
    if (string == NULL) {
        /* Empty string */
        *error_code = INVALID_STRING;
        return;
    }
    remove_spaces(string);

    /* Test start with " and ends with " */
    if ((strlen(string) < 2) || (string[0] != CHAR_QUOTATION) || (
            string[strlen(string) - 1] != CHAR_QUOTATION)) {
        /* Invalid format */
        *error_code = INVALID_STRING;
        return;
    }
    len_without_quotation = strlen(string) - 2; /* Only inside part */
    for (i = 1; i <= len_without_quotation; i++) {
        /* Insert each char to DC array */
        insert_to_DC_array(DC_arr, (short) string[i]);
    }
    /* Insert null terminator at the end */
    insert_to_DC_array(DC_arr, (short) NULL_TERMINATOR);
}


void handle_dot_data_case(char *string, Array *DC_arr, int *error_code) {
    int parsed_data_arr_size, i;
    short *parsed_data_arr;
    /* Get data part */
    string = strtok(NULL, DELIM_NEWLINE);
    parsed_data_arr = get_data(string, error_code, &parsed_data_arr_size);

    /* Error in getting data */
    if (*error_code != OFF) {
        return;
    }
    /* Insert each number from the parsed data to DC array */
    for (i = 0; i < parsed_data_arr_size; i++) {
        insert_to_DC_array(DC_arr, parsed_data_arr[i]);
    }
    free(parsed_data_arr);
}

void handle_command_case(char *token, Array *IC_arr, int *error_code) {
    short cmd_code, src_adrs_method, target_adrs_method;
    char *src_op = strtok(NULL, DELIM_COMMA);
    char *target_op = strtok(NULL, DELIM_NEWLINE);

    /* Parse values */
    cmd_code = get_cmd(token);
    src_adrs_method = get_adrs_method(src_op, error_code);
    target_adrs_method = get_adrs_method(target_op, error_code);
    if (*error_code != OFF) {
        return;
    }
    /* Analyze and test values */
    if (is_cmd_args_valid(src_adrs_method, target_adrs_method, cmd_code, error_code)) {
        /* Addressing method of the operands for the current command is valid */
        /* Create first machine code word */
        machine_code(cmd_code, src_adrs_method, target_adrs_method, IC_arr);

        /* Both are registers addressing methods */
        if (both_operands_are_registers(src_adrs_method, target_adrs_method)) {
            machine_code_double_registers(src_op, target_op, IC_arr);
        }

        /* 2 more machine code words, one for each operand */
        else if (target_adrs_method != UNUSED_OPERARND && src_adrs_method != UNUSED_OPERARND) {
            machine_extra_code(src_op, src_adrs_method, IC_arr, SRC_OP);
            machine_extra_code(target_op, target_adrs_method, IC_arr, TARGET_OP);
        } else {
            /* Only one operand */
            machine_extra_code(src_op, src_adrs_method, IC_arr,TARGET_OP);
        }
    } /* Else is not needed , will return to calling function to handle error's */
}
