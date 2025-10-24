#include <stdlib.h>
#include <string.h>

#include "../headers/second_pass.h"
#include "../headers/constants.h"
#include "../headers/data_structures.h"
#include "../headers/error_handling.h"
#include "../headers/machine_code.h"
#include "../headers/utilities.h"

void second_pass(Array *IC_array, Array *DC_array, LabelTable *label_table, LabelTable *ent_table, ExtList *ext_list,
                 short ext_flag, short ent_flag, char *file_name, int *error_code) {
    char *label_name = NULL;
    short *merged_array, merged_arr_len;
    int IC_end = get_index_special_array(IC_array); /* Max len of IC array */
    int i, IC_arr_val = NIL, label_address, label_location;

    *error_code = OFF;

    /* look for NIL address in the IC array, if found update that location with correct label address */
    for (i = 0; i < IC_end; i++) {
        IC_arr_val = get_IC_val(IC_array, i);
        label_name = get_IC_label(IC_array, i);
        if ((IC_arr_val == NIL) && (is_empty(label_name) == FALSE)) {
            label_address = get_label_address(label_table, label_name);
            label_location = get_label_location(label_table, label_name);

            if (label_location == EXTERNAL) {
                /* Found EXTERNAL so add it to the list of apppeances */
                add_ext_appearence(ext_list, label_name, i + 100);
            }
            /* Insert machine code of the correct address */
            machine_code_label(IC_array, i, label_address, label_location);
        }
    }
    /* found an error untill now */
    if (*error_code != OFF) {
        handle_error(error_code, NIL);
        return;
    }

    /* There was .entry lines in the file -> create .ent file */
    if (ent_flag != OFF) {
        update_entries_table(label_table, ent_table, error_code);
        if (*error_code == OFF) {
            build_entries_file(ent_table, file_name);
        }
    }

    if (*error_code != OFF) {
        handle_error(error_code, NIL);
        return;
    }

    /* There was .extern lines in the file -> create .ext file */
    if (ext_flag != OFF) {
        build_ext_file(ext_list, file_name);
    }

    /* Create the memory of the file*/
    merged_array = merge_array(IC_array, DC_array);
    merged_arr_len = get_index_special_array(IC_array) + get_index_special_array(DC_array);

    /* Build the .obj file */
    if (merged_array != NULL) {
        build_object_file(file_name, merged_array, get_index_special_array(IC_array), get_index_special_array(DC_array),
                          merged_arr_len);
        free(merged_array);
    }
}


void build_entries_file(LabelTable *entries_table, char *name_of_file) {
    char *label_name, *file;
    int label_address;
    FILE *fp;
    LabelNode *temp;
    /* Create name of .ent file */
    file = malloc(strlen(name_of_file) + strlen(ENTRY_FILE_ENDING) + 1);
    if (file == NULL) {
        return;
    }
    strcpy(file, name_of_file);
    strcat(file, ENTRY_FILE_ENDING);

    /* Open file */
    fp = open_file(file, APPEND_MODE);
    if (fp == NULL) {
        free(file);
        return;
    }

    /* Loop though the entries list and "print" each data to the file */
    temp = get_first_node(entries_table);
    while (temp != NULL) {
        label_name = get_label_name(temp);
        label_address = get_label_address_from_node(temp);
        fprintf(fp, "%s %04d\n", label_name, label_address);
        temp = get_next_node(temp);
    }
    fclose(fp);
    free(file);
}

void build_ext_file(ExtList *ext_list, char *name_of_file) {
    char *file = malloc(strlen(name_of_file) + strlen(EXTERNAL_FILE_ENDING) + 1);
    FILE *fp;
    ExtLabelNode *ext_node;
    if (file == NULL) {
        return;
    }
    strcpy(file, name_of_file);
    strcat(file, EXTERNAL_FILE_ENDING);
    fp = open_file(file, APPEND_MODE);
    if (fp == NULL) {
        free(file);
        return;
    }
    ext_node = get_ext_list_head(ext_list);
    while (ext_node != NULL) {
        print_to_file_ext_appearences(fp, ext_node);
        ext_node = get_next_ext_node(ext_node);
    }
    fclose(fp);
    free(file);
}

void build_object_file(char *name_of_file, short *arr, short IC, short DC, short len) {
    int i;
    unsigned short val, mask, result;
    char *file = malloc(strlen(name_of_file) + strlen(OBJECT_FILE_ENDING) + 1);
    FILE *fp;
    mask = 0x7FFF; /* Used for octal representation */
    /* create object file name */
    if (file == NULL) {
        return;
    }
    strcpy(file, name_of_file);
    strcat(file, OBJECT_FILE_ENDING);
    fp = open_file(file, APPEND_MODE);
    if (fp == NULL) {
        free(file);
        return;
    }

    /* Header of file */
    fprintf(fp, "  %d %d\n", IC, DC);

    /* Print values */
    for (i = 0; i < len; i++) {
        val = arr[i];
        result = val & mask;
        fprintf(fp, "%04d %05o\n", (i + 100), result);
    }
    fclose(fp);
    free(file);
}
