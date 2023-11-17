#include "types.h"
#include "riscv.h"
#include "console.h"
#include "port.h"

// start() jumps here in supervisor mode
void
main()
{
  // initialize uart
  uartinit();
  printf("\n");
  printf("SEMOS kernel is booting\n");
  printf("\n");

  // initialize other kernel subsystems
  port_init();

  // test the port system
  port_test();

  panic("All done! (for now...)");
}
