#include "../headers/machine_code.h"

#include <stdlib.h>
#include <string.h>

#include "../headers/constants.h"
#include "../headers/utilities.h"

#define END_CODE_SECTION 4
#define START_OP_CODE_INDEX 11
#define START_TARGET_OPERAND_INDEX 3
#define START_SOURCE_OPERAND_INDEX 7

/* Bit setting */
#define A_FIELD_INDEX 2
#define R_FIELD_INDEX 1
#define E_FIELD_INDEX 0

#define SET_NTH_BIT(number, n) ((number) | (1 << (n)))
#define IS_NTH_BIT_SET(number, n) (((number) & (1 << (n))) != 0)

void machine_code_double_registers(char *src_op, char *target_op, Array *IC_arr) {
    short src_op_val, dest_op_val;
    short coded_word_i, current_var_i = 0, coded_word = NIL;

    /* Remove any space/asterik/'r' and get registers values */
    remove_spaces(src_op);
    remove_spaces(target_op);
    src_op_val = atoi(strtok(src_op, DELIM_ASTERISK_R));
    dest_op_val = atoi(strtok(target_op, DELIM_ASTERISK_R));


    /* Turn source operand bits */
    for (coded_word_i = 6; coded_word_i <= 8; coded_word_i++, current_var_i++) {
        if (IS_NTH_BIT_SET(src_op_val, current_var_i)) {
            coded_word = SET_NTH_BIT(coded_word, coded_word_i);
        }
    }

    /* Turn target operand bits */
    for (current_var_i = 0, coded_word_i = 3; coded_word_i <= 5; coded_word_i++, current_var_i++) {
        if (IS_NTH_BIT_SET(dest_op_val, current_var_i)) {
            coded_word = SET_NTH_BIT(coded_word, coded_word_i);
        }
    }

    /* Turn A field on */
    coded_word = SET_NTH_BIT(coded_word, A_FIELD_INDEX);

    /* Insert machine code to IC array */
    insert_to_IC_array(IC_arr, coded_word, NULL);
}


void machine_extra_code(char *operand, short adrs_method, Array *IC_arr, short choosen_op) {
    short coded_word_i, current_var_i = 0, coded_word = NIL;
    int op_val;
    switch (adrs_method) {
        case IMMEDIATE_ADDRESSING:

            /* Convert the immediate operand value to a number */
            remove_spaces(operand);
            op_val = atoi(strtok(operand,DELIM_HASHTAG));

        /* "Construct" the number in machine code */
            for (coded_word_i = 3; coded_word_i <= 14;
                 coded_word_i++, current_var_i++) {
                if (IS_NTH_BIT_SET(op_val, current_var_i)) {
                    coded_word = SET_NTH_BIT(coded_word, coded_word_i);
                }
            }

        /* Set the correspanding 'ARE' field */
            coded_word = SET_NTH_BIT(coded_word, A_FIELD_INDEX);

        /* Insert machine code to IC array */
            insert_to_IC_array(IC_arr, coded_word, NULL);
            break;

        case DIRECT_MEMORY_ADDRESSING:
            /* Empty space for second pass */
            insert_to_IC_array(IC_arr, NIL, operand);
            break;

        case DIRECT_REGISTER_ADDRESSING:
            /* It's a register like r3, so remove the 'r' & get the register's number */
            remove_spaces(operand);
            op_val = atoi(strtok(operand, DELIM_R));
        /* It's a source operand so activate bits 6 -> 8 accordingly */
            if (choosen_op == SRC_OP) {
                for (coded_word_i = 6; coded_word_i <= 8; coded_word_i++, current_var_i++) {
                    if (IS_NTH_BIT_SET(op_val, current_var_i)) {
                        coded_word = SET_NTH_BIT(coded_word, coded_word_i);
                    }
                }
            } else {
                /* It's a target operand so activate bits 3 -> 5 accordingly*/
                for (coded_word_i = 3; coded_word_i <= 6; coded_word_i++, current_var_i++) {
                    if (IS_NTH_BIT_SET(op_val, current_var_i)) {
                        coded_word = SET_NTH_BIT(coded_word, coded_word_i);
                    }
                }
            }
        /* set A field to on for first code word */
            coded_word = SET_NTH_BIT(coded_word, A_FIELD_INDEX);

        /* Insert machine code to IC array */
            insert_to_IC_array(IC_arr, coded_word, NULL);
            break;
        case INDIRECT_REGISTER_ADDRESSING:
            /* It's a register like *r3, so remove the '*r' & get the register's number */
            op_val = atoi(strtok(operand, DELIM_ASTERISK_R));
            if (choosen_op == SRC_OP) {
                /* It's a source operand so activate bits 6 -> 8 accordingly */
                for (coded_word_i = 6; coded_word_i <= 8; coded_word_i++, current_var_i++) {
                    if (IS_NTH_BIT_SET(op_val, current_var_i)) {
                        coded_word = SET_NTH_BIT(coded_word, coded_word_i);
                    }
                }
            } else {
                /* It's a target operand so activate bits 3 -> 5 accordingly*/
                for (coded_word_i = 3; coded_word_i <= 6; coded_word_i++, current_var_i++) {
                    if (IS_NTH_BIT_SET(op_val, current_var_i)) {
                        coded_word = SET_NTH_BIT(coded_word, coded_word_i);
                    }
                }
            }
        /* set A field to on for first code word */
            coded_word = SET_NTH_BIT(coded_word, A_FIELD_INDEX);
        /* Insert machine code to IC array */
            insert_to_IC_array(IC_arr, coded_word, NULL);
            break;
        default:
            /* Unused operand -> do nothing */
            break;
    }
}

void machine_code(short opcode, short src_adrs_method, short target_adrs_method, Array *IC_arr) {
    short coded_word = NIL;
    short current_var_i = 0, coded_word_i = START_OP_CODE_INDEX;

    /* Set OP code */
    while (current_var_i < END_CODE_SECTION) {
        if (IS_NTH_BIT_SET(opcode, current_var_i)) {
            coded_word = SET_NTH_BIT(coded_word, coded_word_i);
        }
        coded_word_i++;
        current_var_i++;
    }
    if (src_adrs_method != UNUSED_OPERARND) {
        if (target_adrs_method == UNUSED_OPERARND) {
            /* Only 1 operand -> set only target operand code */
            coded_word_i = START_TARGET_OPERAND_INDEX;
            current_var_i = 0;
            while (current_var_i < END_CODE_SECTION) {
                if (IS_NTH_BIT_SET(src_adrs_method, current_var_i)) {
                    coded_word = SET_NTH_BIT(coded_word, coded_word_i);
                }
                coded_word_i++;
                current_var_i++;
            }
        } else {
            /* Both operand are active */
            /* Set source operand code */
            coded_word_i = START_SOURCE_OPERAND_INDEX;
            current_var_i = 0;
            while (current_var_i < END_CODE_SECTION) {
                if (IS_NTH_BIT_SET(src_adrs_method, current_var_i)) {
                    coded_word = SET_NTH_BIT(coded_word, coded_word_i);
                }
                coded_word_i++;
                current_var_i++;
            }
            /* Set target operand code */
            coded_word_i = START_TARGET_OPERAND_INDEX;
            current_var_i = 0;
            while (current_var_i < END_CODE_SECTION) {
                if (IS_NTH_BIT_SET(target_adrs_method, current_var_i)) {
                    coded_word = SET_NTH_BIT(coded_word, coded_word_i);
                }
                coded_word_i++;
                current_var_i++;
            }
        }
        /* Set A field ON 1 operand */
        coded_word = SET_NTH_BIT(coded_word, A_FIELD_INDEX);
    }
    if (src_adrs_method == UNUSED_OPERARND && target_adrs_method == UNUSED_OPERARND) {
        /* stop/rts commands */
        coded_word = SET_NTH_BIT(coded_word, A_FIELD_INDEX);
    }

    /* Insert machine code to IC array */
    insert_to_IC_array(IC_arr, coded_word, NULL);
}


void machine_code_label(Array *IC_arr, short index, short label_address, short label_location) {
    short coded_word = 0;
    short current_var_i, coded_word_i;

    for (coded_word_i = 3, current_var_i = 0; coded_word_i <= 14; coded_word_i++, current_var_i++) {
        if (IS_NTH_BIT_SET(label_address, current_var_i)) {
            coded_word = SET_NTH_BIT(coded_word, coded_word_i);
        }
    }

    if (label_location == EXTERNAL) {
        coded_word = SET_NTH_BIT(coded_word, E_FIELD_INDEX);
    } else {
        coded_word = SET_NTH_BIT(coded_word, R_FIELD_INDEX);
    }

    update_IC_value_at_index(IC_arr, index, coded_word);
}
