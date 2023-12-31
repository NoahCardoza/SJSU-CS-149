#include <stdio.h>
#include <unistd.h>
#include <string.h>
#include "manager.h"

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

    fflush(stdout);
    fflush(stderr);
    pid_t pid = fork();
    if (pid < 0) {
        perror("fork");
    } else if (pid == 0) {
        close(fd[1]);
        dup2(fd[0], 0);
        manger_run();
    } else {
        close(fd[0]);
        close(1);
        close(2);
        char line[LINE_BUFFER_SIZE];

        while (fgets(line, LINE_BUFFER_SIZE, stdin)) {
            write(fd[1], line, strlen(line));
            sleep(1);
        }

        close(fd[1]);
        wait(NULL);
    }

    return 0;
}

int main(int argc, char **argv) {
    if (argc > 1) {
        chdir(argv[1]);
        if (argc > 2) {
            freopen(argv[2], "r", stdin);
        }
    }

    #if FEATURE_PROCESS_FOR_MANAGER
        fork_and_pipe();
    #else
        manger_run();
    #endif

    return 0;
}