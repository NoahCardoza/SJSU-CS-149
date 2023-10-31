//
// Created by Vulcan on 10/30/23.
//

#include <stddef.h>
#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include "manager.h"
#include "cpu.h"
#include "pcb.h"

char* read_str_param_from_line(char *line) {
    return line + 2;
}

int read_int_param_from_line(char *line) {
    return atoi(read_str_param_from_line(line));
}

void execute_program_instruction(cpu_t cpu) {
    char *instruction = cpu.program->lines[cpu.program_counter];
    switch (instruction[0]) {
        case 'S': // set
            cpu.state = read_int_param_from_line(instruction);
            cpu.program_counter++;
            break;
        case 'A': // sdd
            cpu.state += read_int_param_from_line(instruction);
            cpu.program_counter++;
            break;
        case 'D': // sub
            cpu.state -= read_int_param_from_line(instruction);
            cpu.program_counter++;
            break;
        case 'B': // block
            break;
        case 'E': // terminate
            break;
        case 'F': // fork
            break;
        case 'R': // load another process from a file

            break;
    }
}

void manger_run(int stdin_fd) {
    cpu_t cpu;
    pcb_t pcb_table[100];
    pcb_t ready_queue[100];
    pcb_t blocked_queue[100];
    pcb_t *current_process = NULL;


    char line[100];
    FILE *stdin = fdopen(stdin_fd, "r");
    ssize_t n = 0;

    while ((n = getline((char **) line, (size_t *) sizeof(line), stdin)) > 0) {
        write(1, line, n);


        if (cpu.time_slice == cpu.used_time_slices) {
            cpu.time_slice = 0;
            cpu.used_time_slices = 0;
            current_process = NULL;
        }
    }
}