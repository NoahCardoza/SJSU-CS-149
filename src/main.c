//
// Created by Vulcan on 10/30/23.
//

#include <stdio.h>
#include "program.h"
#include "pcb.h"
#include "cpu.h"


int main(int argc, char **argv) {
//    int fd[2];
//
//    if (pipe(fd) < 0) {
//        perror("pipe");
//    }
//
//    pid_t pid = fork();
//    if (pid < 0) {
//        perror("fork");
//    } else if (pid == 0) {
//        close(fd[0]);
//        manger_run(fd[1]);
//    } else {
//        close(fd[1]);
//        char buf[1024];
//
//        while (1) {
//            int n = read(0, buf, sizeof(buf));
//            if (n < 0) {
//                perror("read");
//            } else if (n == 0) {
//                break;
//            } else {
//                write(fd[1], buf, n);
//            }
//        }
//    }

    program_t  *p = program_get("init");
    pcb_t *pcb = pcb_create(0, p, 0);
    cpu_t cpu;

    context_switch_pcb_to_cpu(&cpu, pcb);

    context_switch_cpu_to_pcb(&cpu, pcb);

    for (int i = 0; i < p->count; i++) {
        printf("%s", p->lines[i]);
    }
    program_free(p);

    return 0;
}