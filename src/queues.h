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
 * This data structure represents a queue of PCBs.
 */
struct pbc_queue_item {
    pcb_t *value;
    STAILQ_ENTRY(pbc_queue_item) entries;
};

STAILQ_HEAD(pbc_queue_head, pbc_queue_item);

/**
 * This data structure represents a queue of queues of PCBs.
 */
struct pbc_queue_item_item {
    struct pbc_queue_item *value;
    STAILQ_ENTRY(pbc_queue_item_item) entries;
};

STAILQ_HEAD(pbc_queue_item_head, pbc_queue_item_item);

/**
 * Enqueues a PCB into the queue.
 * @param head The queue to enqueue into.
 * @param pcb_el The PCB to enqueue.
 * @return The PCB that was enqueued.
 */
struct pbc_queue_item_item *pbc_queue_enqueue(struct pbc_queue_item_head *head, struct pbc_queue_item *pcb_el);

/**
 * Dequeues a PCB from the queue.
 * @param head The queue to dequeue from.
 * @return The PCB that was dequeued.
 */
struct pbc_queue_item *pbc_queue_dequeue(struct pbc_queue_item_head *head);

/**
 * Prints a queue of PCBs.
 * @param head The queue to print.
 * @param include_priority Whether to include the priority column when printing.
 */
void pbc_queue_print(struct pbc_queue_item_head *head, int include_priority);

#endif //SJSU_CS_149_QUEUES_H
