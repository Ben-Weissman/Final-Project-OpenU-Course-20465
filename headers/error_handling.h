/**
* error_handling header
 */

#ifndef ERROR_HANDLING_H
#define ERROR_HANDLING_H

enum ErrorValues {
    /* Used for commands and associated operands addressing methods */
    INVALID_AMOUNT_OF_OPERANDS = 1,
    INVALID_TARGET_ADDRESS_METHOD,
    INVALID_SOURCE_ADDRESS_METHOD,

    /* Used for syntax test's */
    ILLEGAL_COMMA,
    INVALID_INT,
    INVALID_INT_RANGE,
    INVALID_ACTION,


    /* Used for operands error's */
    IMMEDIATE_OP_NOT_INT,
    IMMEDIATE_OP_INT_OUT_RANGE,
    INVALID_REG_NAME, /* used in line 207 @ get_adrs_method */
    INVALID_STRING,

    /* Used for label name test's */
    LABEL_NAME_INVALID,
    LABEL_NAME_START_ERROR,
    LABEL_NAME_TOO_LONG,
    LABEL_NAME_IS_CMD_NAME,
    LABEL_NAME_IS_INSTRUCTION_NAME,
    LABEL_NAME_IS_REG_NAME,
    LABEL_NAME_IS_MACRO_NAME,
    LABEL_NAME_EXISTS,

    NOT_ALL_ENTRIES_FOUND,

    /* Used for pre-processor (macro's) */
    MACRO_NAME_IS_CMD_NAME,
    MACRO_NAME_IS_INSTRUCTION_NAME,
    EXTRA_TEXT_AT_START_OF_MACRO
};

typedef struct ErrorTable ErrorTable;

void handle_error(int *error_code, int line_number);

#endif /* ERROR_HANDLING_H */
