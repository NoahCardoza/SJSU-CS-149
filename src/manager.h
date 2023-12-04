//
// Created by Vulcan on 10/30/23.
//

#ifndef SJSU_CS_149_MANAGER_H
#define SJSU_CS_149_MANAGER_H
#include "program.h"
#include "scheduler.h"
#include "cpu.h"

#define INTERRUPT_NONE 0
#define INTERRUPT_TERMINATE 1
#define INTERRUPT_BLOCK 2
#define INTERRUPT_FORK 3
#define INTERRUPT_LOAD 4

/**
 * Used to set the current value of the program state.
 */
#define INSTRUCTION_SET 'S'

/**
 * Used to add a value to the current value of the program state.
 */
#define INSTRUCTION_ADD 'A'

/**
 * Used to subtract a value from the current value of the program state.
 */
#define INSTRUCTION_SUBTRACT 'D'

/**
 * Used to tell the operating system to put the current process on the blocked queue.
 */
#define INSTRUCTION_BLOCK 'B'

/**
 * Used to tell the operating system to terminate the current process.
 */
#define INSTRUCTION_TERMINATE 'E'

/**
 * Used to tell the operating system to fork the current process.
 */
#define INSTRUCTION_FORK 'F'

/**
 * Used to tell the operating system to load a program into memory.
 */
#define INSTRUCTION_LOAD 'R'

typedef struct  {
    cpu_t cpu;
    scheduler_t scheduler;
    int time;
    int total_turnaround;
    int processes_ended;
    struct pbc_queue_item *current_process;
} manager_t;

typedef void manager_handel_interrupt_f(manager_t *);

void manger_run(int stdin_fd);

void manager_handel_interrupt(manager_t *manager);

void manager_unblock_process(scheduler_t *scheduler);

void manager_init(manager_t *manager);

void manager_print_system_state(manager_t *manager);

int manager_calculate_turn_around_time(manager_t *manager);

int manager_terminate(manager_t *manager);

void manager_handel_interrupt_load(manager_t *manager);

void manager_handel_interrupt_fork(manager_t *manager);

void manager_handel_interrupt_block(manager_t *manager);

void manager_handel_interrupt_terminate(manager_t *manager);
#endif //SJSU_CS_149_MANAGER_H
