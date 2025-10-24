#include "../headers/parser.h"

#include <ctype.h>
#include <stdlib.h>
#include <string.h>

#include "../headers/constants.h"
#include "../headers/data_structures.h"
#include "../headers/preprocessor.h"
#include "../headers/utilities.h"
#include "../headers/error_handling.h"

static char *instructions[] = {".data", ".string", ".entry", ".extern"};
static char *commands[] = {
    "mov", "cmp", "add", "sub", "lea", "clr", "not", "inc", "dec", "jmp", "bne", "red", "prn", "jsr", "rts", "stop"
};
static char *registers[] = {"r0", "r1", "r2", "r3", "r4", "r5", "r6", "r7"};

int get_cmd(char *string) {
    int i;

    /* Get OP command from commands list */
    for (i = 0; i < COMMANDS_LIST_LENGTH; i++) {
        if (strcmp(commands[i], string) == EQUAL) {
            return i;
        }
    }
    return NIL;
}

short both_operands_are_registers(short src_op_adrs_method, short target_op_adrs_method) {
    if (src_op_adrs_method == INDIRECT_REGISTER_ADDRESSING || src_op_adrs_method == DIRECT_REGISTER_ADDRESSING) {
        if (target_op_adrs_method == INDIRECT_REGISTER_ADDRESSING || target_op_adrs_method ==
            DIRECT_REGISTER_ADDRESSING) {
            /* Same register addressing method */
            return TRUE;
        }
    }
    return FALSE;
}

short is_cmd_args_valid(short src_op_adrs_method, short target_op_adrs_method, int cmd_code, int *error_code) {
    switch (cmd_code) {
        case CMP:
            if (src_op_adrs_method == UNUSED_OPERARND || target_op_adrs_method == UNUSED_OPERARND) {
                *error_code = INVALID_AMOUNT_OF_OPERANDS;
                return FALSE;
            }
            return TRUE;
        case MOV:
        case ADD:
        case SUB:
            if (src_op_adrs_method == UNUSED_OPERARND || target_op_adrs_method == UNUSED_OPERARND) {
                *error_code = INVALID_AMOUNT_OF_OPERANDS;
                return FALSE;
            }
            if (target_op_adrs_method == IMMEDIATE_ADDRESSING) {
                *error_code = INVALID_TARGET_ADDRESS_METHOD;
                return FALSE;
            }
            return TRUE;
        case LEA:
            if (src_op_adrs_method == UNUSED_OPERARND || target_op_adrs_method == UNUSED_OPERARND) {
                *error_code = INVALID_AMOUNT_OF_OPERANDS;
                return FALSE;
            }
            if (target_op_adrs_method == IMMEDIATE_ADDRESSING) {
                *error_code = INVALID_TARGET_ADDRESS_METHOD;
                return FALSE;
            }
            if (src_op_adrs_method != DIRECT_MEMORY_ADDRESSING) {
                *error_code = INVALID_SOURCE_ADDRESS_METHOD;
                return FALSE;
            }
            return TRUE;
        case CLR:
        case NOT:
        case INC:
        case DEC:
        case RED:
            if (src_op_adrs_method == UNUSED_OPERARND) {
                *error_code = INVALID_AMOUNT_OF_OPERANDS;
                return FALSE;
            }
            if (src_op_adrs_method == IMMEDIATE_ADDRESSING) {
                *error_code = INVALID_TARGET_ADDRESS_METHOD;
                return FALSE;
            }
            if (target_op_adrs_method != UNUSED_OPERARND) {
                *error_code = INVALID_AMOUNT_OF_OPERANDS;
                return FALSE;
            }
            return TRUE;
        case JMP:
        case BNE:
        case JSR:
            if (src_op_adrs_method == UNUSED_OPERARND) {
                *error_code = INVALID_AMOUNT_OF_OPERANDS;
                return FALSE;
            }
            if (target_op_adrs_method != UNUSED_OPERARND) {
                *error_code = INVALID_AMOUNT_OF_OPERANDS;
                return FALSE;
            }
            if (src_op_adrs_method == IMMEDIATE_ADDRESSING || src_op_adrs_method == DIRECT_REGISTER_ADDRESSING) {
                *error_code = INVALID_TARGET_ADDRESS_METHOD;
                return FALSE;
            }
            return TRUE;
        case PRN:
            if (src_op_adrs_method == UNUSED_OPERARND) {
                *error_code = INVALID_AMOUNT_OF_OPERANDS;
                return FALSE;
            }
            if (target_op_adrs_method != UNUSED_OPERARND) {
                *error_code = INVALID_AMOUNT_OF_OPERANDS;
                return FALSE;
            }
            return TRUE;
        case RTS:
        case STOP:
            if (src_op_adrs_method != UNUSED_OPERARND || target_op_adrs_method != UNUSED_OPERARND) {
                *error_code = INVALID_AMOUNT_OF_OPERANDS;
                return FALSE;
            }
            return TRUE;
        default:
            /* Couldn't identify the command */
            *error_code = NIL;
            return FALSE;
    }
}

short get_instruction(char *string) {
    remove_spaces(string);
    if (strcmp(string, DOT_STRING) == EQUAL) {
        /* string == .string */
        return IS_DOT_STRING;
    }
    if (strcmp(string, DOT_EXTERN) == EQUAL) {
        /* string == .extern */
        return IS_DOT_EXTERN;
    }
    if (strcmp(string, DOT_DATA) == EQUAL) {
        /* string == .data */
        return IS_DOT_DATA;
    }
    if (strcmp(string, DOT_ENTRY) == EQUAL) {
        /* string == .entry */
        return IS_DOT_ENTRY;
    }
    return UNDERFINED_TYPE;
}


short get_word_type(char *string) {
    int i;
    /* Is empty line */
    if (string == NULL || is_empty(string)) {
        return IS_EMPTY_LINE;
    }

    /* Start of comment line */
    if (string[0] == COMMENT_LINE_START) {
        return IS_COMMENT_LINE;
    }

    /* Instruction word */
    for (i = 0; i < INSTRUCTIONS_LIST_LENGTH; i++) {
        if (strcmp(instructions[i], string) == EQUAL) {
            return IS_INSTRUCTION;
        }
    }

    /* OP Command word  */
    for (i = 0; i < COMMANDS_LIST_LENGTH; i++) {
        if (strcmp(commands[i], string) == EQUAL) {
            return IS_COMMAND;
        }
    }

    /* End of tokened string is ':' so it's a label */
    if (string[strlen(string) - 1] == CHAR_COLON) {
        return IS_LABEL;
    }

    /* Underfind word type */
    return UNDERFINED_TYPE;
}

short get_adrs_method(char *token, int *error_code) {
    int i, val;
    char *endptr; /* for validating integer whole numbers */
    char *string; /* copy of given token */
    char *string_temp; /* used to free allocated string copy */

    if (token == NULL) {
        return UNUSED_OPERARND;
    }
    /* Make copy of given token */
    string = malloc(strlen(token) + 1);
    if (string == NULL) {
        exit(EXIT_FAILURE);
    }
    string_temp = string; /* Used to free original string */
    strncpy(string, token, strlen(token));

    /* Null terminate the string and "clean" it */
    string[strlen(string)] = NULL_TERMINATOR;
    remove_spaces(string);

    /* Immediate addressing */
    if (string[0] == CHAR_HASHTAG) {
        /* Test if value is valid */
        string = strtok(string,DELIM_HASHTAG);
        val = strtol(string, &endptr, DECIMNAL_BASE);
        /* If the next if statment is true than test failed -> exit */
        if ((*endptr != NULL_TERMINATOR && *endptr != CHAR_NEWLINE) || endptr == string) {
            free(string_temp);
            *error_code = IMMEDIATE_OP_NOT_INT;
            return ERROR;
        }
        /* Out of range for 12 bit memory */
        if (val > MAX_VALUE_12_BIT || val < MIN_VALUE_12_BIT) {
            free(string_temp);
            *error_code = IMMEDIATE_OP_INT_OUT_RANGE;
            return ERROR;
        }
        /* Test passed */
        free(string_temp);
        return IMMEDIATE_ADDRESSING;
    }

    /* Indirect register addressing, e.g *r2 */
    if (string[0] == CHAR_ASTERISK) {
        /* Get register name */
        string = strtok(string,DELIM_ASTERISK);
        for (i = 0; i < REGISTERS_LIST_LENGTH; i++) {
            if (strncmp(registers[i], string, 2) == EQUAL) {
                free(string_temp);
                return INDIRECT_REGISTER_ADDRESSING;
            }
        }
        /* No match for above test -> invalid register name */
        *error_code = INVALID_REG_NAME;
        free(string_temp);
        return ERROR;
    }

    /* Direct register addressing */
    for (i = 0; i < REGISTERS_LIST_LENGTH; i++) {
        if (strncmp(registers[i], string, 2) == EQUAL) {
            free(string_temp);
            return DIRECT_REGISTER_ADDRESSING;
        }
    }

    /* Not the above -> direct memory addressing (label) */
    free(string_temp);
    return DIRECT_MEMORY_ADDRESSING;
}

short is_label_name_valid(char *string, MacroList *macro_list, LabelTable *label_table,
                          int *error_code) {
    int i, macro_flag = IS_NOT_MACRO_NAME;
    MacroNode *temp_macro;

    /* Label name is empty */
    if (string == NULL) {
        *error_code = LABEL_NAME_INVALID;
        return FALSE;
    }

    /* First char is not a letter -> invalid format */
    if (isdigit(string[0]) == TRUE) {
        *error_code = LABEL_NAME_START_ERROR;
        return FALSE;
    }

    /* Name of label is too long -> invalid format */
    if (strlen(string) > MAX_LABEL_LENGTH) {
        *error_code = LABEL_NAME_TOO_LONG;
        return FALSE;
    }

    /* Test if is OP command, if true -> invalid label name */
    for (i = 0; i < COMMANDS_LIST_LENGTH; i++) {
        if (strcmp(commands[i], string) == EQUAL) {
            *error_code = LABEL_NAME_IS_CMD_NAME;
            return FALSE;
        }
    }

    /* Test if is instruction, if true -> invalid label name */
    for (i = 0; i < INSTRUCTIONS_LIST_LENGTH; i++) {
        if (strcmp(instructions[i], string) == EQUAL) {
            *error_code = LABEL_NAME_IS_INSTRUCTION_NAME;
            return FALSE;
        }
    }

    /* Test if it's a regiser name, if true -> invalid label name */
    for (i = 0; i < REGISTERS_LIST_LENGTH; i++) {
        if (strcmp(registers[i], string) == EQUAL) {
            *error_code = LABEL_NAME_IS_REG_NAME;
            return FALSE;
        }
    }

    /* Test for existing macro name */
    temp_macro = is_line_name_of_macro(string, &macro_flag, macro_list);
    if (temp_macro != NULL) {
        *error_code = LABEL_NAME_IS_MACRO_NAME;
        return FALSE;
    }

    /* Test for already existing label with the same name */
    if (is_label_name_exists(string, label_table)) {
        *error_code = LABEL_NAME_EXISTS;
        return FALSE;
    }

    /* Passed all test's -> valid label name */
    return TRUE;
}

short *get_data(char *string_of_vals, int *error_code, int *parsed_data_arr_size) {
    int i = 0, arr_size = START_SIZE, converted_val;
    short *parsed_data_arr;
    char *token, *endptr;

    /* Allocate memory for data array */
    parsed_data_arr = malloc(arr_size * sizeof(short));
    if (parsed_data_arr == NULL) {
        exit(EXIT_FAILURE);
    }

    /* Get first value and "clean" the string*/
    token = tokenize(string_of_vals,DELIM_COMMA);
    if (token != NULL) {
        remove_spaces(token);
    }

    while (token != NULL) {
        if (is_empty(token)) {
            /* If token == "", then string was: "X,,X"*/
            *error_code = ILLEGAL_COMMA;
            free(parsed_data_arr);
            return NULL;
        }
        /* Get intger value */
        converted_val = strtol(token, &endptr, DECIMNAL_BASE);
        /* Error converting the integer */
        if ((*endptr != NULL_TERMINATOR && *endptr != CHAR_NEWLINE) || endptr == token) {
            *error_code = INVALID_INT;
            free(parsed_data_arr);
            return NULL;
        }
        /* Max value for 15 bit storage */
        if (converted_val > MAX_VALUE_15_BIT || converted_val < MIN_VALUE_15_BIT) {
            *error_code = INVALID_INT_RANGE;
            free(parsed_data_arr);
            return NULL;
        }
        /* Increase array memory for more values */
        if (i >= arr_size) {
            arr_size += START_SIZE;
            parsed_data_arr = realloc(parsed_data_arr, arr_size * sizeof(short));
            if (parsed_data_arr == NULL) {
                /* Allocation failed */
                exit(EXIT_FAILURE);
            }
        }
        /* Insert converted val to the array and advance index */
        parsed_data_arr[i] = converted_val;
        i++;
        /* Get next integer */
        token = tokenize(NULL,DELIM_COMMA);
        if (token != NULL) {
            remove_spaces(token);
        }
    }
    *parsed_data_arr_size = i;
    return parsed_data_arr;
}
