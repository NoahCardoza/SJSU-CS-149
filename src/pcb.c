//
// Created by Vulcan on 10/31/23.
//

#include "program.h"
#include "manager.h"
#include <stdlib.h>
#include <stdio.h>
#include "pcb.h"

pcb_t *pcb_create(int parent_process_id, int priority, program_t *program, int state, int program_counter, int time) {
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
    pcb->priority = priority;
    pcb->state = state;
    pcb->start_time = time;
    pcb->cpu_time_used = 0;

    return pcb;
}

void pcb_free(pcb_t *pcb) {
    program_free(pcb->program);
    free(pcb);
}

void pcb_print_header(int include_priority) {
    printf("+-----------------------------------------------------------------+\n");
    if (include_priority) {
        printf("|  PID | PPID | Priority |    State | Start Time | CPU Time Used  |\n");
    } else {
        printf("|  PID |            PPID |    State | Start Time | CPU Time Used  |\n");
    }
    printf("+-----------------------------------------------------------------+\n");
}

void pcb_print(pcb_t *pcb, int include_priority) {
    if (include_priority) {
        printf("|  %3d | %4d | %8d | %8d | %10d | %13d  |\n",
               pcb->process_id,
               pcb->parent_process_id,
               pcb->priority,
               pcb->state,
               pcb->start_time,
               pcb->cpu_time_used);
    } else {
        printf("|  %3d | %15d | %8d | %10d | %13d |\n",
               pcb->process_id,
               pcb->parent_process_id,
               pcb->state,
               pcb->start_time,
               pcb->cpu_time_used);
    }
}

