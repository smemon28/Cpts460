/*********** type.h file ************/
#define NPROC    9          // number of PROCs
#define SSIZE 1024          // stack size = 4KB
#define BLOCK    1
// PROC status
#define FREE     0          
#define READY    1
#define SLEEP    2
#define ZOMBIE   3
char *status[ ] = {"FREE", "READY", "SLEEP", "ZOMBIE"};

typedef struct proc{
    struct proc *next;      // next proc pointer       
    int  *ksp;              // saved sp: at byte offset 4 

    int   pid;              // process ID
    int   ppid;             // parent process pid 
    int   status;           // PROC status=FREE|READY, etc. 
    int   priority;         // scheduling priority 

    int   event;            // event value to sleep on
    int   exitCode;         // exit value

    struct proc *child;     // first child PROC pointer       
    struct proc *sibling;   // sibling PROC pointer  
    struct proc *parent;    // parent PROC pointer       

    int   kstack[1024];     // process stack                 
}PROC;
