#include <stdio.h>   // For printf
#include <unistd.h>  // For getpid()

int main() {
    // Obtain the current process ID
    pid_t pid = getpid();

    // Print the process ID
    printf("The process ID is %d\n", pid);

    // Exit the program
    return 0;
}
