Introduction
============
In this assignment, we will be implementing the system calls and scheduler for
SEMOS. Remember, this system is not a UNIX system! Rather, we are implementing
a microkernel system which will allow us to write daemons to implement higher
level functions. We could, if we wanted, write a daemon which would provide Unix
services to our system. 

Because ours is a microkernel, the system calls are rather simpler. They only
provide the fundamental operations that only the kernel can supply. Also,
because message passing is key to this system, IPC is quite fluid. Every process
can write to any open port, which is insecure but easy to manage. This means
that all a daemon need do is claim a specific port and then process messages via
some established protocol. 

The fundamental operations of this system can broken into 3 categories:
  1. Port I/O
  2. Port Management
  3. Process Management

Also note that there are no device-specific calls. Rather, some ports have
special meanings. (Recall that we we opened a few ports in the port lab by
default. These will take on their special meanings in this lab.)

Before we delve into the actual coding of this lab, let's explore what each of
the system calls in these categories are, and what they do. As is the case with
the UNIX system calls, we will document these calls as though they were
C functions because that is the native language of the SEMOS kernel. It does,
after all, use the C calling convention in processing all system calls!


Port I/O
--------
The system calls in this section all have one thing in common. They provide
basic I/O operations on ports. There are two port I/O calls:
  1. `int port_write(int port, const void *buf, int count)` - Write
     count bytes from buf into the port. If the port is full, it waits
     to write the remainder of the data. `port_write` returns
     `count` on success and -1 if the port is closed.
     
  2. `int port_read(int port, void *buf, int count)` - Wait until
     input is available from the port, and then copy at most `count`
     bytes into buf. Returns the number of bytes read on success and
     -1 if the port is closed.


Port Management
---------------
These system calls are responsible for the management of ports. There are two
port management calls:
  1. `int port_acquire(int port)` - Acquire the specified `port`. If
     `-1` is specified, it acquires the lowest numbered available
     port. Returns the number of the port acquired or -1 on failure.
     
  2. `int port_close(int port)` - Closes the specified port. This will
     only succeed if the port is open and owned by the calling
     process. Return 0 on success, -1 on failure.


Process Management
------------------
  1. `int clone()` - Create a clone of the calling process. Return
     0 to the newly created process and the PID of the new process to
     the parent.

  2. `void load_elf(void *bin)` - Replace the calling process with the
     elf image pointed to by `bin`. Note that this function does not
     return on success. If it returns, `bin` must not have been valid!
     
  3. `int getpid()` - Return the PID of the calling process.
     
  4. `int64 getsize()` - Return the size of the calling process's user
     memory.

  5. `int resize(int64 size)` - Resize the calling process to `size`
     bytes. Returns 0 on success, -1 on failure.
     
  6. `int terminate(int pid)` - Terminate the process specified by
     pid. Returns 0 on success, -1 on failure. When a process
     terminates, all of the ports that it owns are closed.

  7. `int status(int pid)` - Return the status of the specified
     process. The status is one of the following constants:
       - `NOT_PRESENT` - The process does not exist
       - `RUNNABLE` - The process is runnable (or running)
       - `WAITING` - The process is waiting.


Procedure
------------
All of the system calls are implemented in the file
`kernel/syscall.c`. Go ahead and have a look in this file. There are
several helper functions which must be implemented in order to create
the system calls. These are:
  - `copyin`
  - `copyout`
  - `findproc`
Go ahead and implement these functions.

Now, try to run the system by typing `make qemu`. You'll notice that
it complains about quite a few missing system calls!  Take a look at
the file `user/init.c` and make sure you understand what a successful
run would look like.

Required System Calls
---------------------
You are required to implement the system calls which are needed to
make user/init.c succeed. Take a look in this file to see the system
calls that are needed. Once you have completed these, you may move on
to the extra credit system calls.

Extra Credit
------------
Implement the remaining system calls and add code to `init.c` to test
your new system calls. You will be graded on how effectively you test
the calls. More difficult system calls will earn more points! The
hardest are `clone` and `load_elf`. Note that for `load_elf`, you may
have to write another program and modify the Makefile in order to
test `load_elf`. Enjoy!
