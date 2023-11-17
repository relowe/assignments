#include "userlib/userlib.h"

void readline(char *buf, int size);

int main()
{
    char buf[100];
    int port, size;

    printf("Hello, my pid is  %d\n", getpid());
    size = getsize();
    printf("    process size: %d\n", size);
    resize(size+1024);
    printf("    added 1024, new size: %d\n", getsize());

    // Test the port system
    port = port_acquire(-1);
    printf("Acquired Port: %d\n", port);
    port_write(port, "ports work", 10);
    size = port_read(port, buf, 100);
    if(size != -1) {
        buf[size]='\0';
        printf("Port Result: %s\n", buf); 
    } else {
        printf("Could not read\n");
    }
    printf("Closing port 0: %d\n", port_close(0));
    printf("Closing acquired port: %d\n", port_close(port));
    printf("Reading from closed port...");
    if(port_read(port, buf, 100) == -1) {
        printf("PASSED\n");
    } else {
        printf("FAILED\n");
    }

    terminate(getpid());
    printf("ERROR: Still Running!\n");
    
    while(1) {
    }
    return 0;
}
