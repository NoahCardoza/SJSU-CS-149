//
// Created by Vulcan on 10/31/23.
//

#include "scheduler.h"
#include "config.h"
#include "manager.h"
#include <strings.h>
#include <assert.h>
#include <stdlib.h>
#include <stdio.h>
#include <stddef.h>
#include "pcb.h"
#include "cpu.h"
#include "../libs/zf_log/zf_log.h"



void context_switch_pcb_to_cpu(cpu_t *cpu, pcb_t *pcb) {
    ZF_LOGI("Context switching from PCB to CPU: process_id = %d", pcb->process_id);
    cpu->program = pcb->program;
    cpu->program_counter = pcb->program_counter;
    cpu->state = pcb->state;
    cpu->time_slice = 1 << pcb->priority;
    cpu->used_time_slices = 0;
}

void context_switch_cpu_to_pcb(cpu_t *cpu, pcb_t *pcb) {
    ZF_LOGI("Context switching from CPU to PCB: process_id = %d", pcb->process_id);
    pcb->program_counter = cpu->program_counter;
    pcb->program = cpu->program;
    pcb->state = cpu->state;
    pcb->cpu_time_used += cpu->used_time_slices;
}

void cpu_set_interrupt(cpu_t *cpu, int interrupt_id, size_t interrupt_argument) {
    cpu->interrupt_id = interrupt_id;
    cpu->interrupt_argument = interrupt_argument;
}

void cpu_state_set(cpu_t *cpu, int state) {
    ZF_LOGI("Setting CPU value to %d.", state);
    cpu->state = state;
}

void cpu_state_add(cpu_t *cpu, int state) {
    ZF_LOGI("Adding %d to CPU value.", state);
    cpu->state += state;
}

void cpu_state_sub(cpu_t *cpu, int state) {
    ZF_LOGI("Subtracting %d from CPU value.", state);
    cpu->state -= state;
}