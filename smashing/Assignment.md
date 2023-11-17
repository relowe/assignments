## Introduction

In this assignment, you will be working through the famous article
"Smashing the Stack for Fun and Prophet", which first appeared in Phrack
(an underground hacker ezine) in 1996. This article outlines a way to
invade a process's memory by using a buffer overrun attack. This sort of
attack has been used in the wild, in fact this is the basis for how the
infamous
"<a href="https://en.wikipedia.org/wiki/Code_Red_(computer_worm)"
class="external" target="_blank" rel="noopener"><span>Code
Red</span><span class="screenreader-only"> (Links to an external
site.)</span></a>" worm infected microsoft's IIS product back in 2001.

In general, this sort of vulnerability appears when a programmer is not
careful about bounds checking in strings.  This is a concern for all
developers, but especially systems programmers. So today, rather than
trying to write secure systems, we are going to learn how to break them.
That way we can avoid these sorts of problems in our own software.

Fortunately (and unfortunately) the buffer overrun techniques will not
quite work with modern programs. The reason is that compiler authors
have devised schemes that make these sorts of attacks harder to realize.
For example, gcc shadows the stack and then creates a very small
subroutine which checks for stack smashing. This does not make it
impossible to carry out this sort of attack, however, it just means that
modern stack smashing efforts have to be much more sophisticated. Of
course, these modern contrivances work at the expense of increased
memory size and more processor cycles being devoted to function calls. 
In this modern age, both resources are plentiful. So this is a fair
trade.

## Using the Virtual Machine

So then, how are you to try out the 90's hacker techniques in 2020?
Simple!  I have supplied you with a virtual machine which has a redhat
5.1 installation.  This is an early linux distribution, and it was
released in 1998. The compilers on this distro will not add this
additional code, and so all of the attack vectors in the article will
work. Just for fun, I should note that this virtual linux machine is
quite similar to a machine I had when I was an undergraduate.  It has a
400MB hard drive, and 128MB of RAM. Unlike the machine I had back in
those days, QEMU will likely run it as though it had a 1GHz processor.
(my real rig had a 266MHz processor!)

To run the virtual machine, you will untar the archive. This will
require around 250mb of disk space.  Change into the directory, and then
run the script using this command:

    ./rh5.sh

This will execute the virtual machine.  The machine is running in curses
mode, which means that it will work a bit differently. To get into the
monitor, press "ALT+2".  Pressing "ALT+1" will get you back into the
system.  Once the machine is running, you will be presented with a
prompt to log in to the text console. The login information is:

    login: marcy
    password: playground

Redhat 5 will be somewhat familiar territory. It has all the usual linux
programs, though with much earlier versions. It will be a tad less
userfriendly than modern machines. Most notably, the only editors
available on the system are vim, jed, and ed.  (I recommend vim.)  Also,
your cursor may not return to you when leaving some programs. Should
this happen, simply run the shell commands "clear" and "reset" to fix
your terminal.

When you are done working, you must shut the machine down. This is
especially important because this thing has a 90's era ext2 file system
which does not have any sort of crash recovery!

To do this, you will logout, and then login as root:

login: root  
password: 12345

Then execute the command:

    halt

Once you see the message which says 

    System halted

Switch to the qemu monitor (ALT+2) and type:

    quit

 

## The Assignment

1.  Download the archive of the virtual machine: smashing.tar.gz

    Alternatively, you can do this assignment on replit by forking the
    following repl: 

    <a href="https://replit.com/@pngwen/Smashing-the-Stack-for-Fun-and-Profit" class="inline_disabled" target="_blank" rel="noopener" title="Link">https://replit.com/@pngwen/Smashing-the-Stack-for-Fun-and-Profit</a> 

2.  Untar the archive and make sure you can run the virtual machine.

3.  Read through the article smashing.txt

    and get all of the code working through exploit3.c.

4.  Create a pdf document with answers to the following questions:
    1.  Provide screenshots which prove you managed to get exploit3.c to
        work.
    2.  Define the stack. What are the two main operations performed on
        the stack?
    3.  What is the difference between the stack and stack pointer?
    4.  What are the purposes of the stack pointer and base pointer
        registers?
    5.  What instructions are part of the function prolog and epilog,
        and what do they do?
    6.  Can a programmer identify the offset by knowing where the
        beginning of the stack lives? Explain your answer.
    7.  What did the author propose as a work around to increase the
        chances of guessing the beginning address of the vulnerable
        stack?
    8.  Give examples of at least 5 functions that are included in the
        standard C library which do not perform memory checks against
        buffer overflows. What should we do to ensure application safety
        when using these functions?
    9.  For the final questions, try to get exploit3.c to work on a
        modern system, such as PPG (not inside the RH5 machine.)
        1.  What happens when you try to attack the program under a
            modern compiler?
        2.  After taking a look at the stack in GDB, what do you think
            is causing the issue, and how could you overcome this
            limitation?
        3.  **Extra Credit**: Successfully attack the program using code
            generated by a modern compiler. Explain how you achieved
            this, and post screenshots to prove that you made it work.
5.  Submit your pdf to canvas
6.  HACK THE PLANET!

NOTE: Xterm is not installed on the redhat image, so you can skip the
exercises which use xterm.
