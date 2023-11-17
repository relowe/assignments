Introduction
============
SEMOS is a system based on a fairly simple microkernel. At the core of
this microkernel is a system of circular buffers called ports. The
system has a statically defined number of these ports, each of which
contains a 1024 byte buffer. These ports are used for two primary
purposes:

  1. Communication with Devices
  2. Communication Between Processes

To understand the first case, we first need to take a look at the
machine which SEMOS is designed to operate. The machine consists of
the computer itself (CPU and RAM), one hard disk, and a serial
terminal. The basic setup is shown in the diagram below:

    +------------+                +-----------+
    |  Computer  |     UART       | SERIAL    |
    |    CPU     | <------------> |  TERMINAL |
    |    RAM     |                |           |
    +------------+                +-----------+
          ^                      / .:::::::. /
          | VIRTIO               ------------
          |
        __V___ 
       | HARD |
       | DISK | 
        ------

Essentially, the kernel controls the machine's RAM, communicates with
the hard disk via the VIRTIO block device interface, and it
communicates with the user via the UART device which is attached to
a serial terminal. Of course, most of these operations are privileged,
and so it must provide an interface to allow user process to use these
devices.

There are three ports which are associated with the serial terminal
and the hard disk. These are defined by the constants in
kernel/port.h:

    /* Predefined port constants */
    #define PORT_CONSOLEIN  0
    #define PORT_CONSOLEOUT 1
    #define PORT_DISKCMD    2

The first port will be populated with the input from the serial
terminal's keyboard as it comes in, and the second port will send
anything written to its buffer to the terminal. The third port will be
used to send commands to the disk.

The user process interface is provided via the port system calls:

    int port_acquire(int port);
    void port_close(int port);
    int port_write(int port, char *buf, int n);
    int port_read(int port, char *buf, int n);

We will see these system calls in a future assignment. For now, we are
going to focus on the task of creating the data structures and
management system for the ports themselves. In this assignment, you
will fill in the code that implements the kernel side of the
conversation with ports.


Getting Started
===============
The first step in this assignment is to download the starter file:

  semos-ports.tar.gz

Then, expand the file by running the following command:

    tar xzf semos-ports.tar.gz

This will create the directory "semos-ports". Change into this
directory and run the operating system by executing the following
commands:

    cd semos-ports
    make qemu

As SEMOS is based in part on xv6, you can also run the debugger as you
could in xv6. As it is now, when you run SEMOS, you will see the
following output

    
    SEMOS kernel is booting

    port init test...FAILED
    port write test...FAILED
    port read test...FAILED
    port acquire/close test...FAILED
    panic: All done! (for now...)

This doesn't bode well! It failed all of its tests! Ok, so of course
it did. We haven't written the port system yet. Press Ctrl-a followed
by x to exit qemu.


Exploring the Source Code
=========================
SEMOS is derived from xv6. Compared to xv6, it is a simplified kernel
which runs on a single cpu. For now, there is no user environment, and
so all of the code is contained in the kernel directory. The files in
this directory are:

  * console.h - Header file containing low-level text i/o interface.
  * entry.S - The entry point of the kernel.
  * kernel.ld - The kernel's linker script.
  * libprecompiled.a - Precompiled files from other assignments.
  * main.c - The main function of the kernel.
  * memlayout.h - Layout of the machine's physical memory.
  * port.c - The implementation of the port system.
  * port.h - Header file of the port system.
  * printf.c - A few functions for formatted output.
  * riscv.h - Wrappers for riscv assembly instructions.
  * start.c - The part of the kernel which calls main.
  * types.h - Defined types for the kernel.

Take a moment and look through each of these files. Understanding how
the kernel as a whole works will help you succeed in your kernel
assignments.

The key parts to explore for this assignment are main.c, port.c, and
port.h. In main.c, we have the kernel's main function:

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

As you can see, the kernel is very incomplete at this stage. It simply
displays a start message, initializes the port system, and then runs
the unit test for the port system. After it does all this, it panics
with the message that it is all done. That is because we do not have
a user space, system calls, or anything else in this version of the
kernel. The entire purpose of the kernel at this stage is to test its
core port system.

In the port.h file, you will see the following struct:

    /* The port type creates a circular buffer for each port */
    struct Port {
        uchar buffer[PORT_BUF_SIZE];// The port's buffered data
        int free;                   // 1 if port is free, 0 if in use
        int head;                   // The index of the first element
        int tail;                   // The index of the first empty slot
        int count;                  // The number of bytes in the buffer
        procid_t owner;             // Owning Process ID (0 for kernel)
    };

This struct defines a port, and the port.c file contains a global
variable which has an array of NPORT numbers of ports. A port is
essentially a circular buffer. For more information about circular
buffers, read the comments at the top of the port.c file.


Implementing the Port System
============================
Your assignment is to complete the port system. After studying the
various source files, you will be ready to begin. In the file port.c,
there are several functions which have a comment which reads:

    // YOUR CODE HERE

These are the sections which you must fill in to complete the
assignment. These are found in the following functions:

  * port_init
  * port_close
  * port_acquire
  * port_write
  * port_read

The comments in and around these functions will tell you how these
are meant to perform, and contain several useful hints for how to
implement these functions. If you like, you are welcome to add static
helper functions to the port.c file, but please do not edit any other
source files in this assignment.

Finally, at the bottom of the port.c file, you will see the port_test
function. This is the unit test that checks that your functions work.
You are not allowed to alter this function, but feel free to read it
as it may give you clues to the expected behavior of the port system.

Once you have successfully completed the assignment, when you run
SEMOS you will see the following output:


    SEMOS kernel is booting

    port init test...PASSED
    port write test...PASSED
    port read test...PASSED
    port acquire/close test...PASSED
    panic: All done! (for now...)


Submitting Your Assignment
==========================
To submit your assignment, run the following command:

    make turnin

This will create the file semos-ports-turnin.tar.gz in the directory
above semos-ports. Upload this tar.gz file to canvas to submit your
assignment.
