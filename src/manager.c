//
// Created by Vulcan on 10/30/23.
//

#include <stddef.h>
#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include <strings.h>
#include "manager.h"
#include "config.h"
#include "cpu.h"
#include "scheduler.h"
#include "../libs/zf_log/zf_log.h"


void print_system_status(int time, pcb_t* current_process, scheduler_t *scheduler);
char* read_str_param_from_line(char *line) {
    return line + 2;
}

int read_int_param_from_line(char *line) {
    return atoi(read_str_param_from_line(line));
}
void check_time_slice(cpu_t *cpu, struct pbc_queue_item **current_pcb, scheduler_t *scheduler){//added - R
    assert(cpu->used_time_slices <= cpu->time_slice);
    assert(PRIO_LEVELS > (*current_pcb)->value->priority);

    if (cpu->time_slice == cpu->used_time_slices ) {//check if priority > 3
        ZF_LOGI("Time slice expired: pid=%d, priority=%d", (*current_pcb)->value->process_id, (*current_pcb)->value->priority);
        if ((*current_pcb)->value->priority < PRIO_LEVELS - 1) {
            (*current_pcb)->value->priority++;
        }

        scheduler_enqueue_process(scheduler, (*current_pcb));
        struct pbc_queue_item* pcb_el = scheduler_dequeue_process(scheduler);
        if (pcb_el == *current_pcb) { // if not context switch, reset time slice
            cpu->used_time_slices = 0;
            cpu->time_slice = 1 << (*current_pcb)->value->priority;
            ZF_LOGI("Refreshing time slice: pid=%d, priority=%d, quantum=%d", (*current_pcb)->value->process_id, (*current_pcb)->value->priority, cpu->time_slice);
        } else { // only switch if there is another process ready to run
            context_switch_cpu_to_pcb(cpu, (*current_pcb)->value);
            context_switch_pcb_to_cpu(cpu, pcb_el->value);
            *current_pcb = pcb_el;
        }
    }

}
//added time - R
int execute_program_instruction(
        cpu_t *cpu,
        struct pbc_queue_item *pcb_el,
        scheduler_t *scheduler,
        int time) {

    assert(pcb_el != NULL);
    int int_param;
    char *str_param;
    char *instruction = cpu->program->lines[cpu->program_counter++];
    instruction[strcspn(instruction, "\r\n")] = 0;
    ZF_LOGI("Executing instruction: \"%s\".", instruction);
    switch (instruction[0]) {
        case 'S': // set
            int_param = read_int_param_from_line(instruction);
            ZF_LOGI("Setting CPU value to %d.", int_param);
            cpu->state = int_param;
            break;
        case 'A': // sdd
            int_param = read_int_param_from_line(instruction);
            ZF_LOGI("Adding %d to CPU value.", int_param);
            cpu->state += int_param;
            break;
        case 'D': // sub
            int_param = read_int_param_from_line(instruction);
            ZF_LOGI("Subtracting %d from CPU value.", int_param);
            cpu->state -= int_param;
            break;
        case 'B': // block
            ZF_LOGI("Blocking process.");
            return 2;
        case 'E': // terminate
            ZF_LOGI("Terminating process.");
            return 1;
        case 'F': // fork
            ZF_LOGI("Forking process.");
            scheduler_process_init(
                    scheduler,
                    pcb_el->value->process_id,
                    program_copy(cpu->program),
                    cpu->state,
                    cpu->program_counter,
                    time                    // pass current system time
                    );
            cpu->program_counter += read_int_param_from_line(instruction);
            break;
        case 'R': // load another process from a file
            str_param = read_str_param_from_line(instruction);
            ZF_LOGI("Loading another process from \"%s\".", str_param);
            program_t* temp_program = cpu->program;
            // TODO: ask "what does int undefined mean?"
            cpu->program = program_get(str_param);
            cpu->program_counter = 0;
            program_free(temp_program);
    }
    return 0;
}

void manger_run(int stdin_fd) {
    cpu_t cpu;
    scheduler_t scheduler;
    int time = 0;
    int total_turnaround = 0;
    int processes_ended = 0;
    struct pbc_queue_item *current_process = NULL;
    struct pbc_queue_item *unblocked_pcb_el = NULL;

    scheduler_init(&scheduler);
    scheduler_process_init(&scheduler, 0, program_get("init"), 0, 0, time);

    current_process = scheduler_dequeue_process(&scheduler);
    context_switch_pcb_to_cpu(&cpu, current_process->value);

    char *line;
    size_t len = 32;
    FILE *in = fdopen(stdin_fd, "r");
    int running = 1;

    line = (char *)malloc(len * sizeof(char));
    if( line == NULL)
    {
        perror("Unable to allocate buffer");
        exit(1);
    }
    int interrupt;

    while (running && !feof(in)) {
        printf("$ ");
        getline(&line, &len, in);
        switch (line[0]) {
            case 'Q': // end of one unit of time added Turnaround time and processed ended
                if (current_process == NULL) {
                    current_process = scheduler_dequeue_process(&scheduler);
                    if (current_process == NULL) {
                        break;
                    }
                    context_switch_pcb_to_cpu(&cpu, current_process->value);
                }
//                printf("All processes completed.\n");
//                print_system_status(time, (void*)0, &scheduler);
//                printf("Average total_turnaround time: %d\n", (total_turnaround / processes_ended));//calculate average time
                interrupt = execute_program_instruction(&cpu, current_process, &scheduler, time);
                if (interrupt == 1) {
                    time++;
                    total_turnaround = total_turnaround + (time - current_process->value->start_time); //TODO: ADD variable to keep track of Turnaround time
                    scheduler_process_free(&scheduler, current_process);
                    current_process = scheduler_dequeue_process(&scheduler);
                    processes_ended++; //added to keep track of total process ended
                    if (current_process == NULL) {
                        current_process = (void*)0;
                    } else {
                        context_switch_pcb_to_cpu(&cpu, current_process->value);
                    }
                } else if (interrupt == 2) {
                    context_switch_cpu_to_pcb(&cpu, current_process->value);
                    if (current_process->value->priority > 0) {
                        current_process->value->priority--;
                    }
                    scheduler_block_process(&scheduler, current_process);
                    current_process = scheduler_dequeue_process(&scheduler);
                    assert(current_process != NULL && "Process blocked without another process ready to run.");
                    context_switch_pcb_to_cpu(&cpu, current_process->value);
                }else {
                    time++;
                    cpu.used_time_slices++;
                    check_time_slice(&cpu, &current_process, &scheduler); //checking for preempting of program
                }
                break;
            case 'U': // unblock the first simulated process in blocked queue
                unblocked_pcb_el = scheduler_unblock_process(&scheduler);
                if (unblocked_pcb_el != NULL) {
                    scheduler_enqueue_process(&scheduler, unblocked_pcb_el);
                } else {
                    printf("No process to unblock.\n");
                }
                break;
            case 'P': // print the current state of the system
                ZF_LOGI("Printing system status.");
                // On receiving a P command, the process
                // manager spawns a new reporter process.
                print_system_status(time, current_process->value, &scheduler);
                break;
            case 'T': // print the average total_turnaround time and terminate the system
                ZF_LOGI("Printing turn around time.");
                // TODO: ask about storage of processes in pcb table
                // On receiving a T command, the process
                // manager first spawns a reporter process and then terminates after termination of the
                // reporter process. The process manager ensures that no more than one reporter process is
                // running at any moment.
                if (processes_ended == 0) {
                    printf("No processes have ended.\n");
                    break;
                } else {
                    printf("Average total_turnaround time: %d", (total_turnaround / processes_ended));//calculate average time
                }
                running = 0;
                break;
            default:
                printf("Invalid command.\n");
                break;
        }
    }

    // TODO: free rio

    free(line);
}

void print_system_status(int time, pcb_t* current_process, scheduler_t *scheduler) {
    printf("\n****************************************************************\n"
           "The current system state is as follows:\n"
           "****************************************************************\n");
    printf("CURRENT TIME: %d\n", time);

    printf("RUNNING PROCESS:\n");
    if (current_process != NULL) {
    print_running_process(current_process->process_id,
                          current_process->parent_process_id,
                          current_process->priority,
                          current_process->state,
                          current_process->start_time,
                          current_process->cpu_time_used);
    } else {
        printf("No process is currently running.\n");
    }

    printf("BLOCKED PROCESSES:\n");
    print_blocked_process(&scheduler->blocked_queue_head);

    printf("PROCESSES READY TO EXECUTE:\n");

    for (int priority = 0; priority < PRIO_LEVELS; ++priority) {
        printf("Queue of processes with priority %d:\n", priority);
        print_pcb_info(&scheduler->priority_queue_heads[priority]);
    }
    printf("****************************************************************\n");
}