#include "../headers/error_handling.h"
#include "../headers/constants.h"

#include <stdio.h>

struct ErrorTable {
    short error_code;
    char *error_msg;
};

ErrorTable error_table[] = {
    {0, ""}, /* used to start from 1 */
    {INVALID_AMOUNT_OF_OPERANDS, "Amount of operands does not correspand to the given command format"},
    {INVALID_TARGET_ADDRESS_METHOD, "Target operand memory addressing method for the given command is incorrect"},
    {INVALID_SOURCE_ADDRESS_METHOD, "Source operand memory addressing method for the given command is incorrect"},

    {ILLEGAL_COMMA, "Illegal comma in the data.as part"},
    {INVALID_INT, "One or more of the given data.as is not a whole number"},
    {INVALID_INT_RANGE, "One or more of the given numbers can not be stored in 15 bit memory (out of range)"},
    {INVALID_ACTION, "Unknown action to be performed"},

    {IMMEDIATE_OP_NOT_INT, "Immediate operand is not a whole integer (must be a whole number in range)"},
    {
        IMMEDIATE_OP_INT_OUT_RANGE,
        "Given immediate operand can not be stored in 12 bit memory (out of range for immediate addressing method numbers)"
    },
    {INVALID_REG_NAME, "Unknown register name"},
    {INVALID_STRING, "Input string invalid, missing start/end quotation mark"},

    {LABEL_NAME_INVALID, "Label name is invalid,"},
    {LABEL_NAME_START_ERROR, "Label name must start with a letter,"},
    {LABEL_NAME_TOO_LONG, "Label name must be no longer than 31 charachters long"},
    {LABEL_NAME_IS_CMD_NAME, "Label name is a OP command (cmp, mov, lea etc..)"},
    {LABEL_NAME_IS_INSTRUCTION_NAME, "Label name is a instruction command (.string, .data.as etc..)"},
    {LABEL_NAME_IS_REG_NAME, "Label name is a register name (r1, r2, r3 etc..)"},
    {LABEL_NAME_IS_MACRO_NAME, "Label name is an already existing macro name"},
    {LABEL_NAME_EXISTS, "Label name already exists"},

    {NOT_ALL_ENTRIES_FOUND, "One or more of stated .entry labels were not defined in this file"},
    {NOT_ALL_ENTRIES_FOUND, "One or more of stated .entry labels were not defined in this file"},


};


void handle_error(int *error_code, int line_number) {
    if (line_number != NIL) {
        /* After preprocessor phase */
        printf("Error : %s, please check line number %d in the .am file\n", error_table[(*error_code)].error_msg,
               line_number);
    } else {
        /* Pre-processor phase so no line numbers */
        printf("Error : %s, cannot continue with the program.\n", error_table[(*error_code)].error_msg);
    }
}
