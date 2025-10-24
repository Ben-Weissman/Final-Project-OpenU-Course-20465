#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
#include <string.h>

#include "../headers/constants.h"
#include "../headers/utilities.h"
#include "../headers/parser.h"

int is_empty(char *string) {
    if (string == NULL || (strncmp(string, "", 1) == EQUAL)) {
        return TRUE;
    }
    while (*string != NULL_TERMINATOR) {
        if (!isspace(*string))
            return FALSE;
        string++;
    }
    return TRUE;
}

void remove_spaces(char *string) {
    char *start = string;
    char *end;

    if (start == NULL) {
        return;
    }
    /* Skip leading whitespace */
    while (isspace((unsigned char) *start)) {
        start++;
    }

    /* String is only whitespace */
    if (*start == NULL_TERMINATOR) {
        string = NULL;
        return;
    }

    /* Skip trailing whitespace */
    end = start;
    while (*end != NULL_TERMINATOR) {
        end++;
    }
    end--; /* Point to last char */

    /* Find last non whitespace char */
    while (end >= start && isspace((unsigned char) *end)) {
        end--;
    }

    /* Null-terminate the string */
    *(end + 1) = NULL_TERMINATOR;

    /* Shift string to the beginning of the buffer */
    while (*start != NULL_TERMINATOR) {
        *(string++) = *(start++);
    }
    *string = NULL_TERMINATOR; /* Null-terminate the string */
}

char *duplicate_string(char *string) {
    size_t len;
    char *dup;

    if (string == NULL) {
        return NULL;
    }

    len = strlen(string);
    dup = malloc(len + 1);
    if (dup == NULL) {
        return NULL;
    }

    memcpy(dup, string, len + 1);
    return dup;
}


int get_next_word_type(char *string) {
    remove_spaces(string);
    return get_word_type(string);
}

FILE *open_file(char *file_name, char *open_mode) {
    FILE *fp;

    if (file_name == NULL || open_mode == NULL) {
        return NULL;
    }

    fp = fopen(file_name, open_mode);
    if (fp == NULL) {
        perror("Error");
        return NULL;
    }
    return fp;
}


char *tokenize(char *string, char *delim) {
    static char *start = NULL; /* stores string str for next calls */
    char *token = NULL; /* token to be returned */

    if (string) {
        /* assign start to input string */
        start = string;
    }
    if (start == NULL) {
        /*string is empty*/
        return NULL;
    }
    /* remember current start as found token */
    token = start;
    /* find next occurrence of delim */
    start = strpbrk(start, delim);

    if (start) {
        /*Null terminate end of string */
        *start++ = NULL_TERMINATOR;
    }
    return token;
}

void remove_ending_colon(char *string) {
    short len;
    /* String empty */
    if (string == NULL || strlen(string) == 0) {
        return;
    }

    len = strlen(string);

    /* Last char is colon */
    if (string[len - 1] == CHAR_COLON) {
        string[len - 1] = NULL_TERMINATOR; /* replace with null terminator */
    }
}


void end_cycle(FILE *fp, Array *IC, Array *DC, LabelTable *label, LabelTable *entries, ExtList *ext, char *line) {
    if (fp != NULL) {
        fclose(fp);
    }
    free_array(IC);
    free_array(DC);
    free_label_table(label);
    free_label_table(entries);
    free_ext_list(ext);
    free(line);
}
