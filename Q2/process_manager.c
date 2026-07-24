#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <signal.h>
#include <sys/wait.h>

int main() {
    pid_t pid1, pid2, pid3;
    int status;

    pid1 = fork();
    if (pid1 == 0) {
        printf("Child 1 (PID %d): doing work\n", getpid());
        sleep(2);
        printf("Child 1 (PID %d): finished\n", getpid());
        exit(0);
    }

    pid2 = fork();
    if (pid2 == 0) {
        printf("Child 2 (PID %d): doing work\n", getpid());
        sleep(100);
        exit(0);
    }

    pid3 = fork();
    if (pid3 == 0) {
        printf("Child 3 (PID %d): doing work\n", getpid());
        sleep(3);
        printf("Child 3 (PID %d): finished\n", getpid());
        exit(0);
    }

    printf("Parent (PID %d): created children %d, %d, %d\n", getpid(), pid1, pid2, pid3);

    sleep(5);

    printf("Parent: checking if Child 2 (PID %d) is still running\n", pid2);
    if (waitpid(pid2, &status, WNOHANG) == 0) {
        printf("Parent: Child 2 is unresponsive, sending SIGKILL\n");
        kill(pid2, SIGKILL);
        waitpid(pid2, &status, 0);
        printf("Parent: Child 2 terminated\n");
    }

    waitpid(pid1, &status, 0);
    printf("Parent: Child 1 exited with status %d\n", WEXITSTATUS(status));

    waitpid(pid3, &status, 0);
    printf("Parent: Child 3 exited with status %d\n", WEXITSTATUS(status));

    printf("Parent: all children handled, no zombies\n");
    return 0;
}
