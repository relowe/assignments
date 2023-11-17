#ifndef SEMOS_PORT_H
#define SEMOS_PORT_H

/* The number of ports */
#define NPORT 256

/* size of the port buffer */
#define PORT_BUF_SIZE 1024

/* The port type creates a circular buffer for each port */
struct Port {
    uchar buffer[PORT_BUF_SIZE];// The port's buffered data
    int free;                   // 1 if port is free, 0 if in use
    int head;                   // The index of the first element
    int tail;                   // The index of the first empty slot
    int count;                  // The number of bytes in the buffer
    procid_t owner;             // Owning Process ID (0 for kernel)
};

extern struct Port ports[];


/* Predefined port constants */
#define PORT_CONSOLEIN  0
#define PORT_CONSOLEOUT 1
#define PORT_DISKCMD    2


// Initialize the ports
void port_init();

// Acquire Port.  If the specified port is negative, allocate the next available port.
int port_acquire(int port, procid_t proc_id);

// Close the port. 
void port_close(int port);

// Write up to n characters from buf to a port.  Return the number of bytes written.
int port_write(int port, char *buf, int n);

// Read up to n characters from a port into buf. Return the number of bytes read.
int port_read(int port, char *buf, int n);

// Run unit tests on the port system (for debugging)
void port_test();
#endif
