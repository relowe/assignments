#include "types.h"
#include "riscv.h"
#include "console.h"
#include "port.h"
#include "mem.h"

void trapinit(void);

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
  vm_test();

  // initialize other kernel subsystems
  port_init();

  panic("All done! (for now...)");
}
