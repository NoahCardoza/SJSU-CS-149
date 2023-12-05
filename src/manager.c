//
// Created by Vulcan on 10/30/23.
//

#include <stddef.h>
#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include <strings.h>
#include <pthread.h>
#include "manager.h"
#include "config.h"
#include "cpu.h"
#include "scheduler.h"
#include "../libs/zf_log/zf_log.h"
#include "pcb.h"

manager_handel_interrupt_f *interrupt_vector_table[] = {
        manager_handel_interrupt_terminate,
        manager_handel_interrupt_block,
        manager_handel_interrupt_fork,
        manager_handel_interrupt_load,
};

void print_system_status(manager_t *manager);

void check_time_slice(cpu_t *cpu, struct pcb_queue_node **current_pcb, scheduler_t *scheduler){//added - R
    assert(cpu->used_time_slices <= cpu->time_slice);
    assert(PRIO_LEVELS > (*current_pcb)->value->priority);

    if (cpu->time_slice == cpu->used_time_slices ) {//check if priority > 3
        ZF_LOGI("Time slice expired: pid=%d, priority=%d", (*current_pcb)->value->process_id, (*current_pcb)->value->priority);
        if ((*current_pcb)->value->priority < PRIO_LEVELS - 1) {
            (*current_pcb)->value->priority++;
        }

        scheduler_enqueue_process(scheduler, (*current_pcb));
        struct pcb_queue_node* pcb_el = scheduler_dequeue_process(scheduler);
        if (pcb_el == *current_pcb) { // if not context switch, reset time slice
            // increase allowed time slices when not context switching
            int new_quantum = 1 << (*current_pcb)->value->priority;
            cpu->time_slice += new_quantum;
            ZF_LOGI("Refreshing time slice: pid=%d, priority=%d, quantum=%d", (*current_pcb)->value->process_id, (*current_pcb)->value->priority, new_quantum);
        } else { // only switch if there is another process ready to run
            context_switch_cpu_to_pcb(cpu, (*current_pcb)->value);
            context_switch_pcb_to_cpu(cpu, pcb_el->value);
            *current_pcb = pcb_el;
        }
    }
}

void execute_program_instruction(manager_t *manager) {
    char *instruction = manager->cpu.program->lines[manager->cpu.program_counter++];
    instruction[strcspn(instruction, "\r\n")] = 0;
    ZF_LOGI("Executing instruction: \"%s\".", instruction);
    switch (instruction[0]) {
        case INSTRUCTION_SET:
            cpu_state_set(&manager->cpu, program_read_int_param_from_line(instruction));
            return;
        case INSTRUCTION_ADD:
            cpu_state_add(&manager->cpu, program_read_int_param_from_line(instruction));
            return;
        case INSTRUCTION_SUBTRACT:
            cpu_state_sub(&manager->cpu, program_read_int_param_from_line(instruction));
            return;
        case INSTRUCTION_BLOCK:
            cpu_set_interrupt(&manager->cpu, INTERRUPT_BLOCK, 0);
            return;
        case INSTRUCTION_TERMINATE:
            cpu_set_interrupt(&manager->cpu, INTERRUPT_TERMINATE, 0);
            return;
        case INSTRUCTION_FORK:
            cpu_set_interrupt(&manager->cpu, INTERRUPT_FORK,
                              program_read_int_param_from_line(instruction));
            return;
        case INSTRUCTION_LOAD:
            cpu_set_interrupt(&manager->cpu, INTERRUPT_LOAD,
                              (size_t) program_read_str_param_from_line(instruction));
            return;
    }
}

void manger_handel_command_process_time_slice(manager_t *manager) {
    ZF_LOGI("Processing command \"Q\".");

    if (manager->current_process == NULL) {
        manager->current_process = scheduler_dequeue_process(&manager->scheduler);
        if (manager->current_process == NULL) {
            return;
        }
        context_switch_pcb_to_cpu(&manager->cpu, manager->current_process->value);
    }

    manager->cpu.interrupt_id = INTERRUPT_NONE;
    execute_program_instruction(manager);
    manager_handel_interrupt(manager);
}

void manger_run() {
    manager_t manager;
    int running = 1;
    char line[LINE_BUFFER_SIZE];

    manager_init(&manager);

    scheduler_init(&manager.scheduler);
    scheduler_process_init(
        &manager.scheduler,
        0,
        0,
        program_get("init"),
        0,
        0,
        manager.time
    );

    manager.current_process = scheduler_dequeue_process(&manager.scheduler);
    context_switch_pcb_to_cpu(&manager.cpu, manager.current_process->value);

    while (running && fgets(line, LINE_BUFFER_SIZE, stdin)) {
        printf("$ ");
        ZF_LOGI("System time: %d", manager.time);
        switch (line[0]) {
            case 'Q':
                manger_handel_command_process_time_slice(&manager);
                break;
            case 'U':
                manager_handel_command_unblock_process(&manager.scheduler);
                break;
            case 'P':
                manager_handel_command_print_system_state(&manager);
                break;
            case 'T':
                running = manager_handel_command_terminate(&manager);
                break;
            default:
                printf("Invalid command.\n");
                break;
        }
    }

    fflush(stdout);
    fflush(stderr);
}

/**
 * On receiving a T command, the process
 * manager first spawns a reporter process and then terminates after termination of the
 * reporter process. The process manager ensures that no more than one reporter process is
 * running at any moment. It should also print average turnaround time of all processes.
 * @param manager
 */
int manager_handel_command_terminate(manager_t *manager) {
    ZF_LOGI("Processing command \"T\".");

    if (manager->processes_ended == 0) {
        printf("No processes have ended.\n");
        return 1;
    }

    #if FEATURE_THREAD_FOR_PRINT_STATE
        pthread_t thread;
        pthread_create(&thread, NULL, (void *) print_system_status, manager);
        pthread_join(thread, NULL);
    #else
        print_system_status(manager);
    #endif

    printf("Average Turnaround Time: %.2f", ((float)manager->total_turnaround / (float)manager->processes_ended)); // calculate average time

    return 0;
}

/**
 * On receiving a P command, the process manager spawns a new reporter process.
 * @param manager
 */
void manager_handel_command_print_system_state(manager_t *manager) {
    ZF_LOGI("Processing command \"P\".");
    #if FEATURE_THREAD_FOR_PRINT_STATE
        pthread_t thread;
        pthread_attr_t attr;
        pthread_attr_init(&attr);
        pthread_attr_setdetachstate(&attr, PTHREAD_CREATE_DETACHED);
        pthread_create(&thread, NULL, (void *) print_system_status, manager);
    #else
        print_system_status(manager);
    #endif
}

void manager_init(manager_t *manager) {
    manager->time = 0;
    manager->total_turnaround = 0;
    manager->processes_ended = 0;
    manager->current_process = NULL;
    scheduler_init(&manager->scheduler);
    cpu_init(&manager->cpu);
    pthread_mutex_init(&manager->reporter_mutex, NULL);
}

void manager_handel_command_unblock_process(scheduler_t *scheduler) {
    ZF_LOGI("Processing command \"U\".");
    struct pcb_queue_node *unblocked_pcb_el = NULL;

    unblocked_pcb_el = scheduler_unblock_process(scheduler);
    if (unblocked_pcb_el != NULL) {
        scheduler_enqueue_process(scheduler, unblocked_pcb_el);
    } else {
        printf("No process to unblock.\n");
    }
}

int manager_calculate_turnaround_time(manager_t *manager) {
    return (manager->time - manager->current_process->value->start_time);
}

void manager_handel_interrupt(manager_t *manager) {
    if (manager->cpu.interrupt_id != INTERRUPT_NONE) {
        interrupt_vector_table[manager->cpu.interrupt_id - 1](manager);
    }

    manager->time++;

    // interrupt block already does context switch
    // we don't want to increment time slice on the new process
    if (manager->cpu.interrupt_id != INTERRUPT_BLOCK) {
        manager->cpu.used_time_slices++;
    }

    // on none, fork, load
    if (manager->cpu.interrupt_id == INTERRUPT_NONE || manager->cpu.interrupt_id > 2) {
        check_time_slice(
                &manager->cpu,
                &manager->current_process, &manager->scheduler); //checking for preempting of program
    }
}

void manager_handel_interrupt_terminate(manager_t *manager) {
    ZF_LOGI("Terminating process.");

    manager->processes_ended++; //added to keep track of total process ended
    manager->total_turnaround += manager_calculate_turnaround_time(manager);

    scheduler_process_free(&manager->scheduler, manager->current_process);
    manager->current_process = scheduler_dequeue_process(&manager->scheduler);
    if (manager->current_process != NULL) {
        context_switch_pcb_to_cpu(&manager->cpu, manager->current_process->value);
    }
}

void manager_handel_interrupt_block(manager_t *manager) {
    ZF_LOGI("Blocking process.");
    context_switch_cpu_to_pcb(&manager->cpu, manager->current_process->value);
    if (manager->current_process->value->priority > 0) {
        manager->current_process->value->priority--;
    }

    scheduler_block_process(&manager->scheduler, manager->current_process);
    manager->current_process = scheduler_dequeue_process(&manager->scheduler);

    if (manager->current_process != NULL) {
        context_switch_pcb_to_cpu(&manager->cpu, manager->current_process->value);
    }
}

void manager_handel_interrupt_fork(manager_t *manager) {
    ZF_LOGI("Forking process.");
    scheduler_process_init(
            &manager->scheduler,
            manager->current_process->value->process_id,
            manager->current_process->value->priority,
            program_copy(manager->cpu.program),
            manager->cpu.state,
            manager->cpu.program_counter,
            manager->time                    // pass current system time
    );
    if (manager->cpu.interrupt_argument < 0) {
        fprintf(stderr, "Invalid fork (ln: %d): jump less than zero.\n", manager->cpu.program_counter);
        exit(1);
    }

    manager->cpu.program_counter += (int) manager->cpu.interrupt_argument;

    if (manager->cpu.program_counter >= manager->cpu.program->count) {
        fprintf(stderr, "Invalid fork (ln: %d): program counter overflow.\n",
                manager->cpu.program_counter - (int) manager->cpu.interrupt_argument);
        exit(1);
    }
}

void manager_handel_interrupt_load(manager_t *manager) {
    program_t *temp_program;
    ZF_LOGI("Loading another process from \"%s\".", (char *) manager->cpu.interrupt_argument);
    temp_program = manager->cpu.program;
    manager->cpu.program = program_get((char *) manager->cpu.interrupt_argument);
    manager->cpu.program_counter = 0;
    program_free(temp_program);
}

void print_system_status(manager_t *manager) {
    #if FEATURE_THREAD_FOR_PRINT_STATE
        pthread_mutex_lock(&manager->reporter_mutex);
    #endif

    printf("\n*******************************************************************\n"
             "*              The current system state is as follows             *\n"
             "*******************************************************************\n"
             );
    printf(  "| CURRENT TIME : %3d                                              |\n", manager->time);
    printf(  "+=================================================================+\n");
    printf(  "| RUNNING PROCESS                                                 |\n");
    printf(  "+=================================================================+\n");
    if (manager->current_process != NULL) {
        pcb_print_header(1);
        pcb_print(manager->current_process->value, 1);
        printf("+-----------------------------------------------------------------+\n");
    } else {
        printf("+-----------------------------------------------------------------+\n");
        printf("| * No process is currently running.                              |\n");
        printf("+-----------------------------------------------------------------+\n");
    }

    printf(  "+=================================================================+\n");
    printf(  "| CPU                                                             |\n");
    printf(  "+=================================================================+\n");
    cpu_print(&manager->cpu);

    printf(  "+=================================================================+\n");
    printf(  "| BLOCKED PROCESSES                                               |\n");
    printf(  "+=================================================================+\n");
    pcb_queue_print(&manager->scheduler.blocked_queue_head, 1);

    for (int priority = 0; priority < PRIO_LEVELS; ++priority) {
        printf(  "+=================================================================+\n");
        printf(  "| Priority Queue %d                                                |\n", priority);
        printf(  "+=================================================================+\n");
        pcb_queue_print(&manager->scheduler.priority_queue_heads[priority], 0);
    }
    printf("*******************************************************************\n");
    #if FEATURE_THREAD_FOR_PRINT_STATE
        pthread_mutex_unlock(&manager->reporter_mutex);
    #endif
}