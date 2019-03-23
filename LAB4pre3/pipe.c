#define NPIPE 10
#define PSIZE 8

typedef struct pipe
{
  char buf[8];
  int head, tail;
  int data, room;
  int nreader;
  int nwriter;
} PIPE;

PIPE pipe;

int show_pipe()
{
  PIPE *p = &pipe;
  int i;
  printf("----------------------------------------\n");
  printf("room=%d data=%d buf=", p->room, p->data);
  for (i = 0; i < p->data; i++)
    putchar(p->buf[p->tail + i]);
  printf("\n");
  printf("----------------------------------------\n");
}
int kpipe()
{
  int i;
  PIPE *p = &pipe;
  p->head = p->tail = 0;
  p->data = 0;
  p->room = PSIZE;

  p->nreader = 1;
  p->nwriter = 1;
}

int read_pipe(PIPE *p, char *buf, int n)
{
//   (2). P1: enter a line to write  OR  a NULL line to exit;
//          e.g. 12345 ==> should only write 5 chars to pipe
//      P1: enter a NULL line ==> P1 should exit

// (3). P2: enter bytes to read  OR  0 to exit;
//          e.g. nytes = 10  ==> should read 5 chars

//      P2: nbytes = 20      ==> should return 0 because no data and no writer

// Run the program again:

// (4). P1: try to write 1234567890 to pipe => write 8 chars and SLEEP

// (5). P2: try to read 20 bytes ==> should return with 8 chars read
//      P2: read 0 byte          ==> P2 shoud exit switch to P1

// (6). P1: must see a BROKEN PIPE condition and exit

  int ret;
  char c;
  if (n <= 0)
    {
      p->nreader--;
      kexit(running->pid);
      return 0;
    }
  // if (n <= 0)
  //   return 0;
  show_pipe();

  while (n)
  {
    printf("reader %d reading pipe\n", running->pid);
    ret = 0;
    while (p->data)
    {
      *buf = p->buf[p->tail++];
      p->tail %= PSIZE;
      buf++;
      ret++;
      p->data--;
      p->room++;
      n--;
      if (n <= 0)
        break;
    }
    show_pipe();
    if (ret || n==0)
    { /* has read something */
      kwakeup(&p->room);
      return ret;
    }
    // pipe has no data
    if(p->nwriter)
   { 
    printf("reader %d sleep for data\n", running->pid);
    kwakeup(&p->room);
    ksleep(&p->data);
    continue;
    }
    //pipe has no writer no data
    return 0;
  }
}

int write_pipe(PIPE *p, char *buf, int n)
{
  char c;
  int ret = 0;
  show_pipe();
  // if(n <=0)
  //   return 0;
  while (n)
  {
    printf("writer %d writing pipe\n", running->pid);

    if(!p->nreader)
    {
      printf("proc %d broken pipe", running->pid);
      kexit(running->pid);

    }
    while (p->room && n)
    {
      p->buf[p->head++] = *buf;
      p->head %= PSIZE;
      buf++;
      ret++;
      p->data++;
      p->room--;
      n--;
      if (n == 0)
      {
        break;
      }
    }
    show_pipe();
    printf("writer %d sleep for room\n", running->pid);
    //wakeup all readers
    kwakeup(&p->data);
    	//wrote n bytes
    if(n==0)
    {
      show_pipe();
      printf("pipe after writing\n");
      // kwakeup(&p->data);
      return ret;
    }
    //still has data to write but pipe has no room
		printf("pipe before writer goes to sleep\n");
    show_pipe(p);
    ksleep(&p->room);
  }
  return ret;
}

int pipe_reader()
{
  char line[128];
  int nbytes, n;
  PIPE *p = &pipe;
  printf("proc %d as pipe reader\n", running->pid);
  while (1)
  {
    printf("input nbytes to read : ");
    scanf("%d", &nbytes);
    getchar();
     if (nbytes <= 0)
    {
      p->nreader--;
      kexit(running->pid);
      tswitch();
      return 0;
    }
    n = read_pipe(p, line, nbytes);
    line[n] = 0;
    printf("Read n=%d bytes : line=%s\n", n, line);
  }
}

int pipe_writer()
{
  char line[128];
  int nbytes, n;
  PIPE *p = &pipe;
  printf("proc %d as pipe writer\n", running->pid);
  while (1)
  {
    printf("input a string to write : ");

    fgets(line, 128, stdin);
    line[strlen(line) - 1] = 0;

    if (strcmp(line, "") == 0) //if nothing is entered
    {
      kexit(running->pid);
      return 0;
    }

    nbytes = strlen(line);
    printf("nbytes=%d buf=%s\n", nbytes, line);
    n = write_pipe(p, line, nbytes);
    printf("wrote n=%d bytes\n", n);
  }
}
