/**
* first_pass header
 */

#ifndef FIRST_PASS_H
#define FIRST_PASS_H
#include "data_structures.h"

/** @brief First pass of assembler
 * This is the first pass of the assembler.
 * The function first understands what the syntax/format of the line is (i.e comment line, instruction line, command line etc..)
 * and then sends the appropriate data to the case functions to handle further processing.
 * @param name_of_file The name of the current file to read
 * @param macro_list The list of macros from the preprocessor phase.
 */
void first_pass(char *name_of_file, MacroList *macro_list);


/** @brief Case handling for .extern line
 * This function handles the .extern type of lines.
 * It recieves the line and inserts the appropriate values to the label  & extern table.
 * @param string The line which was read
 * @param label_table Label table to add the extern label to.
 * @param ext_list Extern label table to add the extern label to.
 * @param error_code For error status
 */
void handle_dot_extern_case(char *string,LabelTable *label_table, ExtList *ext_list, int *error_code);

/** @brief Case handling for .data line
 * This function recieves the read line from the file, extracts the data from the line (via external function),
 * and inserts each of the numbers to the DC array.
 * @param string The line from the source file
 * @param DC_arr The DATA array that holds all the data values
 * @param error_code For error status
 */
void handle_dot_data_case(char *string, Array *DC_arr, int *error_code);

/** @brief Case handling for .string line
 * This function recieves the read line from the file, extracts the string from the line,
 * and inserts each of the charachters to the DC array.
 * @param string The line from the source file
 * @param DC_arr The DATA array that holds all the charachters values
 * @param error_code For error status
 */
void handle_dot_string_case(char *string, Array *DC_arr, int *error_code);

/** @brief Case handling for a OP command line.
* This function recieves the read line from the file, parses (via external function) the operands and their addressing methods,
 * It then translates the line to machine code and insers each machine code to the IC array.
 * @param token The line from the source file
 * @param IC_arr The IC array that holds all the machine code
 * @param error_code For error status
 */
void handle_command_case(char *token, Array *IC_arr, int *error_code);

/** @brief Case handling for .entry line
 * This function recieves the line from the file, extracts the label name after the .entry
 * and stores it in the label table.
 * @param string The line from the source file
 * @param table Label table to store the .entry label
 * @param value The label's value (current IC index)
 * @param error_code For error status
 */
void handle_dot_entry_case(char *string, LabelTable *table, short value, int *error_code);


#endif /* FIRST_PASS_H */
