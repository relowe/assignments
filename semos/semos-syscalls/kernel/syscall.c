#include "types.h"
#include "memlayout.h"
#include "riscv.h"
#include "console.h"
#include "trap.h"
#include "proc.h"
#include "port.h"
#include "mem.h"
#include "scheduler.h"
#include "syscall.h"
#include "string.h"

#define NELEM(x) (sizeof(x)/sizeof((x)[0]))

//////////////////////////////////////////////////////////////////////
// Static Helper Function prototypes
//////////////////////////////////////////////////////////////////////
static uint64 sys_port_write();
static uint64 sys_port_read();
static uint64 sys_port_acquire();
static uint64 sys_port_close();
static uint64 sys_clone();
static uint64 sys_load_elf();
static uint64 sys_getpid();
static uint64 sys_getsize();
static uint64 sys_resize();
static uint64 sys_terminate();
static uint64 sys_status();

static int copyin(pagetable_t pagetable, char *dst, uint64 srcva, uint64 len);
static int copyout(pagetable_t pagetable, uint64 dstva, char *src, uint64 len);
static struct proc* findproc(int pid);

//////////////////////////////////////////////////////////////////////
// Static Variables 
//////////////////////////////////////////////////////////////////////

// An array of function pointers to syscalls
static uint64 (*syscalls[])(void) = {
    [SYS_PORT_WRITE]    sys_port_write,
    [SYS_PORT_READ]     sys_port_read,
    [SYS_PORT_ACQUIRE]  sys_port_acquire,
    [SYS_PORT_CLOSE]    sys_port_close,
    [SYS_CLONE]         sys_clone,
    [SYS_LOAD_ELF]      sys_load_elf,
    [SYS_GETPID]        sys_getpid,
    [SYS_GETSIZE]       sys_getsize,
    [SYS_RESIZE]        sys_resize,
    [SYS_TERMINATE]     sys_terminate,
    [SYS_STATUS]        sys_status
};


//////////////////////////////////////////////////////////////////////
// API Functions
//////////////////////////////////////////////////////////////////////
void
syscall(void)
{
  int num;
  struct proc *p = cpu.proc;

  num = p->trapframe->a0;
  if(num >= 0 && num < NELEM(syscalls) && syscalls[num]) {
    p->trapframe->a0 = syscalls[num]();
  } else {
    printf("%d: unknown sys call %d\n",
            p->pid, num);
    p->trapframe->a0 = -1;
  }

  console_dump();
}


void 
console_dump()
{
    char c;

    while(ports[PORT_CONSOLEOUT].count) {
        port_read(PORT_CONSOLEOUT, &c, 1);
        uartputc_sync(c);
    }
}



//////////////////////////////////////////////////////////////////////
// Static Helper Functions
//////////////////////////////////////////////////////////////////////


/*
 * Implementation of the write system call. 
 *      port_write(int port, void *buf, int count)
 */
static uint64 
sys_port_write()
{
    // Use the port_write function to write to a port. If the port is
    // closed, return -1. Copy up to the count number of bytes,
    // or until the port is full (whichever occurs first.) 
    //
    // Return the length of characters copied.
    // HINT: This is very similar to the Xv6 call, write.
    //       The copyin function will be helpful here.
    // YOUR CODE HERE
    printf("port_write not implemented\n");
    return -1;
}


/*
 * Implementation of the read system call. 
 *      port_read(int port, void *buf, int count)
 */
static uint64 
sys_port_read()
{
    // Use the port_read function read from a port. If the port
    // is closed, return -1. Otherwise, wait until there is at least
    // one character to be read. If the port is empty, yield to the
    // scheduler. Keep doing that until the port is no longer empty.
    // Read at most count bytes, or until you have read the last byte
    // from the port. Return the number of characters you have read.
    // HINT: This is very similar to the Xv6 call, read.
    //       The copyout function will be helpful here.
    // YOUR CODE HERE
    printf("port_read not implemented\n");
    return -1;
}


/*
 * Implementation of the port_acquire system call.
 *     int port_acquire(int port)
 */
static uint64 
sys_port_acquire()
{
    // This function should attempt to acquire a port. It is 
    // basically a wrapper for the kernel function port_acquire.
    // All you need to do is extract the parameter, call
    // port_acquire, and then retrun its result. Note that this
    // function should mark ownership of the port.
    // YOUR CODE HERE
    printf("port_acquire not implemented\n");
    return -1;
}


/*
 * Implementation of the port_close system call.
 *     int port_close(int port)
 */
static uint64 
sys_port_close()
{
    // This function should close a port by calling the kernel
    // function port_close. Before doing this, however, it should 
    // checked to make sure the current process owns the port. If
    // the current process is not the owner, this function should
    // return -1 and not call port_close.
    // YOUR CODE HERE
    printf("port_close not implemented\n");
    return -1;
}


/*
 * Implementation of the clone system call
 *     int clone()
 */
static uint64 
sys_clone()
{
    // This function should do the following:
    //     1.) Allocate a new process.
    //     2.) Duplicate the current process's memory in the new
    //         process's page table, copying memory as you do so.
    //     3.) Return 0 to the child and the pid to the parent.
    // Note: This is very similar to the "fork" system call in xv6,
    // however it does not duplicate file descriptors or ports. It
    // only copies the memory and returns.
    // YOUR CODE HERE
    printf("clone not implemented\n");
    return -1;
}


/*
 * Implement the load_elf system call
 *     int load_elf(void *buf, int size)
 */
static uint64 
sys_load_elf()
{
    // This system call should do the following:
    //     1.) Copy size bytes from buf into the kernel.
    //     2.) Deallocate the current process's memory.
    //     3.) Use the kernel's proc_load_elf to load up copied binary
    //       string.
    // This function should not return on success, it returns -1 on
    // error. 
    // This is quite similar to the exec system call in xv6, however
    // it does not have the filesystem interactions.
    // YOUR CODE HERE
    printf("load_elf not implemented\n");
    return -1;
}


/*
 * Implement the getpid system call
 *     int getpid()
 */
static uint64 
sys_getpid()
{
    // This system call simply returns the PID of the current process 
    // YOUR CODE HERE
    printf("getpid not implemented\n");
    return -1;
}


/*
 * Implement the getsize system call
 *     int getsize()
 */
static uint64 
sys_getsize()
{
    // This system call simply returns the size of the current process
    // YOUR CODE HERE
    printf("getsize not implemented\n");
    return -1;
}


/*
 * Implementation of the resize system call
 *     int resize(int size)
 */
static uint64 
sys_resize()
{
    // This system call will resize the process to the given size.
    // This is just a wrapper for the proc_resize method. You should
    // extract the parameters and call this fucntion returning what it
    // returns.
    // YOUR CODE HERE
    printf("resize not implemented\n");
    return -1;
}


/*
 * Implementation of the terminate system call
 *     int terminate(int pid)
 */
static uint64 
sys_terminate()
{
    // This function terminates the specified process. This is similar
    // to the kill system call in xv6, though it is a bit more abrupt
    // as we don't have any notion of waiting or zombies within the
    // SEMOS kernel. This system call should do the following:
    //     1.) Find the process. If it is not present, return -1
    //     2.) Deallocate the specified process's memory.
    //     3.) Mark the process as free, so it will not be run again.
    //     4.) If the calling process is the process which has been
    //       terminated, call yield. Otherwise, return 0
    // YOUR CODE HERE
    printf("terminate not implemented\n");
    return -1;
}


/*
 * Implementation of the status system call
 *     int status(int pid)
 */ 
static uint64 
sys_status()
{
    // This system call will attempt to return the status of the
    // specified process. The procedure is as follows:
    //     1.) Find the process. If it is not present, return -1
    //     2.) If the process was found, return its status.
    // YOUR CODE HERE
    printf("status not implemented\n");
    return -1;
}




// Copy from user to kernel.
// Copy len bytes to dst from virtual address srcva in a given page table.
// Return 0 on success, -1 on error.
static int 
copyin(pagetable_t pagetable, char *dst, uint64 srcva, uint64 len)
{
  // This function exists within Xv6 and can be almost directly
  // adapted. You just need to make sure that you adapt it for our
  // virtual memory functions.
  // YOUR CODE HERE
  return -1;
}


// Copy from kernel to user.
// Copy len bytes from src to virtual address dstva in a given page table.
// Return 0 on success, -1 on error.
static int 
copyout(pagetable_t pagetable, uint64 dstva, char *src, uint64 len)
{
  // This function exists within Xv6 and can be almost directly
  // adapted. You just need to make sure that you adapt it for our
  // virtual memory functions.
  // YOUR CODE HERE
  return -1;
}



// Find the process with the given pid and return a pointer to it.
// If the process is not found, return 0
static struct proc* 
findproc(int pid)
{
    // Simply search the proc array, looking for the specified pid.
    // YOUR CODE HERE
    return 0;
}
