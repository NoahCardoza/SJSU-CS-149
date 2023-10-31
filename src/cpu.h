//
// Created by Vulcan on 10/31/23.
//

#ifndef SJSU_CS_149_CPU_H
#define SJSU_CS_149_CPU_H
#include "cpu.h"
#include "pcb.h"

/**
 * This data structure represents a simulated CPU.
 */
typedef struct {
    /**
     * The program that is being simulated. For the purposes of this
     * assignment, a program is a null-terminated array of strings.
     */
    program_t *program;
    /**
     * The current program counter position.
     */
    int program_counter;
    /**
     * The state of the program. For the purposes of this assignment,
     * the state of the program is described by a single integer value.
     */
    int state;
    /**
     * The time slice of the simulated process.
     */
    int time_slice;
    /**
     * The number of time slices that the process has used.
     */
    int used_time_slices;
} cpu_t;

void context_switch_pcb_to_cpu(cpu_t *cpu, pcb_t *pcb);

void context_switch_cpu_to_pcb(cpu_t *cpu, pcb_t *pcb);


#endif //SJSU_CS_149_CPU_H
