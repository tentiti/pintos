#include <stdbool.h>
#ifndef USERPROG_SYSCALL_H
#define USERPROG_SYSCALL_H

// to use type definition same as Pintos manual
typedef int pid_t;
typedef int tid_t;

void syscall_init(void);

// // project 1
// void halt(void);
// void exit(int status);
// pid_t exec(const char *cmd_line);
// int wait(tid_t tid);
// int read(int fd, void *buffer, unsigned size);
// int write(int fd, const void *buffer, unsigned size);

// // project 1 additional system calls
// int fibonacci(int n);
// int max_of_four_int(int a, int b, int c, int d);

// // project 02
// bool create(const char *file, unsigned initial_size);
// bool remove(const char *file);
// int open(const char *file);
// void close(int fd);
// int filesize(int fd);
// void seek(int fd, unsigned position);
// unsigned tell(int fd);

// read, write -> improve from project 01.

#endif /* userprog/syscall.h */
