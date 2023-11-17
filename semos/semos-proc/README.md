Introduction
============
In this assignment, you will be implementing the proc subsystem which will
enable the kernel to load a user init program. There will be no system
calls for now, and so the user processes cannot really do much.

Remember that we do not have any notion of a filesystem in the SEMOS 
kernel, and so things will be a little bit different than they are in
xv6. We will be loading our init elf object from a binary string 
embedded as a BLOB in the kernel image. The program that will be included
is the init.c program in the user directory.

Go ahead and take a look at init.c. Note that it makes several system calls
using the function provided in syscall.S. After that, it enters a busy loop.
The reason for the busy loop is that there is no way for the process to request
termination, and rather than allow it to pagefault we allow it to run forever.


Procedure
=========
Go ahead and run
    make qemu
As expected, this does not work at all! You will need to add code to complete
the following functions:
  - `proc_init`
  - `proc_load_user_init`
  - `proc_alloc`
  - `proc_free`
  - `proc_load_elf`
  - `proc_resize`
  - `proc_pagetable` 
  - `proc_free_pagetable`
  - `proc_loadseg`

All of these functions can be found in the file `proc.c`. As always, these 
functions have analogous functions in xv6. The xv6 analogs can be found in:
  - `xv6-riscv/kernel/proc.c`
  - `xv6-riscv/kernel/exec.c`
  - `xv6-riscv/kernel/vm.c`

Be sure to read the function comments for hints about how the SEMOS functions
differ from the xv6 functions. You will also want to take a look at the 
contents of `proc.h`, `trap.c`, and `main.c` to get a good handle on how
all of these functions work.

When you successfully complete this assignment, you will see the following
output in response to running `make qemu`:

    
    SEMOS kernel is booting
    
    System Call: 1
    System Call: 2
    System Call: 42


Submitting Your Assignment
==========================
To submit your assignment, run the following command:

    make turnin

This will create the file semos-proc-turnin.tar.gz in the directory
above semos-ports. Upload this tar.gz file to canvas to submit your
assignment.
