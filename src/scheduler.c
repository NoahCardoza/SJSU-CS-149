//
// Created by Vulcan on 11/1/23.
//

#include <assert.h>
#include "scheduler.h"

void scheduler_init(scheduler_t *scheduler_queues) {
    for (int i = 0; i < PRIO_LEVELS; i++) {
        STAILQ_INIT(&scheduler_queues->priority_queue_heads[i]);
    }
    STAILQ_INIT(&scheduler_queues->blocked_queue_head);
}

void scheduler_enqueue_process(scheduler_t *scheduler_queues, pcb_t *pcb) {
    assert(pcb->priority >= 0 && pcb->priority < PRIO_LEVELS);
    pbc_queue_enqueue(&scheduler_queues->priority_queue_heads[pcb->priority], pcb);
}

pcb_t *scheduler_dequeue_process(scheduler_t *scheduler_queues) {
    for (int i = 0; i < PRIO_LEVELS; i++) {
        pcb_t *pcb = pbc_queue_dequeue(&scheduler_queues->priority_queue_heads[i]);
        if (pcb != NULL) {
            return pcb;
        }
    }
    return NULL;
}

void scheduler_block_process(scheduler_t *scheduler_queues, pcb_t *pcb) {
    pbc_queue_enqueue(&scheduler_queues->blocked_queue_head, pcb);
}

pcb_t *scheduler_unblock_process(scheduler_t *scheduler_queues) {
    return pbc_queue_dequeue(&scheduler_queues->blocked_queue_head);
}


