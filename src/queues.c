//
// Created by Vulcan on 10/31/23.
//

/**
* The process manager also implements a scheduling policy. You may experiment with a
scheduling policy of multiple queues with priority classes. In this policy, the first simulated
process (created by the process manager) starts with priority 0 (highest priority). There are a
maximum of four priority classes. Time slice (quantum size) for priority class 0 is 1 unit of
time; time slice for priority class 1 is 2 units of time; time slice for priority class 2 is 4 units of
time; and time slice for priority class 3 is 8 units of time. If a running process uses its time
slice completely, it is preempted and its priority is lowered. If a running process blocks before
its allocated quantum expires, its priority is raised.
*/

#include <stdio.h>
#include <stdlib.h>
#include <sys/queue.h>
#include "pcb.h"
#include "config.h"
#include "queues.h"

struct pbc_queue_item_item *pbc_queue_enqueue(struct pbc_queue_item_head *head, struct pbc_queue_item *pcb_el) {
    struct pbc_queue_item_item *el = (struct pbc_queue_item_item *) malloc(sizeof(struct pbc_queue_item_item));
    if (el) {
        el->value = pcb_el;
    } else {
        perror("malloc");
        exit(1);
    }
    STAILQ_INSERT_TAIL(head, el, entries);
    return el;
}

struct pbc_queue_item *pbc_queue_dequeue(struct pbc_queue_item_head *head) {
    struct pbc_queue_item *pcb_el;
    struct pbc_queue_item_item *el;
    el = STAILQ_FIRST(head);

    if (el == NULL) {
        return NULL;
    }

    STAILQ_REMOVE_HEAD(head, entries);
    pcb_el = el->value;
    free(el);
    return pcb_el;
}

void print_running_process(int pid, int ppid, int priority, int value, int start_time, int used_time) {
    printf("%d, %d, %d, %d, %d, %d\n", pid, ppid, priority, value, start_time, used_time);
}


void print_blocked_process(struct pbc_queue_item_head *head) {
    struct pbc_queue_item_item *pcb;

    STAILQ_FOREACH(pcb, head, entries) {
        printf("%d, %d, %d, %d, %d, %d\n",
               pcb->value->value->process_id, pcb->value->value->parent_process_id, pcb->value->value->priority, pcb->value->value->state, pcb->value->value->start_time, pcb->value->value->cpu_time_used);
    }
}

void print_pcb_info(struct pbc_queue_item_head *head) {
    struct pbc_queue_item_item *pcb;

    STAILQ_FOREACH(pcb, head, entries) {
        printf("%d, %d, %d, %d, %d\n",
               pcb->value->value->process_id, pcb->value->value->parent_process_id, pcb->value->value->state, pcb->value->value->start_time, pcb->value->value->cpu_time_used);
    }
}
