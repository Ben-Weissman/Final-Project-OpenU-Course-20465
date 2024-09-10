#include "../headers/data_structures.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "../headers/constants.h"
#include "../headers/error_handling.h"
#include "../headers/utilities.h"

struct ExtLabelNode {
    char *name;
    short *appearance_addresses;
    ExtLabelNode *next;
};

struct ExtList {
    ExtLabelNode *head;
};

struct MacroNode {
    char *macro_name;
    char *macro_def;
    MacroNode *next;
};

struct MacroList {
    MacroNode *head;
};


struct LabelNode {
    char *label_name;
    int value;
    int location;
    LabelNode *next;
};

struct LabelTable {
    struct LabelNode *head;
};


struct Array {
    unsigned short size;
    unsigned short i;
    short *data;
    char **label_name_arr;
};

/* !!!!!!!!!!!!! */
/* ~~~~~~~~~~~~~ */
/* MACRO SECTION */

MacroNode *get_list_head(MacroList *list) {
    return list->head;
}

MacroNode *get_next_macro_node(MacroNode *node) {
    return node->next;
}

char *get_macro_name(MacroNode *node) {
    return node->macro_name;
}

char *get_macro_definition(MacroNode *node) {
    return node->macro_def;
}

MacroList *new_macro_list() {
    MacroList *list = malloc(sizeof(MacroList));
    list->head = NULL;
    return list;
}

void add_to_macro_list(MacroList *list, MacroNode *node) {
    /* list is empty */
    if (list->head == NULL) {
        list->head = node;
    }
    /* list is not empty */
    else {
        node->next = list->head;
        list->head = node;
    }
}

MacroNode *create_new_macro(char *new_name, char *new_defenition) {
    /* create a new node */
    MacroNode *new_node = malloc(sizeof(MacroNode));

    if (new_node == NULL) {
        return NULL;
    }
    /* memory for the macro name & defnition */
    new_node->macro_name = malloc(strlen(new_name) + 1);
    new_node->macro_def = malloc(strlen(new_defenition) + 1);
    new_node->next = NULL;

    /* memory allocation failed */
    if (new_node->macro_name == NULL || new_node->macro_def == NULL) {
        free(new_node);
        return NULL;
    }

    /* copy the contents */
    memcpy(new_node->macro_def, new_defenition, strlen(new_defenition) + 1);
    memcpy(new_node->macro_name, new_name, strlen(new_name) + 1);
    return new_node;
}


/* !!!!!!!!!!!!! */
/* ~~~~~~~~~~~~~ */
/* LABEL SECTION */

LabelTable *new_label_table() {
    LabelTable *head = malloc(sizeof(LabelTable));
    return head;
}

void add_to_label_table(LabelTable *table, char *label_name, int location, int value, int *error_code) {
    LabelNode *node = malloc(sizeof(LabelNode));
    remove_ending_colon(label_name);
    if (is_label_name_exists(label_name, table) != NULL) {
        *error_code = LABEL_NAME_EXISTS;
        return;
    }
    node = new_label_node(node, label_name, value, location);
    /* list is empty */
    if (table->head == NULL) {
        table->head = node;
    }
    /* list is not empty */
    else {
        node->next = table->head;
        table->head = node;
    }
}

LabelNode *new_label_node(LabelNode *new_node, char *new_name, int new_val, int new_location) {
    if (new_node == NULL) {
        return NULL;
    }

    /* memory for the label name */
    new_node->label_name = malloc(strlen(new_name) + 1);

    /* assign label value */
    new_node->value = new_val;
    new_node->location = new_location;
    new_node->next = NULL;

    /* memory allocation failed */
    if (new_node->label_name == NULL) {
        free(new_node);
        return NULL;
    }

    /* copy the contents */
    memcpy(new_node->label_name, new_name, strlen(new_name) + 1);

    return new_node;
}

void update_label_table(LabelTable *table, int IC) {
    LabelNode *temp = table->head;
    while (temp != NULL) {
        if (temp->location == DATA) {
            temp->value += (100 + IC);
        } else if (temp->location == CODE) {
            temp->value += 100;
        }
        temp = temp->next;
    }
}

int get_label_address(LabelTable *table, char *label_name) {
    LabelNode *temp = is_label_name_exists(label_name, table);
    if (temp != NULL) {
        /* Label exist's in the list so return the address */
        return temp->value;
    }
    return NIL;
}

int get_label_location(LabelTable *table, char *label_name) {
    LabelNode *temp = is_label_name_exists(label_name, table);
    if (temp != NULL) {
        /* Label exist's in the list so return the location */
        return temp->location;
    }
    return NIL;
}

char *get_label_name(LabelNode *node) {
    return (node != NULL) ? node->label_name : NULL;
}

int get_label_address_from_node(LabelNode *node) {
    return (node != NULL) ? node->value : NIL;
}

LabelNode *get_first_node(LabelTable *table) {
    return (table != NULL) ? table->head : NULL;
}

LabelNode *get_next_node(LabelNode *node) {
    return node->next;
}

LabelNode *is_label_name_exists(char *label_name, LabelTable *label_table) {
    int result;
    char *label_name_copy;
    LabelNode *temp = label_table->head;
    if (label_name == NULL) {
        return NULL;
    }
    label_name_copy = malloc(strlen(label_name) + 1);

    if (label_name_copy == NULL) {
        exit(EXIT_FAILURE);
    }
    /*Make copy of input string and remove \n and whitespaces */
    strcpy(label_name_copy, label_name);
    remove_spaces(label_name_copy);
    label_name_copy[strcspn(label_name_copy, DELIM_WHITESPACE)] = 0;


    /* check if the given input line is a name of existing label */
    while (temp != NULL) {
        result = strcmp(label_name_copy, temp->label_name);
        /* found a label in the list that is same as the input name */
        if (result == EQUAL) {
            return temp;
        }
        /* advance in the list */
        temp = temp->next;
    }
    /* no match was found in the table */
    return NULL;
}

/* !!!!!!!!!!!!! */
/* ~~~~~~~~~~~~~ */
/* ARRAY SECTION */

Array *new_array(unsigned short size) {
    Array *array = malloc(sizeof(Array));
    array->size = size;
    array->data = malloc(size * sizeof(short));
    array->label_name_arr = (char **) malloc(size * sizeof(char *));
    memset(array->data, 0, size);
    array->i = 0;
    return array;
}

int get_index_special_array(Array *arr) {
    return arr->i;
}

void insert_to_DC_array(Array *DC_arr, short value) {
    int index;
    index = DC_arr->i;
    DC_arr->data[index] = value;
    index++;
    DC_arr->i = index;
}

void insert_to_IC_array(Array *IC_arr, short value, char *label_name) {
    int index;
    index = IC_arr->i;
    if (label_name != NULL) {
        IC_arr->label_name_arr[index] = duplicate_string(label_name);
    }
    IC_arr->data[index] = value;
    index++;
    IC_arr->i = index;
}

void increase_IC_index(Array *IC_arr) {
    IC_arr->i += 1;
}

int get_IC_val(Array *IC_arr, int index) {
    return IC_arr->data[index];
}

char *get_IC_label(Array *IC_arr, int index) {
    return IC_arr->label_name_arr[index];
}

void update_IC_value_at_index(Array *IC_arr, int index, int new_val) {
    IC_arr->data[index] = new_val;
}

int get_DC_val(Array *DC_arr, int index) {
    return DC_arr->data[index];
}


short *merge_array(Array *IC_arr, Array *DC_arr) {
    short IC = get_index_special_array(IC_arr);
    short DC = get_index_special_array(DC_arr);
    int i, j;
    short *merg_arr = malloc((IC + DC) * sizeof(short));
    for (i = 0; i < IC; i++) {
        merg_arr[i] = get_IC_val(IC_arr, i);
    }
    for (i = IC, j = 0; j < DC; i++, j++) {
        merg_arr[i] = get_DC_val(DC_arr, j);
    }
    return merg_arr;
}

/* !!!!!!!!!!!!!!! */
/* ~~~~~~~~~~~~~~~ */
/* ENTRIES SECTION */

void update_entries_table(LabelTable *label_table, LabelTable *entries_table, int *error_code) {
    LabelNode *entries_temp = entries_table->head;
    LabelNode *label_temp;
    while (entries_temp != NULL) {
        /* Search for current entry label in "regular" label table */
        label_temp = is_label_name_exists(entries_temp->label_name, label_table);
        if (label_temp == NULL) {
            *error_code = NOT_ALL_ENTRIES_FOUND;
            return;
        }
        /* Update entry value */
        entries_temp->value = label_temp->value;
        /* Advance */
        entries_temp = entries_temp->next;
    }
}

/* !!!!!!!!!!!!!!!!! */
/* ~~~~~~~~~~~~~~~~~ */
/* EXTERNALS SECTION */

ExtList *new_ext_list() {
    ExtList *list = malloc(sizeof(ExtList));
    list->head = NULL;
    return list;
}

ExtLabelNode *new_ext_label_node(char *new_name) {
    ExtLabelNode *new_node = malloc(sizeof(ExtLabelNode));

    /* Allocate Memory */
    new_node->name = malloc(strlen(new_name) + 1);
    new_node->appearance_addresses = malloc(MAX_APPEARANCES * sizeof(short));

    /* Zero out appearances & set next node  */
    memset(new_node->appearance_addresses, NIL, MAX_APPEARANCES);
    new_node->next = NULL;

    /* Assign name */
    memcpy(new_node->name, new_name, strlen(new_name) + 1);

    return new_node;
}

void add_to_ext_list(ExtList *list, char *name_of_node) {
    ExtLabelNode *node = new_ext_label_node(name_of_node);
    /* List is empty */
    if (list->head == NULL) {
        list->head = node;
    }
    /* List is not empty */
    else {
        node->next = list->head;
        list->head = node;
    }
}

void add_ext_appearence(ExtList *list, char *node_name, short address) {
    ExtLabelNode *temp = list->head;
    int i;
    remove_spaces(node_name);
    /* Find the correct node by name */
    while (temp != NULL) {
        if (strcmp(temp->name, node_name) == EQUAL) {
            break;
        }
        temp = temp->next;
    }
    if (temp == NULL) {
        /* No node was found with the given name */
        return;
    }
    for (i = 0; i <= MAX_APPEARANCES; i++) {
        if (temp->appearance_addresses[i] == NIL) {
            temp->appearance_addresses[i] = address;
            return;
        }
    }
}

ExtLabelNode *get_ext_list_head(ExtList *ext_list) {
    return (ext_list != NULL) ? ext_list->head : NULL;
}


char *get_ext_name(ExtLabelNode *node) {
    return (node != NULL) ? node->name : NULL;
}

void print_to_file_ext_appearences(FILE *fp, ExtLabelNode *node) {
    int i;
    for (i = 0; i < MAX_APPEARANCES; i++) {
        /* Get each address and add it to file */
        if (node->appearance_addresses[i] != NIL) {
            fprintf(fp, "%s %04d\n", node->name, node->appearance_addresses[i]);
        }
    }
}

ExtLabelNode *get_next_ext_node(ExtLabelNode *node) {
    return (node != NULL) ? node->next : NULL;
}
