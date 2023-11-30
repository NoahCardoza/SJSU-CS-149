//
// Created by Vulcan on 10/31/23.
//

#include "pcb.h"
#include "cpu.h"
#include "../libs/zf_log/zf_log.h"



void context_switch_pcb_to_cpu(cpu_t *cpu, pcb_t *pcb) {
    ZF_LOGI("Context switching from PCB to CPU:\n\tprocess_id: %d\n", pcb->process_id);
    cpu->program = pcb->program;
    cpu->program_counter = pcb->program_counter;
    cpu->state = pcb->state;
    cpu->time_slice = priorityArr[pcb->priority]; // TODO: where do we get this number from?
    cpu->used_time_slices = 0;
}

void context_switch_cpu_to_pcb(cpu_t *cpu, pcb_t *pcb) {
    ZF_LOGI("Context switching from CPU to PCB:\n\tprocess_id: %d\n", pcb->process_id);
    pcb->program_counter = cpu->program_counter;
    pcb->state = cpu->state;
    pcb->cpu_time_used += cpu->used_time_slices;
}