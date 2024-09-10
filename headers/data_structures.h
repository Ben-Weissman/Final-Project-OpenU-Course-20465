/**
* data_structures header
 */

#ifndef DATA_STRUCTURES_H
#define DATA_STRUCTURES_H

#include <stdio.h>

/* External label's list & head, defined in assroted C file
 * Each external label node contains a name and an integer array with the address's
 * where the label is mentioned in the file
 */
typedef struct ExtLabelNode ExtLabelNode;
typedef struct ExtList ExtList;

/* Macro's list & node are the nodes used for pre-processor stage.
 * The macro node contains name & defenition (all the lines that define the macro,
 * one after the other).
 */
typedef struct MacroNode MacroNode;
typedef struct MacroList MacroList;

/* Label's list & head, defined in assroted C file,
 * contains label name, value and location (CODE, DATA, EXT, ENTRY..)
 * Also contains the pointer to the next node in the list.
 */
typedef struct LabelNode LabelNode;
typedef struct LabelTable LabelTable;

/* This is the 'special' array that mimic's the computer memory.
 * The size is the max size of the array.
 * The index is increased dynamicaly and saved each time to point to the last position.
 * The *data is the "regular" array that will hold the values.
 * The **label_name_arr is the same as the data array only in each index it will be NULL if the current
 * index is not a label and a string if the current index is label position.
 */
typedef struct Array Array;


/* !!!!!!!!!!!!! */
/* ~~~~~~~~~~~~~ */
/* MACRO SECTION */

/** @brief Returns the given macro list head
 * @param list The macro list we want to return the head of.
 * @return Head of the given macro list
 */
MacroNode *get_list_head(MacroList *list);


/** @brief Returns the next node
 * This function returns the next node in the given macro list
 * @param node The node to get the successive of
 * @return The next node after given node
 */
MacroNode *get_next_macro_node(MacroNode *node);


/** @brief Retuns the given macro name
 * @param node the macro node which we want to get the name of
 * @return The name of the given macro node
 */
char *get_macro_name(MacroNode *node);


/** @brief Returns the given macro definition
 * @param node the macro node which we want to get the definition of
 * @return The definition of the given macro node
 */
char *get_macro_definition(MacroNode *node);

/** @brief Creates an empty macro list
 * @return A Macro list pointer to the new empty macro list
 */
MacroList *new_macro_list();

/** @brief Adds a given macro node to the macro list
 * This function adds a given macro node to the given macro list
 * @param list The list we want to add the macro node to.
 * @param node The macro node we want to add to the list.
 */
void add_to_macro_list(MacroList *list, MacroNode *node);

/** @brief Creates a new macro node
 * This functions creates a new macro node whilst testing for errors.
 * It then returns a pointer to the newly created macro node
 * @param new_name The name of the new macro.
 * @param new_defenition The defnition string of the whole macro, one line after another untill endmacr
 * @return A macro node pointer to the new macro node.
 */
MacroNode *create_new_macro(char *new_name, char *new_defenition);


/* !!!!!!!!!!!!! */
/* ~~~~~~~~~~~~~ */
/* LABEL SECTION */

/** @brief Creates a new label table ("tavlat smalim")
 * This function creates an empty label's list.
 * @return a pointer to an empty label's list
 */
LabelTable *new_label_table();

/** @brief Add's a new label to the label's table
 * This function adds a label node to an exisiting label table
 * @param table table of labels
 * @param label_name name of the new label
 * @param location the kind of saving (relocatble, external, code , data).
 * @param value value of the label
 * @param error_code Code for any error whilst adding
 */
void add_to_label_table(LabelTable *table, char *label_name, int location, int value, int *error_code);

/** @brief Creates a new label node
 * This recieves a pointer to a allocated label node and insert's it's information (name, value and location),
 * to the given node.
 * This function does not create a node, it "updates" a pre declared label node.
 * @param new_node The node to update the details of
 * @param new_name The name of the new label
 * @param new_val Value of the label
 * @param new_location The kind of memory location (DATA, CODE, EXTERNAL etc..)
 * @return a label node with the updates values.
 */
LabelNode *new_label_node(LabelNode *new_node,  char *new_name, int new_val, int new_location);

/** @brief Updates the label table after first pass
 * This function updates the label table after the first pass by adding +100 to any DATA/CODE types.
 * @param table The label table to update
 * @param IC The current IC index.
 */
void update_label_table(LabelTable *table, int IC);

/** @brief Retuns the address of the given label name.
 * This function searches the label table to find the given label name and returns it's address.
 * @param table The label table to search for given label name
 * @param label_name The label to find in the table.
 * @return If found, the address of the label, else will return NIL.
 */
int get_label_address(LabelTable *table, char *label_name);

/** @brief Retuns the location of the given label name.
 * This function searches the label table to find the given label name and returns it's location.
 * @param table The label table to search for given label name
 * @param label_name The label to find in the table.
 * @return If found, the location of the label memory, else will return NIL.
 */
int get_label_location(LabelTable *table, char *label_name);

/** @brief Retuns the name of the given label node
 * @param node The label node to ge the name off
 * @return The name of the given label node, NULL otherwise.
 */
char *get_label_name(LabelNode *node);

/** @brief Retuns the address value of the given label node
 * @param node The label node to ge the address off
 * @return The name of the given label node, NULL otherwise.
 */
int get_label_address_from_node(LabelNode *node);

/** @brief Gets the first node in the list.
 * This function returns the first node in the given label list.
 * @param table The table to return the head off
 * @return The head of the given label list.
 */
LabelNode *get_first_node(LabelTable *table);


/** @brief Returns the next node after the given node.
 * This function returns the next node after the given node in the list.
 * @param node The node to get the next node in the list.
 * @return The next node from the given node.
 */
LabelNode *get_next_node(LabelNode *node);


/** @brief Returns the label node of a given lable name
 * This function searches the label table for a given label name, if found it will return the said node,
 * otherwise it will return NULL
 * @param label_name The label name to test if exists
 * @param label_table The label list to search in.
 * @return If found the node with the given name, NULL otherwise.
 */
LabelNode *is_label_name_exists(char *label_name, LabelTable *label_table);

/* !!!!!!!!!!!!! */
/* ~~~~~~~~~~~~~ */
/* ARRAY SECTION */

/** @brief Creates a new Array 'object' in the size of the given value
 * Note: I have assumed a fix size, this can be fixed (explained in the top section of this file).
 * @param size The number of elements that the array will hold.
 * @return An Array pointer to the newly created object.
 */
Array *new_array(unsigned short size);


/** @brief Returns the current index of the array.
 * @param arr The special array to return the index of
 * @return The current special array Index
 */
int get_index_special_array(Array *arr);


/** @brief Inserts a new machine code to DC array
 * This function inserts a new input given by the arguments to the data counter array (DC).
 * @param DC_arr The special DATA array to insert the new input to.
 * @param value The machine code value to insert
 */
void insert_to_DC_array(Array *DC_arr, short value);


/** @brief Inserts a new machine code to IC array
 *  * This function inserts a new input given by the arguments to the instruction counter array (IC).
 *  If a lable name is given that it will be inserted to the label's name array assoicated with this IC array.
 * @param IC_arr The special instruction counter array to insert the new value to.
 * @param value The machien code value to insert.
 * @param label_name The asscoiated label name.
 */
void insert_to_IC_array(Array *IC_arr, short value, char *label_name);


/** @brief Increase's the index of the special Array.
 * This function increases the index by 1.
 * @param IC_arr The special array to increase the index by 1.
 */
void increase_IC_index(Array *IC_arr);


/** @brief Returns the value stored in IC Array @ index.
 * This function returns the value @ data[index]
 * @param IC_arr The instruction array
 * @param index The index we want to get the value of @ data.
 * @return The value stored at IC_arr.data[index]
 */
int get_IC_val(Array *IC_arr, int index);

/** @brief Returns the assoicated label to the memory @ index
 * This function returns the label name stored in the label array asscoiated with the IC array
 * The label name is returned from the array at position index.
 * @param IC_arr The instruction array
 * @param index The index we want to get the label name off
 * @return The label name assoicated with the index position at IC.
 */
char *get_IC_label( Array *IC_arr, int index);


/** @brief Updates the values of IC array @ position index.
 * The function updates the values of the memory position index.
 * @param IC_arr The IC array we want to update the info on.
 * @param index The position we want to update.
 * @param new_val The new value to update.
 */
void update_IC_value_at_index(Array *IC_arr, int index, int new_val);

/** @brief Returns the value stored at DC array @ position index.
 * The function returns the value stored in the data array @ position index.
 * @param DC_arr The special array we want to get the value off
 * @param index The position of the value we want.
 * @return The value stored at DC_arr.data[index]
 */
int get_DC_val(Array *DC_arr, int index);

/** @brief Merges the data values of instruction array and data array.
 * This funtion merges the data arrays of the given 'special' arrays and creates a new single array
 * with the instruction part first and the data part last.
 * @param IC_arr IC array
 * @param DC_arr DC array
 * @return Merged array containing machine code of the file.
 */
short *merge_array(Array *IC_arr, Array *DC_arr);

/* !!!!!!!!!!!!!!! */
/* ~~~~~~~~~~~~~~~ */
/* ENTRIES SECTION */


/** @brief Updates the external label values
 * This function scans the entry label list and for each one updates it's exaact value after the first pass
 * @param label_table Label table containing the newly updatd label values
 * @param entries_table Entries table
 * @param error_code Error status.
 */
void update_entries_table(LabelTable *label_table, LabelTable *entries_table, int *error_code);

/* !!!!!!!!!!!!!!!!! */
/* ~~~~~~~~~~~~~~~~~ */
/* EXTERNALS SECTION */

/** @brief Creates a new empty list for external labels
 * @return An external list pointer to an empty list
 */
ExtList *new_ext_list();

/** @brief Creates a new external label node
 * This function creates a new external label node cotaining the given name
 * @param new_name The name of the new external label
 * @return An external node pointer to the newly created node
 */
ExtLabelNode *new_ext_label_node(char *new_name);


/** @brief Adds a new external label to the list
 * This function creates a new external label node with the given name and adds it
 * to the given external label list
 * @param list The external label list to add the new label to.
 * @param name_of_node The new external label name.
 */
void add_to_ext_list(ExtList *list, char *name_of_node);


/** @brief Updates the addresses which the label appears on in the file.
 * This function scans the external label list to locate the given lable name, if it found it updates
 * the correspanding label appearances array with the new given address
 * @param list Exetrnal label list
 * @param node_name The label to update the new appearance address
 * @param address The new address value where the label appears
 */
void add_ext_appearence(ExtList *list, char *node_name, short address);


/** @brief Returns the head of the external label list
 * @param ext_list The externl label list to get the head off.
 * @return External Label Node pointer to the head of the external label list.
 */
ExtLabelNode *get_ext_list_head(ExtList *ext_list);


/** @brief Returns the name of the given external label node
 * @param node External label node we want to know the name off
 * @return The name string of the given node.
 */
char *get_ext_name(ExtLabelNode *node);

/** @brief "Prints" to file each external label appearances mentions
 * This function prints the given external label address appearances to the given file.
 * @param fp The file to print out the appearances
 * @param node The node to print out the appearances off
 */
void print_to_file_ext_appearences(FILE *fp, ExtLabelNode *node);

/** @brief Returns the next node in the list
 * The function returns the next node in the last after the given input node
 * @param node The node to get the next node in the list from
 * @return The next node in the list
 */
ExtLabelNode *get_next_ext_node(ExtLabelNode *node);

#endif /* DATA_STRUCTURES_H */
