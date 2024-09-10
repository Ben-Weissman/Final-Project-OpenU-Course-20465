#ifndef SECOND_PASS_H
#define SECOND_PASS_H

#include "data_structures.h"

/** @brief Builds the .ent file
 * This function loops through the entries table and 'prints' out the values of the table to the new file
 * @param entries_table Entry table to build file from
 * @param name_of_file postfix name of the file.
 */
void build_entries_file(LabelTable *entries_table, char *name_of_file);


/** @brief Second pass
 * This function is the second pass of the assembler, it scans through the IC array and looks for any holes with associated label names,
 * if such holes & names exist in the same location then it's a label that needs to be updated with the correct new address we got during the
 * first pass.
 * Also this function calls to building the output files if needed.
 * @param IC_array IC array
 * @param DC_array DC array
 * @param label_table Label table of the file
 * @param ent_table Entries table of the file
 * @param ext_list External list of the file
 * @param ext_flag To note if any EXTERNAL label exists
 * @param ent_flag To note if any ENTRY label exists
 * @param file_name The file name
 * @param error_code For any error finding
 */
void second_pass(Array *IC_array, Array *DC_array, LabelTable *label_table, LabelTable *ent_table, ExtList *ext_list,
                 short ext_flag, short ent_flag, char *file_name, int *error_code);


/** @brief Builds the .object file
 *  * This function loops through the recieved memory array (after it has been merged from IC and DC array)
 *  It then prints out all the memory values (machine code) to the file.
 * @param name_of_file Name of file
 * @param arr Merged Memory of the system array
 * @param IC Length of IC array
 * @param DC Length of DC array
 * @param len Length of entire memory array
 */
void build_object_file(char *name_of_file, short *arr, short IC, short DC, short len);


/**@brief Builds the .ext file
 * This function loops through the extrnal label list and 'prints' out the values of the list to the new file
 * @param ext_list External label list
 * @param name_of_file Name of file to print to
 */
void build_ext_file(ExtList *ext_list, char *name_of_file);
#endif /* SECOND_PASS_H */
