//
// Created by Vulcan on 10/31/23.
//

#include "pcb.h"
#include "cpu.h"
#include "../libs/zf_log/zf_log.h"

void cpu_init(cpu_t *cpu) {
    cpu->program = NULL;
    cpu->program_counter = 0;
    cpu->state = 0;
    cpu->time_slice = 0;
    cpu->used_time_slices = 0;
    cpu->interrupt_id = 0;
    cpu->interrupt_argument = 0;
}

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

void cpu_state_set(cpu_t *cpu, int value) {
    ZF_LOGI("Setting CPU value to %d.", value);
    cpu->state = value;
}

void cpu_state_add(cpu_t *cpu, int value) {
    ZF_LOGI("Adding %d to CPU value.", value);
    cpu->state += value;
}

void cpu_state_sub(cpu_t *cpu, int value) {
    ZF_LOGI("Subtracting %d from CPU value.", value);
    cpu->state -= value;
}

void cpu_print(cpu_t *cpu) {
    printf("+-----------------------------------------------------------------+\n");
    printf("| PC |    State | Max Slices | Used Slices | INT |        INT ARG |\n");
    printf("+-----------------------------------------------------------------+\n");
    printf("| %2d | %8d | %10d | %11d | %3d | 0x%012zx |\n",
           cpu->program_counter, cpu->state, cpu->time_slice, cpu->used_time_slices,
           cpu->interrupt_id, cpu->interrupt_argument);
    printf("+-----------------------------------------------------------------+\n");
}