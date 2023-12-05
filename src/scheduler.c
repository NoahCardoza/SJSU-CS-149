//
// Created by Vulcan on 11/1/23.
//

#include <assert.h>
#include "scheduler.h"
#include "../libs/zf_log/zf_log.h"

void scheduler_init(scheduler_t *scheduler) {
    STAILQ_INIT(&scheduler->pcb_table);
    for (int i = 0; i < PRIO_LEVELS; i++) {
        STAILQ_INIT(&scheduler->priority_queue_heads[i]);
    }
    STAILQ_INIT(&scheduler->blocked_queue_head);
}

void scheduler_enqueue_process(scheduler_t *scheduler, struct pcb_queue_node *pcb_el) {
    assert(pcb_el->value->priority >= 0 && pcb_el->value->priority < PRIO_LEVELS);
    ZF_LOGI("Enqueuing process: pid=%d, priority=%d", pcb_el->value->process_id, pcb_el->value->priority);
    pcb_queue_enqueue(&scheduler->priority_queue_heads[pcb_el->value->priority], pcb_el);
}

struct pcb_queue_node * scheduler_dequeue_process(scheduler_t *scheduler) {
    for (int i = 0; i < PRIO_LEVELS; i++) {
        struct pcb_queue_node *pcb_el = pcb_queue_dequeue(&scheduler->priority_queue_heads[i]);
        if (pcb_el != NULL) {
            ZF_LOGI("Dequeued process: pid=%d, priority=%d", pcb_el->value->process_id, pcb_el->value->priority);
            return pcb_el;
        }
    }
    ZF_LOGI("No processes to dequeue.");
    return NULL;
}

void scheduler_block_process(scheduler_t *scheduler, struct pcb_queue_node *pcb_el) {
    ZF_LOGI("Blocking process: pid=%d, priority=%d", pcb_el->value->process_id, pcb_el->value->priority);
    pcb_queue_enqueue(&scheduler->blocked_queue_head, pcb_el);
}

struct pcb_queue_node * scheduler_unblock_process(scheduler_t *scheduler) {
    struct pcb_queue_node *item = pcb_queue_dequeue(&scheduler->blocked_queue_head);
    if (item == NULL) {
        ZF_LOGI("No processes to unblock.");
    } else {
        ZF_LOGI("Unblocking process: pid=%d, priority=%d", item->value->process_id, item->value->priority);
    }
    return item;
}

void scheduler_process_init(scheduler_t *scheduler, int parent_pid, int priority, program_t *program,
                            int state, int program_counter, int time) {
    ZF_LOGI("Initializing process: parent_pid=%d, priority=%d, state=%d program_counter=%d, time=%d",
            parent_pid, priority, state, program_counter, time);
    pcb_t *pcb = pcb_create(parent_pid, priority, program, state, program_counter, time);

    struct pcb_queue_node *el = (struct pcb_queue_node *) malloc(sizeof(struct pcb_queue_node));
    if (!el) {
        perror("malloc");
        exit(1);
    }
    el->value = pcb;
    STAILQ_INSERT_TAIL(&scheduler->pcb_table, el, entries);

    // insert the pcb_table queue item into the priority queue
    // this makes termination of processes O(1)
    scheduler_enqueue_process(scheduler, el);
}

void scheduler_process_free(scheduler_t *scheduler, struct pcb_queue_node *el) {
    ZF_LOGI("Freeing process: pid=%d, priority=%d", el->value->process_id, el->value->priority);
    STAILQ_REMOVE(&scheduler->pcb_table, el, pcb_queue_node, entries);
    pcb_free(el->value);
    free(el);
}


