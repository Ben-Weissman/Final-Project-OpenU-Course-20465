/**
 * utilities  header
 */

#ifndef UTILITIES_H
#define UTILITIES_H

#include <stdio.h>

#include "data_structures.h"
/** @brief Checks if string is empty (only whitespaces)
 *
 * This function checks if the string contains only whitesapces (i.e tabs, \n , space etc..).
 * @param string string to check if is empty
 * @return TRUE(1) if empty, FALSE(0) if not empty.
 */
int is_empty(char *string);

/** @brief removes spaces from a given string
 *
 * This function removes spaces " " from a string.
 * @param string to remove spaces from
 */
void remove_spaces(char *string);

/**
 * @brief Opens a file
 * This fucntion opens a file , if the file can't be opened it prints error messege.
 * @param file_name the name of the file
 * @param open_mode the mode in which the file will be used (read, write etc..)
 * @return FILE pointer if opened correctly, else returns NULL.
 */
FILE *open_file(char *file_name, char *open_mode);


/** @bried Used to tokenize strings by a delimeter.
 * This is a function that behaves the same like strtok only this one allows for "" (empty) strings.
 * @param string String to tokenize from
 * @param delim Delimeter
 * @return Pointer to a string that has been tokened by the delim
 */
char *tokenize(char *string, char *delim);


/** @brief Duplicates a given string
 * This function duplicates a given string with malloc to save it for later use
 * @param string String to duplicate
 * @return A pointer to a new string with the same content as the given string
 */
char *duplicate_string(char *string);


/** @brief
 * This function cleans the given string and returns the word type.
 * @param string The string to analyze and get the word type from.
 * @return The value of the word type (i.e for OP command = X, for .string = Y , etc.. ).
 */
int get_next_word_type(char *string);


/** @brief Removes the ending colon of a given string
 * @param string String to remove the ending colon of.
 */
void remove_ending_colon(char *string);


/** @brief This function free's any allocated memory in preperation for the next cycle.
 * @param fp File pointer to free
 * @param IC IC special array to free
 * @param DC DC special array to free
 * @param label Label table to free
 * @param entries Entries table to free
 * @param ext External label list to free
 * @param line String to free
 */
void end_cycle(FILE *fp, Array *IC, Array *DC, LabelTable *label, LabelTable *entries, ExtList *ext, char *line);
#endif /* UTILITIES_H */
