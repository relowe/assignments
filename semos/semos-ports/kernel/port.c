//    What is a CIRCULAR BUFFER?
//    A circular buffer, cyclic buffer or ring buffer is a data structure that
//    uses a single, fixed-size buffer as if it were connected end-to-end.
//    This means that whenever data arrives into the buffer, the buffer
//    actually remembers the order in which data arrived. This will retrieve
//    you with the oldest      data first. So the first data that enters is the
//    first data that leaves the buffer(FIFO).
//
//       - HEAD (H) and TAIL(T) are the locations in the buffer, where data is
//         being deposited and retrieved respectively
//
//       - A circular buffer first starts out empty and has a set length.
//
//    Let's consider below a five-element buffer as an array. In the array, you
//    have variables which keep track of a head and a tail, and they use the
//    indices of the   array as the variable they store.
//
// Step 1:
//       - First, both HEAD and TAIL will be on the same cell.
//
//        H T
//     |------------ |------------|------------|-----------|-----------|
//     |             |            |            |           |           |
//     |-------------|------------|------------|-----------|-----------|
//      HEAD (H) = 0
//      TAIL (T) = 0
//
//    When we insert data into buffer, first thing the function has to do is
//    check if there is any room available in the buffer.
//
//       - One way to do this is, you can take the difference between head and
//         tail, a tail and head depending on which one's larger and compare
//         that to how many spaces  are in the buffer.
//
//       - Other way, (optimal) is to use another variable called COUNT(C),
//         when the buffer is empty, we say COUNT equals to 0 (i.e, COUNT=0).
//         Now the function that we    called, comes in and checks if the COUNT
//         is less than five(which is the size of the buffer),goes ahead and
//         accepts data. Let's continue with this way.
// Step 2:
//    Let's say, we bring in number 5 (number 5 is written to the array) and
//    then several things get updated.
//
//       - H (head)has to get moved up one and in reality the way it actually
//         happens is the index of where head is is incremented to one 
//         (i.e, HEAD = 1)
//
//       - Also, as a peice of data is stored in our array, the COUNT is also
//         incremented to one (i.e, COUNT=1)
//
//        T            H
//    |------------ |------------|------------|-----------|-----------|
//    |     5       |            |            |           |           |
//    |-------------|------------|------------|-----------|-----------|
//      HEAD (H) = 1
//      TAIL (T) = 0
//      COUNT (C) = 1
//
// Step 3:
//    Now that we have a basic understanding of how data comes in, let's speed
//    the process up.  
//
//    Let's say another 3 bytes of data comes in. Ex 7,2,12.
//
//       - So everytime, when each one of these data comes in, the head gets
//         incremented by one and count gets incremented by one.
//
//       - Since three more pieces of data came in, both head and count go by
//         three.
//
//          T                                                   H
//    |------------ |------------|------------|-----------|-----------|
//    |     5       |      7     |     2      |     12    |           |
//    |-------------|------------|------------|-----------|-----------|
//       HEAD (H) = 4
//       TAIL (T) = 0
//       COUNT (C) = 4
//
// Step 4:
//    Now things become little tricky. Because we're usig arrays, arrays always
//    start at index 0 which makes things just a little more to work with.
//
//    Let's say our next byte of data comes in Ex 3
//
//       - As, size of our array is five, we have to wrap it back around and
//         now head gets back to zero. As arrays incremented from four to five,
//         COUNT will be five.
//
//         H T
//    |------------ |------------|------------|-----------|-----------|
//    |     5       |      7     |     2      |     12    |     3     |
//    |-------------|------------|------------|-----------|-----------|
//          ^                                                    |
//          |----------------------------------------------------|
//       HEAD (H) = 0
//       TAIL (T) = 0
//       COUNT (C) = 5
//
// Step 5:
//    Next time when data writes into the buffer, the COUNT lets the function
//    know that the array(buffer) is full. Here, we can decide how the buffer
//    should behave.(Override or stop writing)
//
//       - One method when a data is read out of the buffer is, it first checks
//         whether the COUNT is greater than zero, if yes, it lets you return
//         that data.
//
//       - So we store the 5, then we move the TAIL from zero to one and we
//         decrement the COUNT.
//
//          H              T
//    |------------ |------------|------------|-----------|-----------|
//    |     5       |      7     |     2      |     12    |     3     |
//    |-------------|------------|------------|-----------|-----------|
//          ^                                                    |
//          |----------------------------------------------------|
//    HEAD (H) = 0
//    TAIL (T) = 1
//    COUNT (C) = 5
//
//  Note: Once the tail gets all the way to the end, we have to follow the same
//    kind of modulo increment. i.e, if the tail gets to 5 in our case, the
//    function knows to wrap the tail back around. Once the tail and count are
//    incremented, we take the value 5 that was stored previously and go ahead
//    and return at which exit out of the function.
//
//    Also, there is one more function apart from the functions that put data
//    into the buffer and data out of the buffer, that is 'peak'. It is very
//    similar to the get data out of the buffer but the difference being is
//    that peek retrieves the data out of the buffer without effectively
//    erasing it. In other words, without incrementing  the tail  and
//    decrementing the count.


#include "types.h"
#include "riscv.h"
#include "console.h"
#include "port.h"

// The global collection of ports
struct Port ports[NPORT];


// Initialize the ports
void 
port_init()
{
    // Initialize the ports list.  Upon initialization, the following should be
    // true:
    //    - The Predefined ports (see port.h) should all be owned by the
    //      kernel.
    //    - All other ports should be marked as free.
    //    - All ports should have their start and end set to indicate an empty
    //      buffer
    
    // Loop through 0 to NPORT-1, initialize status of kernal ports and
    // non-kernal ports. Make sure that all ports are empty.

    // YOUR CODE HERE
}


// Close the port.
void 
port_close(int port)
{
    // Close the port.  If the port is not open, nothing will happen.  However,
    // if it is open, we empty its contents and mark it as free.

    // YOUR CODE HERE
}



// Acquire Port.  If the specified port is negative, allocate the next available port.
int 
port_acquire(int port, procid_t proc_id)
{
    // If the port number is -1, allocate the next free port.
    // If the port number is not -1, check to see if the port is available.
    //   If the port is not available, return -1 
    // Mark the port as allocated, set the owner of the port, and
    // then return the port number allocated.
    // 
    // If this operation fails, return -1.

    // YOUR CODE HERE
    
    return -1;
}


// Write up to n characters from buf to a port.  Return the number of bytes written.
int 
port_write(int port, char *buf, int n)
{
    // If the port is not open, return -1
    // Write, at most, n bytes to the buffer.  If the buffer fills
    // up before n bytes, stop writing. Return the actual number of bytes
    // you have written. Be sure to update the count field as you
    // write it.

    // YOUR CODE HERE
    return 0;
}


// Read up to n characters from a port into buf. Return the number of bytes read.
int 
port_read(int port, char *buf, int n)
{
    // If the port is not open, return -1.
    // Read at most n bytes from the port. If the port contents are
    // exhausted before you complete the read, stop reading.
    // Return the actual number of bytes you have read.
    // Be sure to update count as you read.

    // YOUR CODE HERE
    return 0;
}


///////////////////////////////////////////////////////////////////////////////
// Unit Tests below this line. Do not change any part of the
// following!
///////////////////////////////////////////////////////////////////////////////

// Run unit tests on the port system (for debugging)
void 
port_test()
{
    int i;
    int passed;
    char buf[10];

    // test to make sure the ports correctly initialize
    printf("port init test...");
    passed=1;
    for(i=0; passed && i < PORT_DISKCMD; i++) {
        if(ports[i].free || ports[i].owner != 0) passed=0;
    }
    for(i=PORT_DISKCMD+1; passed && i<NPORT; i++) {
        if(!ports[i].free) passed=0;
    }
    for(i=0; passed && i<NPORT; i++) {
        if(ports[i].count != 0 || ports[i].head != 0 || ports[i].tail !=0) 
            passed = 1;
    }
    print_pass(passed);


    // test writing
    printf("port write test...");
    passed=1;
    if(port_write(PORT_CONSOLEIN, "abc", 3) != 3) passed = 0;
    if(port_write(15, "abc", 3) != -1) passed = 0;
    for(i=0; passed && i<PORT_BUF_SIZE; i++) {
        if(port_write(PORT_CONSOLEOUT, "a", 1) != 1) {
            passed=0;        
        }
    }
    if(port_write(PORT_CONSOLEOUT, "a", 1) != 0) passed = 0;
    print_pass(passed);


    // test reading
    printf("port read test...");
    passed = 1;
    if(port_read(PORT_CONSOLEIN, buf, 3) != 3) passed = 0;
    if(buf[0] != 'a' || buf[1] != 'b' || buf[2] != 'c') passed=0;
    if(port_read(PORT_CONSOLEIN, buf, 3) != 0) passed = 0;
    if(port_read(15, "abc", 3) != -1) passed = 0;
    print_pass(passed);

    // test acquire/close
    printf("port acquire/close test...");
    passed=1;
    if(port_acquire(-1, 42) != 3) passed = 0;
    if(ports[3].owner!=42 || ports[3].free) passed = 0;
    if(port_acquire(PORT_CONSOLEOUT, 0) != -1) passed = 0;
    if(port_acquire(255, 15) != 255) passed = 0;
    if(ports[255].owner!=15 || ports[255].free) passed = 0;
    port_close(3);
    if(!ports[3].free || ports[3].owner != 0 || ports[3].head != 0 ||
       ports[3].count != 0) passed = 0;
    print_pass(passed);
    
}
