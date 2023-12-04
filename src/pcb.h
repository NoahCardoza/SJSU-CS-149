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

/**
 * Initializes a pcb_t struct.
 * @param parent_process_id The parent process id.
 * @param program The program to run.
 * @param state The state of the process.
 * @param program_counter The program counter.
 * @param time The time.
 * @return The initialized PCB.
 */
pcb_t *pcb_create(int parent_process_id, int priority, program_t *program, int state, int program_counter, int time);

/**
 * Frees a pcb_t struct.
 * @param pcb The PCB to free.
 */
void pcb_free(pcb_t *pcb);

/**
 * Prints a PCB.
 * @param pcb The PCB to print.
 * @param include_priority Whether to include the priority column when printing.
 */
void pcb_print(pcb_t *pcb, int include_priority);

/**
 * Prints the header for a PCB.
 * @param include_priority Whether to include the priority column when printing.
 */
void pcb_print_header(int include_priority);

#endif //SJSU_CS_149_PCB_H
