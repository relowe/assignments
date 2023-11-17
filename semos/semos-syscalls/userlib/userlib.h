#include "kernel/types.h"
#include "kernel/syscall.h"

// io.c
int getchar();
void putchar(int c);
void printf(char *, ...);

// syslib.c
int port_write(int, char*, int);
int port_read(int, char*, int);
int port_acquire(int);
int port_close(int);
int clone();
int load_elf(void*, int);
int getpid();
int getsize();
int resize(int);
int terminate(int);
int status(int);

// syscall.s
uint64 syscall(int, ...);
