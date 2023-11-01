//
// Created by Vulcan on 10/30/23.
//

#include <stddef.h>
#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include "manager.h"
#include "cpu.h"
#include "scheduler.h"
#include "../libs/zf_log/zf_log.h"

char* read_str_param_from_line(char *line) {
    return line + 2;
}

int read_int_param_from_line(char *line) {
    return atoi(read_str_param_from_line(line));
}

void execute_program_instruction(cpu_t *cpu, struct pbc_queue_item *pcb_el, scheduler_t *scheduler) {
    assert(pcb_el != NULL);
    int int_param;
    char *str_param;
    char *instruction = cpu->program->lines[cpu->program_counter];
    ZF_LOGI("Executing instruction: \"%s\".\n", instruction);

    switch (instruction[0]) {
        case 'S': // set
            int_param = read_int_param_from_line(instruction);
            ZF_LOGI("Setting CPU value to %d.\n", int_param);
            cpu->state = int_param;
            cpu->program_counter++;
            break;
        case 'A': // sdd
            int_param = read_int_param_from_line(instruction);
            ZF_LOGI("Adding %d to CPU value.\n", int_param);
            cpu->state += int_param;
            cpu->program_counter++;
            break;
        case 'D': // sub
            int_param = read_int_param_from_line(instruction);
            ZF_LOGI("Subtracting %d from CPU value.\n", int_param);
            cpu->state -= int_param;
            cpu->program_counter++;
            break;
        case 'B': // block
            ZF_LOGI("Blocking process.\n");
            cpu->program_counter++;
            context_switch_cpu_to_pcb(cpu, pcb_el->value);
            if (pcb_el->value->priority > 0) {
                pcb_el->value->priority--;
            }
            scheduler_block_process(scheduler, pcb_el);
            // TODO: upgrade priority
            pcb_el = scheduler_dequeue_process(scheduler);
            context_switch_pcb_to_cpu(cpu, pcb_el->value);
            break;
        case 'E': // terminate
            ZF_LOGI("Terminating process.\n");
            scheduler_process_free(scheduler, pcb_el);
            pcb_el = scheduler_dequeue_process(scheduler);
            assert(pcb_el != NULL);
            context_switch_pcb_to_cpu(cpu, pcb_el->value);
            break;
        case 'F': // fork
            ZF_LOGI("Forking process.\n");
            // TODO: include current system time
            // TODO: pass value of cpu state
            scheduler_process_init(
                    scheduler,
                    pcb_el->value->process_id,
                    program_copy(cpu->program),
                    cpu->program_counter + 1
                    );
            cpu->program_counter += read_int_param_from_line(instruction) + 1;


            break;
        case 'R': // load another process from a file
            str_param = read_str_param_from_line(instruction);
            ZF_LOGI("Loading another process from \"%s\".\n", str_param);
            free(cpu->program);
            // TODO: ask "what does int undefined mean?"
            cpu->program = program_get(str_param);
            cpu->program_counter = 0;
            // TODO: run next command
            execute_program_instruction(cpu, pcb_el, scheduler); // TODO: ask morty
            return;
    }
    // TODO: increment machine time
//    cpu->used_time_slices++;
    //        cpu.time_slice
//    cpu.used_time_slices++;
    // TODO: preempt process when it runs out of time slices
//        if (cpu.time_slice == cpu.used_time_slices) {
//            cpu.time_slice = 0;
//            cpu.used_time_slices = 0;
//            context_switch_pcb_to_cpu(&cpu, current_process->value);
//            scheduler_enqueue_process(&scheduler, current_process);
//            current_process = scheduler_dequeue_process(&scheduler);
//    if (pcb_el->value->priority < 0) {
//        pcb_el->value->priority++;
//    }
//        }
}

void manger_run(int stdin_fd) {
    cpu_t cpu;
    scheduler_t scheduler;
    int time = 0;
    struct pbc_queue_item *current_process = NULL;
    struct pbc_queue_item *unblocked_pcb_el = NULL;

    scheduler_init(&scheduler);
    scheduler_process_init(&scheduler, 0, program_get("init"), 0);

    current_process = scheduler_dequeue_process(&scheduler);
    context_switch_pcb_to_cpu(&cpu, current_process->value);

    char *line;
    size_t len = 32;
    FILE *in = fdopen(stdin_fd, "r");

    while (!feof(in)) {
        printf("Enter command: ");
        getline(&line, &len, in);
        printf("%s", line);
        switch (line[0]) {
            case 'Q': // end of one unit of time
                execute_program_instruction(&cpu, current_process, &scheduler);
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
                // On receiving a P command, the process
                // manager spawns a new reporter process.
                printf("Not implemented.\n");
                break;
            case 'T': // print the average turnaround time and terminate the system
                // TODO: ask about storage of processes in pcb table
                // On receiving a T command, the process
                // manager first spawns a reporter process and then terminates after termination of the
                // reporter process. The process manager ensures that no more than one reporter process is
                // running at any moment.
                printf("Not implemented.\n");
                break;
            default:
                printf("Invalid command.\n");
                break;
        }
    }

    free(line);
}