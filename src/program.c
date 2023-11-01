//
// Created by Vulcan on 10/31/23.
//

#include "manager.h"
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include <unistd.h>
#include <sys/types.h>
#include "program.h"

program_t *program_get(char *file_name) {
    FILE *file = fopen(file_name, "r");
    if (file == NULL) {
        perror("fopen");
    }

    size_t bufflen = 256;
    char *line = NULL;

    program_t *program = malloc(sizeof(program_t));
    if (program == NULL) {
        perror("malloc");
        exit(1);
    }

    char** tmp;
    program->lines = malloc(sizeof(char *) * 32);
    program->count = 0;
    size_t len;

    while (!feof(file)) {
        getline(&line, &bufflen, file);

        // skip empty lines and comments
        if (line[0] == '\n' || line[0] == '#') {
            continue;
        }

        len = strlen(line);

        // remove newline if it ends with one
        if (line[len - 1] == '\n') {
            line[--len] = '\0';
        }

        program->lines[program->count] = malloc(len + 1);
        memccpy(program->lines[program->count], line, 0, len);
        program->count++;
        if (program->count % 32 == 0) {
            program->lines = realloc(program->lines, sizeof(char *) * (program->count + 32));
            if (program->lines == NULL) {
                perror("realloc");
                exit(1);
            }
        }

    }

    program->lines = realloc(program->lines, sizeof(char *) * program->count);
    if (program->lines == NULL) {
        perror("realloc");
        exit(1);
    }
    fclose(file);
    return program;
}

program_t *program_copy(program_t *program) {
    program_t *copy = malloc(sizeof(program_t));
    copy->count = program->count;
    copy->lines = (char**)malloc(sizeof(char *) * copy->count);
    for (int i = 0; i < copy->count; i++) {
        copy->lines[i] = malloc(strlen(program->lines[i]) + 1);
        strcpy(copy->lines[i], program->lines[i]);
    }
    return copy;
}

void program_free(program_t *program) {
    for (int i = 0; i < program->count; i++) {
        free(program->lines[i]);
    }
    free(program->lines);
    free(program);
}