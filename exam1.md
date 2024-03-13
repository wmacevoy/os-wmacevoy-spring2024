# Exam 1

1. What is an operating system?
1. What is the difference between a kernel and a process?
1. What are some scheduling algorithms?
1. Why do operating systems use geometric averages for statistics?
1. What are advantags and disadvantagegs of the unix fork/exec process creation model compared to the windows CreateProcess model?
1. What is the difference betewen a preemptive and non-preemptive kernel?
1. How does a user-land application "escape" to the kernel for a kernel service?
1. It would be simpler to allow subroutine calls directly to the kernel address space.  Why is this a bad idea?
1. What is the layout of a process (VonNeumann process model)?
1. What are some security measures the operating system does to make buffer overflow attacks hard to use?
1. A typical virtual address space for a process is 127 terabytes.  How does a modern operating system manage such a large address space with a limited amount of physical memeory.

## Memory

1. Why isn't all of physical memory considered "cache"
1. What is the typical block size for a page of ram?
1. What happens with a cache miss?
1. Why is accessing memory sequentially more efficient than randomly?
1. What are some memory-related security attacks?
1. What is ECC ram and who should use it?
1. What is a swap file for?
1. In a secure computing environment, why might you
want to disable swap?
1. Why is it challenging to write a secure application
in an "everything is an object" langage which stores all state on the heap?
1. How does fork() save memory for applications that
have similar behavior, like tabs on a chrome browser?
1. shared libraries also save memory.  how do they work?
1. You are building a robot control system whith many interacting components for sensing and control,and where speed is important.  How would you share state between these components?
1. You are building a video decoding library that plays audio and video streams for applications.  How would you architect working with the GPU on such a system?

what is the output of assume the parent is pid 1000,
and the child is pid 1001.

```C
int main() {
    int x = 1;
    if (fork()==0) {
        ++x;
    }
    printf("pid=%d, x=%d\n",getpid(),x);
}
```