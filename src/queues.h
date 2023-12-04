//
// Created by Vulcan on 10/31/23.
//

#ifndef SJSU_CS_149_QUEUES_H
#define SJSU_CS_149_QUEUES_H

#include <stdio.h>
#include <stdlib.h>
#include <sys/queue.h>
#include "pcb.h"

/**
 * This data structure represents an enqueued PCB.
 */
struct pbc_queue_node {
    pcb_t *value;
    STAILQ_ENTRY(pbc_queue_node) entries;
};

/**
 * This data structure represents a queue head of PCBs.
 */
STAILQ_HEAD(pbc_queue_head, pbc_queue_node);

/**
 * This data structure represents an enqueued queue item of a PCB.
 */
struct pbc_nested_queue_node {
    struct pbc_queue_node *value;
    STAILQ_ENTRY(pbc_nested_queue_node) entries;
};

/**
 * This data structure represents a queue head of queue items of PCBs.
 */
STAILQ_HEAD(pbc_nested_queue_head, pbc_nested_queue_node);

/**
 * Enqueues a PCB into the queue.
 * @param head The queue to enqueue into.
 * @param pcb_el The PCB to enqueue.
 * @return The PCB that was enqueued.
 */
struct pbc_nested_queue_node *pbc_queue_enqueue(struct pbc_nested_queue_head *head, struct pbc_queue_node *pcb_el);

/**
 * Dequeues a PCB from the queue.
 * @param head The queue to dequeue from.
 * @return The PCB that was dequeued.
 */
struct pbc_queue_node *pbc_queue_dequeue(struct pbc_nested_queue_head *head);

/**
 * Prints a queue of PCBs.
 * @param head The queue to print.
 * @param include_priority Whether to include the priority column when printing.
 */
void pbc_queue_print(struct pbc_nested_queue_head *head, int include_priority);

#endif //SJSU_CS_149_QUEUES_H
