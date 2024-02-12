#include <stdio.h>
#include <stdarg.h>
#include <unistd.h>
#include <fcntl.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>
#include <sys/wait.h>

// Function to acquire a write lock on a file descriptor
int lock_file(int fd)
{
    struct flock fl;
    memset(&fl, 0, sizeof(fl));
    fl.l_type = F_WRLCK; // Write lock
    fl.l_whence = SEEK_SET;
    fl.l_start = 0;
    fl.l_len = 0;                    // Lock the entire file
    return fcntl(fd, F_SETLKW, &fl); // Block until lock is acquired
}

// Function to release a write lock on a file descriptor
int unlock_file(int fd)
{
    struct flock fl;
    memset(&fl, 0, sizeof(fl));
    fl.l_type = F_UNLCK; // Unlock
    fl.l_whence = SEEK_SET;
    fl.l_start = 0;
    fl.l_len = 0; // Unlock the entire file
    return fcntl(fd, F_SETLK, &fl);
}

// The vprintf-like function for logging to a file
void log_vprintf(const char *log_file, const char *format, va_list args)
{
    int fd = open(log_file, O_WRONLY | O_CREAT | O_APPEND, 0666);
    if (fd == -1)
    {
        perror("open log file");
        return;
    }

    // Lock the file for exclusive write access
    if (lock_file(fd) == -1)
    {
        perror("lock log file");
        close(fd);
        return;
    }

    // Use vdprintf to write formatted output to the file descriptor
    if (vdprintf(fd, format, args) < 0)
    {
        perror("write log");
    }

    // Unlock the file
    if (unlock_file(fd) == -1)
    {
        perror("unlock log file");
    }

    close(fd);
}

// Wrapper function that takes variadic arguments like printf
void log_printf(const char *log_file, const char *format, ...)
{
    va_list args;
    va_start(args, format);
    log_vprintf(log_file, format, args);
    va_end(args);
}

void wait_children()
{
    pid_t child;
    int status;
    while ((child = waitpid(-1, &status, 0)) != -1)
    {
        // Check if the child exited normally
        if (WIFEXITED(status) && WEXITSTATUS(status) == 0)
        {
            //ok
        }
        else
        {
            printf("Process %d exited abnormally\n", child);
        }
    }

    if (errno != ECHILD)
    {
        perror("waitpid");
        exit(EXIT_FAILURE);
    }
}

int main(int argc, const char *argv[])
{
    double p = 1;
    int n = 5;
    int x = 1, y = 2, z = 3;

    log_printf("prob.csv", "p,x,y,z\n");
    pid_t root_pid = getpid();

    for (int i = 0; i < n; ++i)
    {
        z = z + 1;
        if (fork() == 0)
        {
            p *= 0.3;
            x += 2;
        }
        else
        {
            p *= 0.7;
            x -= 2;
        }
        if (x == 4)
        {
            y = 2;
        }
        else
        {
            if (fork() == 0)
            {
                p *= 0.25;
                y += 1;
            }
            else if (fork() == 0)
            {
                p *= 0.25;
                y -= 1;
            }
            else
            {
                p *= 0.5;
                // dont change y
            }
        }
    }
    log_printf("prob.csv", "%lf,%d,%d,%d\n", p, x, y, z);
    wait_children();
    if (getpid() == root_pid)
    {
        printf("done.\n");
    }
    return 0;
};
