#include "types.h"
#include "riscv.h"
#include "console.h"
#include "port.h"
#include "trap.h"
#include "mem.h"
#include "proc.h"
#include "scheduler.h"

void swtch(struct context *old, struct context *new);
extern char _binary_user_idle_start; 

static void load_idle(void);

// start() jumps here in supervisor mode
void
main()
{
  // initialize uart
  uartinit();
  printf("\n");
  printf("SEMOS kernel is booting\n");
  printf("\n");

  // initialize traps
  trapinit();

  //initialize virtual memory
  vm_init();

  //initialize the device interrupts
  plicinit();

  // initialize other kernel subsystems
  port_init();
  proc_init();

  // load up the user space
  proc_load_user_init();
  //load_idle();

  // enter the scheduler
  scheduler();

  panic("Exited Scheduler");
}


static void
load_idle(void)
{
    void *bin = &_binary_user_idle_start;
    struct proc *p;

    p = proc_alloc();
    if(!p) {
        panic("Could not allocate init!");
    }
    proc_load_elf(p, bin);

    return p;
}
