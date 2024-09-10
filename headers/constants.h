/**
 * constants header
 */
#ifndef CONSTANTS_H
#define CONSTANTS_H

/* OP commands / types of operations */
typedef enum {
    MOV = 0, CMP, ADD, SUB, LEA, CLR, NOT, INC, DEC, JMP, BNE, RED, PRN, JSR, RTS,
    STOP, UNDERFINED_TYPE
} commands_codes;

/* Instruction handles */
typedef enum {
    IS_INSTRUCTION = 1, IS_COMMAND, IS_LABEL, IS_EMPTY_LINE, IS_COMMENT_LINE, IS_DOT_STRING, IS_DOT_DATA, IS_DOT_ENTRY,
    IS_DOT_EXTERN
} action_type;

/* Memory location / addressing methods */
typedef enum {
    IMMEDIATE_ADDRESSING = 1, DIRECT_MEMORY_ADDRESSING = 2, INDIRECT_REGISTER_ADDRESSING = 4,
    DIRECT_REGISTER_ADDRESSING = 8, DATA, CODE, ENTRY, EXTERNAL, UNUSED_OPERARND
} memory_type;

/* Used for macro preoccesing */
typedef enum {
    IS_NOT_MACRO_NAME = 1, IS_MACRO_NAME, IS_NOT_MACRO_START, IS_MACRO_START, IS_MACRO_NAME_WITH_EXTRA_TEXT
} macro;


/* Strings */
#define AS_FILE_ENDING ".as"
#define AM_FILE_ENDING ".am"
#define ENTRY_FILE_ENDING ".ent"
#define EXTERNAL_FILE_ENDING ".ext"
#define OBJECT_FILE_ENDING ".ob"
#define DOT_STRING ".string"
#define DOT_DATA ".data"
#define DOT_EXTERN ".extern"

/* File handling */
#define APPEND_MODE "a"
#define READ_MODE "r"


/* General Codes */
#define ERROR 0
#define EQUAL 0 /* mainly used for strcmp for easy reading */
#define TRUE 1
#define FALSE 0
#define OFF -1
#define ON 1
#define NIL 0


/* Characters */
#define CHAR_COLON ':'
#define CHAR_NEWLINE '\n'
#define CHAR_ASTERISK '*'
#define CHAR_HASHTAG '#'
#define CHAR_QUOTATION '"'
#define COMMENT_LINE_START ';'
#define NULL_TERMINATOR '\0'


/* Delimeters */
#define DELIM_WHITESPACE " "
#define DELIM_COLON ":"
#define DELIM_NEWLINE "\n"
#define DELIM_WHITESPACES " \n\t"
#define DELIM_COMMA ","
#define DELIM_HASHTAG "#"
#define DELIM_ASTERISK "*"
#define DELIM_R "r"
#define DELIM_ASTERISK_R "*r"
#define DELIM_QUOTATION "\""
#define DELIM_DOT "."


/* Used for integer testing */
#define MAX_VALUE_15_BIT 16383
#define MIN_VALUE_15_BIT -16384
#define MAX_VALUE_12_BIT 2047
#define MIN_VALUE_12_BIT -2048


/* Lenghts */
#define MAX_LENGTH_LINE 80 /* Max line length */
#define INSTRUCTIONS_LIST_LENGTH 4
#define COMMANDS_LIST_LENGTH 16
#define REGISTERS_LIST_LENGTH 8
#define MAX_LABEL_LENGTH 31
#define START_SIZE 50 /* Min starting memory size */


/* Misc */
#define SRC_OP 1         /* for machine code */
#define TARGET_OP 2      /* for machine code */
#define DECIMNAL_BASE 10 /* used for strol function */
#define MAX_APPEARANCES 100 /* Used for ext labels */

#endif /* CONSTANTS_H */
