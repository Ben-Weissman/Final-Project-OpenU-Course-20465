/**
 * preprocessor header
 */
#ifndef PREPROCESSOR_H
#define PREPROCESSOR_H
#include "data_structures.h"


void pre_proccesor(char *input_file_name, MacroList *macro_list, int *error_code);

/** @brief Checks if the line is a start of macro defnition
 *
 * This function test's if the given input string is a start of macro definition.
 * The given macro flag is updated accordingly.
 * Also if it is a macro definition then the given name_of_macro will hold the macro's name.
 *
 * @param name_of_macro If it is macro defnition then the name will be stored here.
 * @param input_string The line of text to check
 * @param macro_flag The result of the function will be stored here.
 */
void is_line_start_of_macro_definition(char *name_of_macro, char *input_string, int *macro_flag);

/** @brief Checks if the line is a name of macro
 *
 * This fucntion checks to see if the given string is a name of a macro that is stored in the list of macro's
 * @param input_string line of text to check if is a macro name
 * @param macro_flag currernt macro status to update
 * @param list_of_macro the list of macro's
 * @return the macro node from the list that matches the text, else will return NULL.
 */
MacroNode *is_line_name_of_macro(char *input_string, int *macro_flag, MacroList *list_of_macro);


/** @brief Checks if the name is valid name
 *
 * This functions checks to see if the input string is a command/instruction
 * If it's valid (not a OP command / instruction) retuns TRUE, else it will update the
 * macro error code argument and return FALSE
 * @param name Name of the macro
 * @param macro_error Error code containing error status
 * @return True (1) if name is not command/instruction, False (0) otherwise.
 */
int is_macro_name_valid(char *name, int *macro_error);


#endif /* PREPROCESSOR_H */
