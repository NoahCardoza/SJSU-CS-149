//
// Created by Vulcan on 11/1/23.
//

#ifndef SJSU_CS_149_SCHEDULER_H
#define SJSU_CS_149_SCHEDULER_H


#include "config.h"
#include "queues.h"

/**
 * This data structure represents a simulated scheduler.
 * It contains a queue for each priority level and a blocked queue.
 */
typedef struct {
    struct pbc_queue_head pcb_table;
    struct pbc_nested_queue_head priority_queue_heads[PRIO_LEVELS];
    struct pbc_nested_queue_head blocked_queue_head;
} scheduler_t;

/**
 * Initializes the scheduler queues.
 * @param scheduler The new scheduler to preform the operation on.
 */
void scheduler_init(scheduler_t *scheduler);

/**
 * Initializes a process and enqueues it in the scheduler.
 * @param scheduler The scheduler to preform the operation on.
 * @param parent_pid The parent process id.
 * @param program The program to run.
 * @param state The state of the process.
 * @param program_counter The program counter.
 * @param time The time.
 */
void scheduler_process_init(scheduler_t *scheduler, int parent_pid, program_t *program, int state, int program_counter, int time);

/**
 * Frees a process from the scheduler queues.
 * @param scheduler The scheduler to preform the operation on.
 * @param scheduled_pcb The process to free.
 */
void scheduler_process_free(scheduler_t *scheduler, struct pbc_queue_node *scheduled_pcb);

/**
 * Enqueues a process into the scheduler queues.
 * @param scheduler The scheduler to preform the operation on.
 * @param pcb_el The process to enqueue.
 */
void scheduler_enqueue_process(scheduler_t *scheduler, struct pbc_queue_node * pcb_el);

/**
 * Dequeues a process from the scheduler queues. It first checks
 * the highest priority queue and then goes down the priority
 * levels.
 * @param scheduler The scheduler to preform the operation on.
 * @return The process that was dequeued.
 */
struct pbc_queue_node * scheduler_dequeue_process(scheduler_t *scheduler);

/**
 * Puts a process into the blocked queue.
 * @param scheduler The scheduler to preform the operation on.
 * @param pcb_el The process to block.
 */
void scheduler_block_process(scheduler_t *scheduler, struct pbc_queue_node *pcb_el);

/**
 * Removes a process from the blocked queue.
 * @param scheduler The scheduler to preform the operation on.
 * @return The process that was unblocked.
 */
struct pbc_queue_node * scheduler_unblock_process(scheduler_t *scheduler);

#endif //SJSU_CS_149_SCHEDULER_H
