#include "userlib.h"

int 
port_write(int port, char *buf, int count)
{
    return syscall(SYS_PORT_WRITE, port, buf, count);
}


int 
port_read(int port, char *buf, int count)
{
    return syscall(SYS_PORT_READ, port, buf, count);
}


int 
port_acquire(int port)
{
    return syscall(SYS_PORT_ACQUIRE, port);
}


int 
port_close(int port)
{
    return syscall(SYS_PORT_CLOSE, port);
}


int 
clone()
{
    return syscall(SYS_CLONE);
}


int 
load_elf(void *buf, int size)
{
    return syscall(SYS_LOAD_ELF, buf, size);
}


int 
getpid()
{
    return syscall(SYS_GETPID);
}


int 
getsize()
{
    return syscall(SYS_GETSIZE);
}


int 
resize(int size)
{
    return syscall(SYS_RESIZE, size);
}


int 
terminate(int pid)
{
    return syscall(SYS_TERMINATE, pid);
}


int 
status(int pid)
{
    return syscall(SYS_STATUS, pid);
}
