//
// Created by Vulcan on 10/31/23.
//

#ifndef SJSU_CS_149_QUEUES_H
#define SJSU_CS_149_QUEUES_H

#include <stdio.h>
#include <stdlib.h>
#include <sys/queue.h>
#include "pcb.h"

struct pbc_queue_item {
    pcb_t *value;
    STAILQ_ENTRY(pbc_queue_item) entries;
};

STAILQ_HEAD(pbc_queue_head, pbc_queue_item);

struct pbc_queue_item_item {
    struct pbc_queue_item *value;
    STAILQ_ENTRY(pbc_queue_item_item) entries;
};

STAILQ_HEAD(pbc_queue_item_head, pbc_queue_item_item);

struct pbc_queue_item_item *pbc_queue_enqueue(struct pbc_queue_item_head *head, struct pbc_queue_item *pcb_el);

struct pbc_queue_item *pbc_queue_dequeue(struct pbc_queue_item_head *head);

void pbc_queue_print(struct pbc_queue_item_head *head, int include_priority);

#endif //SJSU_CS_149_QUEUES_H
