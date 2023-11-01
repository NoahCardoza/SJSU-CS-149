//
// Created by Vulcan on 11/1/23.
//

#include <assert.h>
#include "scheduler.h"

void scheduler_init(scheduler_t *scheduler_queues) {
    STAILQ_INIT(&scheduler_queues->pcb_table);
    for (int i = 0; i < PRIO_LEVELS; i++) {
        STAILQ_INIT(&scheduler_queues->priority_queue_heads[i]);
    }
    STAILQ_INIT(&scheduler_queues->blocked_queue_head);
}

void scheduler_enqueue_process(scheduler_t *scheduler_queues, struct pbc_queue_item *pcb_el) {
    assert(pcb_el->value->priority >= 0 && pcb_el->value->priority < PRIO_LEVELS);
    pbc_queue_enqueue(&scheduler_queues->priority_queue_heads[pcb_el->value->priority], pcb_el);
}

struct pbc_queue_item * scheduler_dequeue_process(scheduler_t *scheduler_queues) {
    for (int i = 0; i < PRIO_LEVELS; i++) {
        struct pbc_queue_item *pcb_el = pbc_queue_dequeue(&scheduler_queues->priority_queue_heads[i]);
        if (pcb_el != NULL) {
            return pcb_el;
        }
    }
    return NULL;
}

void scheduler_block_process(scheduler_t *scheduler_queues, struct pbc_queue_item *pcb_el) {
    pbc_queue_enqueue(&scheduler_queues->blocked_queue_head, pcb_el);
}

struct pbc_queue_item * scheduler_unblock_process(scheduler_t *scheduler_queues) {
    return pbc_queue_dequeue(&scheduler_queues->blocked_queue_head);
}

void scheduler_process_init(scheduler_t *scheduler, int parent_pid, program_t *program, int program_counter) {
    pcb_t *pcb = pcb_create(parent_pid, program, program_counter);

    struct pbc_queue_item *el = (struct pbc_queue_item *) malloc(sizeof(struct pbc_queue_item));

    if (el) {
        el->value = pcb;
    } else {
        perror("malloc");
        exit(1);
    }
    STAILQ_INSERT_TAIL(&scheduler->pcb_table, el, entries);

    scheduler_enqueue_process(scheduler, el);
}

void scheduler_process_free(scheduler_t *scheduler, struct pbc_queue_item *el) {
    // TODO: change this to to a doubly linked TAILQ for O(1) removal
    STAILQ_REMOVE(&scheduler->pcb_table, el, pbc_queue_item, entries);
    pcb_free(el->value);
    free(el);
}


