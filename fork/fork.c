#include <stdio.h>
#include <stdlib.h>
#include <sys/wait.h>
#include <unistd.h>

int state = 36;

int main() {
    pid_t parent_pid = getpid();
    printf("Grandparent is %d\n",getppid());
    printf("Parent PID %d initializing....",getpid());
    sleep(1);
    printf("done\n");

    printf("state for pid %d is %d at line %d\n",getpid(),state,__LINE__);

    pid_t pid = fork(); // Forking a new process

    printf("state for pid %d is %d at line %d\n",getpid(),state,__LINE__);

    state = 47;

    printf("state for pid %d is %d at line %d\n",getpid(),state,__LINE__);

    if (pid == -1) {
        // Fork failed
        perror("fork");
        exit(EXIT_FAILURE);
    } else if (pid == 0) {
        // Child process
        printf("Child Process: PID = %d, Parent's PID = %d\n", getpid(), getppid());
        printf("Child process working before exec...");
        state = 100;

        printf("state for pid %d is %d at line %d\n",getpid(),state,__LINE__);

        sleep(4);
        printf("done.\n");

        // Replace the child process with a new program, for example "/bin/ls"
        execl("/bin/bash", "bash","-c","echo $$", NULL); // Executes "bash" and reports its procces id

        // execl only returns if there is an error
        perror("execl");
        exit(EXIT_FAILURE);
    } else {
        // Parent process
        printf("Parent process working before waiting for child to exit.");
        sleep(1);
        printf("done.\n");
        int status;
        waitpid(pid, &status, 0); // Wait for the child to complete
        if (WIFEXITED(status)) {
            printf("Parent Process: Child exited with status = %d\n", WEXITSTATUS(status));
        } else {
            printf("Parent Process: Child terminated abnormally\n");
        }
    }

    return EXIT_SUCCESS;
}
