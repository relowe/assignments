#include "types.h"
#include "riscv.h"
#include "console.h"
#include "port.h"
#include "trap.h"
#include "mem.h"
#include "proc.h"

void swtch(struct context *old, struct context *new);

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

  // initialize other kernel subsystems
  port_init();
  proc_init();

  // load up the user space
  struct proc* init = proc_load_user_init();
  cpu.proc = init;
  init->state = RUNNING;
  swtch(&cpu.context, &init->context);
  while(1);

  panic("All done! (for now...)");
}
