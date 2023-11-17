#include "types.h"
#include "memlayout.h"
#include "riscv.h"
#include "console.h"

uint ticks;

extern char trampoline[], uservec[], userret[];

// in kernelvec.S, calls kerneltrap().
void kernelvec();

extern int devintr();


// set up to take exceptions and traps while in the kernel.
void
trapinit(void)
{
  w_stvec((uint64)kernelvec);
}

//
// handle an interrupt, exception, or system call from user space.
// called from trampoline.S
//
void
usertrap(void)
{
    panic("Unexpected usertrap");
}


// interrupts and exceptions from kernel code go here via kernelvec,
// on whatever the current kernel stack is.
void 
kerneltrap()
{
    panic("Unexpected kerneltrap");
}
