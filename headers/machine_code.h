/**
* machine_code header
 */

#ifndef MACHINE_CODE_H
#define MACHINE_CODE_H


#include "data_structures.h"

/** @brief Parses, translates and inserts the correct machine code of a given data
 * This function translates the given data to a formatted 15 binary machine code.
 * The code is then inserted to IC array.
 * @param opcode The command to perform
 * @param src_adrs_method Source operand addressing method
 * @param target_adrs_method Target operand addressing method
 * @param IC_arr The IC array (memory of computer) to insert the machine code to.
 */
void machine_code(short opcode, short src_adrs_method, short target_adrs_method, Array *IC_arr);

/** @brief Parses, translates and inserts the correct "extra" machine code of a given data
 * If needed another word of information by the system then this function creates that 'extra' machine code
 * by a set of data given to the function, it is then inserted to IC array.
 * @param operand The current operand 'string'
 * @param adrs_method The current operand address method
 * @param IC_arr The IC array (memory of computer) to insert the machine code to.
 * @param choosen_op Choosen operand (source/target).
 */
void machine_extra_code(char *operand, short adrs_method, Array *IC_arr, short choosen_op);


/** @brief Produces a common machine code for a 2 register command line.
 * If the system requires a machine code for a line that contains 2 operands that are registers addressing methods operands,
 * then this function creates that commono word for both of them and insers it to the IC array.
 * @param src_op Name of source operand
 * @param target_op Name of target operand
 * @param IC_arr The IC array (memory of computer) to insert the machine code to.
 */
void machine_code_double_registers(char *src_op, char *target_op, Array *IC_arr);


/** @brief Creates machine code for a label
 * After the second pass when all the labels addresse's are known, this function creates the correct machine code for that label and insert's it
 * to the IC array.
 * @param IC_arr The IC array (memory of computer) to insert the machine code to.
 * @param index The location in the IC array of where to put the machine code
 * @param label_address The label address value
 * @param label_location The label kind of location saving (DATA, EXTERNAL , etc..)
 */
void machine_code_label(Array *IC_arr, short index,  short label_address,  short label_location);
#endif /* MACHINE_CODE_H */
