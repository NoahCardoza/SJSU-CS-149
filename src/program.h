//
// Created by Vulcan on 10/31/23.
//

#ifndef SJSU_CS_149_PROGRAM_H
#define SJSU_CS_149_PROGRAM_H

/**
 * Represents a simulated program.
 */
typedef struct {
    char **lines;
    int count;
} program_t;

/**
 * Returns a pointer to the beginning of string argument.
 * @param line The line to read from.
 * @return The string parameter.
 */
char* program_read_str_param_from_line(char *line);

/**
 * Returns the value of integer argument.
 * @param line The line to read from.
 * @return The integer parameter value.
 */
int program_read_int_param_from_line(char *line);

/**
 * Gets a program from a file.
 * @param file_name The file to read the program from.
 * @return The program.
 */
program_t *program_get(char *file_name);

/**
 * Copies a program.
 * @param program The program to copy.
 * @return The copied program.
 */
program_t *program_copy(program_t *program);


/**
 * Frees a program.
 * @param program The program to free.
 */
void program_free(program_t *program);

#endif //SJSU_CS_149_PROGRAM_H
