//
// Created by Vulcan on 10/31/23.
//

#ifndef SJSU_CS_149_PCB_H
#define SJSU_CS_149_PCB_H

#include <sys/queue.h>
#include <stdlib.h>
#include <stdio.h>
#include "program.h"

/**
 * This data structure represents a simulated process control block.
 */
typedef struct {
    int process_id;
    int parent_process_id;
    program_t *program;
    int program_counter;
    int priority;
    int state;
    int start_time;
    int cpu_time_used;
} pcb_t;

pcb_t *pcb_create(int parent_process_id, program_t *program, int state, int program_counter, int time);

void pcb_free(pcb_t *pcb);

void pcb_print(pcb_t *pcb, int include_priority);

void pcb_print_header(int include_priority);

#endif //SJSU_CS_149_PCB_H
