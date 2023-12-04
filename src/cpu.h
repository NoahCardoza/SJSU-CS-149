//
// Created by Vulcan on 10/31/23.
//

#ifndef SJSU_CS_149_CPU_H
#define SJSU_CS_149_CPU_H

#include <stddef.h>
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
    /**
     * The interrupt id.
     */
    int interrupt_id;
    /**
     * The interrupt argument.
     */
    size_t interrupt_argument;
} cpu_t;


/**
 * Context switch PCB to CPU.
 * @param cpu The CPU to context switch to.
 * @param pcb The PCB to context switch from.
 */
void context_switch_pcb_to_cpu(cpu_t *cpu, pcb_t *pcb);

/**
 * Context switch CPU to PCB.
 * @param cpu The CPU to context switch from.
 * @param pcb The PCB to context switch to.
 */
void context_switch_cpu_to_pcb(cpu_t *cpu, pcb_t *pcb);

/**
 * Helper function to configure interrupt flags on the CPU.
 * @param cpu The CPU to configure.
 * @param interrupt_id The interrupt id.
 * @param interrupt_argument The optional interrupt argument.
 */
void cpu_set_interrupt(cpu_t *cpu, int interrupt_id, size_t interrupt_argument);

/**
 * Sets the state of the single CPU register to value.
 * @param cpu The CPU to update.
 * @param value The value to set.
 */
void cpu_state_set(cpu_t *cpu, int value);

/**
 * Adds value to the state of the single CPU register.
 * @param cpu The CPU to update.
 * @param value The value to add.
 */
void cpu_state_add(cpu_t *cpu, int value);

/**
 * Subtracts value from the state of the single CPU register.
 * @param cpu The CPU to update.
 * @param value The value to subtract.
 */
void cpu_state_sub(cpu_t *cpu, int value);

#endif //SJSU_CS_149_CPU_H
