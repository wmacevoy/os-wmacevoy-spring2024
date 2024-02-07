
# Fork & Exec Example

## You
please make an example of using fork and exec to launch a program as a child.  Have the parent wait for the child to exit before exiting.

## ChatGPT

Certainly, Warren! I'll provide a simple example using the `fork()` and `exec()` system calls in C on a Unix-based system, such as Linux or macOS. The `fork()` system call creates a new process by duplicating the calling process, and `exec()` replaces the current process image with a new program image. The parent process will use `wait()` to wait for the child process to exit before it exits itself.

### Synopsis
1. **Fork:** The parent process calls `fork()`, creating a child process.
2. **Child Process Execution:** If `fork()` returns 0, we are in the child process, where `exec()` is called to replace the child's image with a new program.
3. **Parent Waits:** The parent process, identified by a positive return value from `fork()`, waits for the child to terminate using `wait()`.
4. **Child and Parent Exit:** The child process exits after execution, and the parent exits after the child.

### Example Code

```c
#include <stdio.h>
#include <stdlib.h>
#include <sys/wait.h>
#include <unistd.h>

int main() {
    pid_t pid = fork(); // Forking a new process

    if (pid == -1) {
        // Fork failed
        perror("fork");
        exit(EXIT_FAILURE);
    } else if (pid == 0) {
        // Child process
        printf("Child Process: PID = %d\n", getpid());

        // Replace the child process with a new program, for example "/bin/ls"
        execl("/bin/ls", "ls", NULL); // Executes "ls" command

        // execl only returns if there is an error
        perror("execl");
        exit(EXIT_FAILURE);
    } else {
        // Parent process
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
```

### Explanation
- **`fork()`** creates a new process. The child gets a return value of 0, and the parent gets the PID of the child.
- **In the child process**, we use `execl()` to run a new program, here the `/bin/ls` command to list directory contents.
- **In the parent process**, `waitpid()` is used to wait for the child's termination. `WIFEXITED(status)` checks if the child exited normally, and `WEXITSTATUS(status)` retrieves the exit status of the child.
- **Error Handling:** Both `fork()` and `execl()` have error handling to catch and report failures.

### Further Reading and Experimentation
- You can replace `execl("/bin/ls", "ls", NULL)` with any other program and its arguments. Just ensure the first argument is the path to the executable and the second is the first argument to the program, conventionally the program's name.
- Explore other `exec` family functions like `execlp()`, `execvp()`, etc., for different use cases, such as searching for executables in the `PATH`.
- Consider error handling and edge cases for robustness, such as handling signals or the child process running indefinitely.

This example demonstrates the basic use of `fork()` and `exec()` for process creation and management in a Unix environment, which can be a foundation for more complex multi-process applications.