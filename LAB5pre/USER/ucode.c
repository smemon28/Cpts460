typedef unsigned char   u8;
typedef unsigned short  u16;
typedef unsigned int    u32;

#include "string.c"
#include "uio.c"




int ubody(char *name)
{
  int pid, ppid;
  char line[64];
  u32 mode,  *up;

  mode = getcsr();
  mode = mode & 0x1F;
  printf("CPU mode=%x\n", mode);
  pid = getpid();
  ppid = getppid();

  while(1){
    printf("This is process #%d in Umode at %x parent=%d\n", pid, getPA(),ppid);
    printf("Child list: ");
    //printChildList(getpid());
    umenu();
    printf("input a command : ");
    ugetline(line); 
    uprintf("\n"); 
 
    if (strcmp(line, "getpid")==0)
       ugetpid();
    if (strcmp(line, "getppid")==0)
       ugetppid();
    if (strcmp(line, "ps")==0) 
       ups();
    if (strcmp(line, "chname")==0)
       uchname();
if (strcmp(line, "wait")==0)
       uwait();
if (strcmp(line, "wakeup")==0)
       uwakeup();
if (strcmp(line, "sleep")==0)
       usleep();
    if (strcmp(line, "switch")==0)
       uswitch();
       if (strcmp(line, "exit")==0)
       uexit();
        if (strcmp(line, "fork")==0)
       do_kfork();
  }
}

int umenu()
{
  uprintf("-------------------------------\n");
  uprintf("getpid getppid ps chname switch wait sleep wakeup kfork \n");
  uprintf("-------------------------------\n");
}

int getpid()
{
  int pid;
  pid = syscall(0,0,0,0);
  return pid;
}    


int do_kfork()
{
   int child = syscall(15,0,0,0);
   if (child < 0)
      printf("kfork failed\n");
   else{
      printf("proc %d kforked a child = %d\n", ugetpid(), child); 
   }
   return child;
}

int ukfork()
{ 
  return syscall(15,0,0,0);
}

int getppid()
{ 
  return syscall(1,0,0,0);
}

int ugetpid()
{
  int pid = getpid();
  uprintf("pid = %d\n", pid);
}

int ugetppid()
{
  int ppid = getppid();
  uprintf("ppid = %d\n", ppid);
}

int ups()
{
  
  return syscall(2,0,0,0);
}

int uchname()
{
  char s[32];
  uprintf("input a name string : ");
  ugetline(s);
  printf("\n");
  return syscall(3,s,0,0);
}

int usleep()
{
  int event;
  uprintf("enter an event to sleep on: ");
  event = geti();
  return syscall(14,event,0,0);
}
int uwait()
{
  return syscall(12,0,0,0);
}

int uexit()
{
  int exitval;
  uprintf("enter an exit value: ");
  exitval = geti();
  return syscall(11,exitval,0,0);
}
int uwakeup()
{
  int event;
  uprintf("enter an event to wakeup on: ");
  event = geti();
  return syscall(13,event,0,0);
}
int uswitch()
{
  return syscall(4,0,0,0);
}


int ugetc()
{
  return syscall(90,0,0,0);
}

int uputc(char c)
{
  return syscall(91,c,0,0);
}

int getPA()
{
  return syscall(92,0,0,0);
}
