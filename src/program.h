//
// Created by Vulcan on 10/31/23.
//

#ifndef SJSU_CS_149_PROGRAM_H
#define SJSU_CS_149_PROGRAM_H

typedef struct {
    char **lines;
    int count;
} program_t;

program_t *program_get(char *file_name);
program_t *program_copy(program_t *program);
void program_free(program_t *program);

#endif //SJSU_CS_149_PROGRAM_H
