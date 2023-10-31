//
// Created by Vulcan on 10/31/23.
//

#include "pcb.h"
#include "program.h"
#include "manager.h"
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include <unistd.h>
#include <sys/types.h>
#include "cpu.h"

void context_switch_pcb_to_cpu(cpu_t *cpu, pcb_t *pcb) {
    cpu->program = pcb->program;
    cpu->program_counter = pcb->program_counter;
    cpu->state = pcb->state;
    cpu->time_slice = 0; // TODO: where do we get this number from?
    cpu->used_time_slices = 0;
}

void context_switch_cpu_to_pcb(cpu_t *cpu, pcb_t *pcb) {
    pcb->program_counter = cpu->program_counter;
    pcb->state = cpu->state;
    pcb->cpu_time_used += cpu->used_time_slices;
}