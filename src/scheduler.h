//
// Created by Vulcan on 11/1/23.
//

#ifndef SJSU_CS_149_SCHEDULER_H
#define SJSU_CS_149_SCHEDULER_H


#include "config.h"
#include "queues.h"

typedef struct {
    struct pbc_queue_head priority_queue_heads[PRIO_LEVELS];
    struct pbc_queue_head blocked_queue_head;
} scheduler_t;

/**
 * This function is used to initialize the scheduler queues.
 * @param scheduler_queues
 */
void scheduler_init(scheduler_t *scheduler_queues);

/**
 * This function is used to enqueue a process into the scheduler queues.
 * @param scheduler_queues
 * @param pcb
 */
void scheduler_enqueue_process(scheduler_t *scheduler_queues, pcb_t *pcb);

/**
 * This function is used to dequeue a process from the scheduler queues. It
 * first checks the highest priority queue and then goes down the priority
 * levels.
 * @param scheduler_queues
 * @return
 */
pcb_t *scheduler_dequeue_process(scheduler_t *scheduler_queues);

void scheduler_block_process(scheduler_t *scheduler_queues, pcb_t *pcb);
pcb_t *scheduler_unblock_process(scheduler_t *scheduler_queues);

#endif //SJSU_CS_149_SCHEDULER_H
