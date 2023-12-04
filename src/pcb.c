//
// Created by Vulcan on 10/31/23.
//

#include "program.h"
#include "manager.h"
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include <unistd.h>
#include <sys/types.h>
#include "pcb.h"

pcb_t *pcb_create(int parent_process_id, program_t *program, int state, int program_counter, int time) {
    static int process_id_counter = 0;

    pcb_t *pcb = malloc(sizeof(pcb_t));
    if (pcb == NULL) {
        perror("malloc");
        exit(1);
    }

    pcb->process_id = process_id_counter++;
    pcb->parent_process_id = parent_process_id;
    pcb->program = program;
    pcb->program_counter = program_counter;
    pcb->priority = 0;
    pcb->state = state;
    pcb->start_time = time; // TODO: accept as argument
    pcb->cpu_time_used = 0;

    return pcb;
}

void pcb_free(pcb_t *pcb) {
    program_free(pcb->program);
    free(pcb);
}