/**
* parser header
 */

#ifndef PARSER_H
#define PARSER_H
#include "data_structures.h"


/** @brief Returns the command to perform
 * This function anlyzes the given string and returns the correct command value
 * Command value is determined by the given rules of the assignment.
 * @param string String that contains the command
 * @return value of the command to perform
 */
int get_cmd(char *string);


/** @brief Tests if both methods are registers addressing methods
 * This function tests if the given operands addressing methods are both
 * some kind of registers addressing methods.
 * @param src_op_adrs_method Source operand addressing method
 * @param target_op_adrs_method Target operand addressing method
 * @return True (1) if both are reg mem methods and false (0) otherwise.
 */
short both_operands_are_registers(short src_op_adrs_method, short target_op_adrs_method);


/** @brief Tests if syntax of the command is valid
 * This function tests if the given operands addressing methods for the given command is valid.
 * The correct args for each command is defined in the project guidelines
 * @param src_op_adrs_method Source operand addressing method
 * @param target_op_adrs_method Target operand addressing method
 * @param cmd_code Command value
 * @param error_code Error code for any error's
 * @return TRUE(1) if valid, FALSE(0) if not valid.
 */
short is_cmd_args_valid(short src_op_adrs_method, short target_op_adrs_method, int cmd_code, int *error_code);


/** @brief Returns the data instruction to perform
 * This function anlyzes the given string and returns the correct data instruction value
 * @param string String that contains the instruction
 * @return data.as instruction to perfom
 */
short get_instruction(char *string);


/** @brief Returns the curent word type
 * This function anlyzes the given string and returns the correct type of word,
 * options are: label, command, data.as instruction, empty line, comment line etc..
 * @param string String with the "action"
 * @return Type of "action"
 */
short get_word_type(char *string);


/** @brief Retuns the addressing method of a given string
 * This function anlyzes the given string and returns the correct type of adressing method
 * @param token String containing  an operand
 * @param error_code Code for error checking
 * @return Type of addressing method.
 */
short get_adrs_method(char *token, int *error_code);


/** @brief Test's if the label name is valid
 * This function test's to see if the given label name is valid according to the assignment rules.
 * @param string Containing the name of the label
 * @param list_of_macro This file macro list
 * @param label_table Label table
 * @param error_code Error code for error checking
 * @return TRUE(1) if valid label name, FALSE(0) otherwise.
 */
short is_label_name_valid(char *string, MacroList *list_of_macro, LabelTable *label_table, int *error_code);


/** @brief Returns an array with parsed data.as
 * This function parses the given string and returns an array with the given integer values.
 * @param string_of_vals String containing the values
 * @param error_code Error code for error checking
 * @param parsed_data_arr_size Size/length of the parsed data.as array
 * @return Integer array containing the parsed data.as.
 */
short *get_data(char *string_of_vals, int *error_code, int *parsed_data_arr_size);

#endif /* PARSER_H */
