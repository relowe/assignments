// Definitions of system call numbers
#define SYS_PORT_WRITE      0
#define SYS_PORT_READ       1
#define SYS_PORT_ACQUIRE    2
#define SYS_PORT_CLOSE      3
#define SYS_CLONE           4
#define SYS_LOAD_ELF        5
#define SYS_GETPID          6
#define SYS_GETSIZE         7
#define SYS_RESIZE          8
#define SYS_TERMINATE       9
#define SYS_STATUS          10


// Process Status Constants
#define NOT_PRESENT 0
#define RUNNABLE    1
#define WAITING     2
