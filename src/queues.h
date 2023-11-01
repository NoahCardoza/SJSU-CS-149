//
// Created by Vulcan on 10/31/23.
//

#ifndef SJSU_CS_149_QUEUES_H
#define SJSU_CS_149_QUEUES_H

#include <stdio.h>
#include <stdlib.h>
#include <sys/queue.h>
#include "pcb.h"

struct pbc_queue {
    pcb_t *value;
    STAILQ_ENTRY(pbc_queue) entries;
};

STAILQ_HEAD(pbc_queue_head, pbc_queue);

void pbc_queue_enqueue(struct pbc_queue_head *head, pcb_t *pcb);

pcb_t *pbc_queue_dequeue(struct pbc_queue_head *head);

#endif //SJSU_CS_149_QUEUES_H
