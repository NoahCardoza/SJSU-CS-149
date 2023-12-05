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
struct pcb_queue_node {
    pcb_t *value;
    STAILQ_ENTRY(pcb_queue_node) entries;
};

/**
 * This data structure represents a queue head of PCBs.
 */
STAILQ_HEAD(pcb_queue_head, pcb_queue_node);

/**
 * This data structure represents an enqueued queue item of a PCB.
 */
struct pcb_nested_queue_node {
    struct pcb_queue_node *value;
    STAILQ_ENTRY(pcb_nested_queue_node) entries;
};

/**
 * This data structure represents a queue head of queue items of PCBs.
 */
STAILQ_HEAD(pcb_nested_queue_head, pcb_nested_queue_node);

/**
 * Enqueues a PCB into the queue.
 * @param head The queue to enqueue into.
 * @param pcb_el The PCB to enqueue.
 * @return The PCB that was enqueued.
 */
struct pcb_nested_queue_node *pcb_queue_enqueue(struct pcb_nested_queue_head *head, struct pcb_queue_node *pcb_el);

/**
 * Dequeues a PCB from the queue.
 * @param head The queue to dequeue from.
 * @return The PCB that was dequeued.
 */
struct pcb_queue_node *pcb_queue_dequeue(struct pcb_nested_queue_head *head);

/**
 * Prints a queue of PCBs.
 * @param head The queue to print.
 * @param include_priority Whether to include the priority column when printing.
 */
void pcb_queue_print(struct pcb_nested_queue_head *head, int include_priority);

#endif //SJSU_CS_149_QUEUES_H
