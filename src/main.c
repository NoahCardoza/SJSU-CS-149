//
// Created by Vulcan on 10/30/23.
//

#include <stdio.h>
#include <sys/types.h>
#include <unistd.h>
#include <stdbool.h>
#include <assert.h>
#include "program.h"
#include "pcb.h"
#include "cpu.h"
#include "manager.h"
#include "config.h"
#include "queues.h"
#include "scheduler.h"

/**
 * This function is used to fork and pipe to the
 * manager process.
 * @return
 */
int fork_and_pipe() {
    int fd[2];

    if (pipe(fd) < 0) {
        perror("pipe");
    }

    pid_t pid = fork();
    if (pid < 0) {
        perror("fork");
    } else if (pid == 0) {
        close(fd[0]);
        manger_run(fd[1]);
    } else {
        close(fd[1]);
        char buf[1024];

        while (1) {
            int n = read(0, buf, sizeof(buf));
            if (n < 0) {
                perror("read");
            } else if (n == 0) {
                break;
            } else {
                write(fd[1], buf, n);
            }
        }
    }

    return 0;
}

int main(int argc, char **argv) {
//    fork_and_pipe();

    manger_run(0);

    return 0;
}