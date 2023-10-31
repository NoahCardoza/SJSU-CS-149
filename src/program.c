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

    while (!feof(file)) {
        getline(&line, &bufflen, file);
        program->lines[program->count] = malloc(strlen(line) + 1);
        memccpy(program->lines[program->count], line, 0, strlen(line));
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

void program_free(program_t *program) {
    for (int i = 0; i < program->count; i++) {
        free(program->lines[i]);
    }
    free(program->lines);
    free(program);
}