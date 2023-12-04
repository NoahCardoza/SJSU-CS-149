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

/**
 * Used to minimize the number of arguments passed to functions.
 */
typedef struct  {
    cpu_t cpu;
    scheduler_t scheduler;
    int time;
    int total_turnaround;
    int processes_ended;
    struct pbc_queue_node *current_process;
} manager_t;

/**
 * Used to create the IVT.
 */
typedef void manager_handel_interrupt_f(manager_t *);

/**
 * Initializes a manager_t struct.
 * @param manager
 */
void manager_init(manager_t *manager);

/**
 * Runs the manager.
 * @param fd The file descriptor to read commands from.
 */
void manger_run(int fd);

/**
 * Handles the process time slice (Q) command.
 * @param manager The state of the manager.
 */
void manger_handel_command_process_time_slice(manager_t *manager);

/**
 * Handles the unblock process (U) command.
 * @param manager The state of the manager.
 */
void manager_handel_command_unblock_process(scheduler_t *scheduler);

/**
 * Handles the print state (P) command.
 * @param manager The state of the manager.
 */
void manager_handel_command_print_system_state(manager_t *manager);

/**
 * Handles the terminate (T) command.
 * @param manager The state of the manager.
 */
int manager_handel_command_terminate(manager_t *manager);

/**
 * Calculates the turn around time of the current process on the CPU.
 * @param manager The state of the manager.
 * @return The turn around time of the current process.
 */
int manager_calculate_turn_around_time(manager_t *manager);

/**
 * Handles the interrupts. Checks if an interrupt has occurred and
 * calls the appropriate interrupt handler using the IVT.
 * @param manager The state of the manager.
 */
void manager_handel_interrupt(manager_t *manager);

/**
 * Handles the load interrupt.
 * @param manager The state of the manager.
 */
void manager_handel_interrupt_load(manager_t *manager);

/**
 * Handles the fork interrupt.
 * @param manager The state of the manager.
 */
void manager_handel_interrupt_fork(manager_t *manager);

/**
 * Handles the block interrupt.
 * @param manager The state of the manager.
 */
void manager_handel_interrupt_block(manager_t *manager);

/**
 * Handles the terminate interrupt.
 * @param manager The state of the manager.
 */
void manager_handel_interrupt_terminate(manager_t *manager);

#endif //SJSU_CS_149_MANAGER_H
