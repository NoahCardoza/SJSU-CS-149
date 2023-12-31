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
#include "queues.h"

struct pcb_nested_queue_node *pcb_queue_enqueue(struct pcb_nested_queue_head *head, struct pcb_queue_node *pcb_el) {
    struct pcb_nested_queue_node *el = (struct pcb_nested_queue_node *) malloc(sizeof(struct pcb_nested_queue_node));
    if (el) {
        el->value = pcb_el;
    } else {
        perror("malloc");
        exit(1);
    }
    STAILQ_INSERT_TAIL(head, el, entries);
    return el;
}

struct pcb_queue_node *pcb_queue_dequeue(struct pcb_nested_queue_head *head) {
    struct pcb_queue_node *pcb_el;
    struct pcb_nested_queue_node *el;
    el = STAILQ_FIRST(head);

    if (el == NULL) {
        return NULL;
    }

    STAILQ_REMOVE_HEAD(head, entries);
    pcb_el = el->value;
    free(el);
    return pcb_el;
}


void pcb_queue_print(struct pcb_nested_queue_head *head, int include_priority) {
    struct pcb_nested_queue_node *pcb;
    if (head->stqh_first == NULL) {
        printf("| Queue is empty.                                                 |\n");
        return;
    }

    pcb_print_header(include_priority);
    STAILQ_FOREACH(pcb, head, entries) {
        pcb_print(pcb->value->value, include_priority);
    }
}
