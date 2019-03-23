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
  printf("room=%d data=%d\n", p->room, p->data);
  printf("----------------------------------------\n");

  printf("nwriter=%d nreader=%d buf=\n", p->nwriter, p->nreader);

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

  // p->nreader = 1;
  // p->nwriter = 1;
}

int read_pipe(PIPE *p, char *buf, int n)
{


  int ret;
  char c;
  if (n <= 0)
    {
      p->nreader--;
      kwakeup(&p->room);
      kexit(running->pid);
      return 0;
    }

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
    if (ret)
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

  while (n)
  {
    printf("writer %d writing pipe\n", running->pid);
    // printf("nreader %d \n", p->nreader);

    if(!p->nreader)
    {
      printf("proc %d broken pipe", running->pid);
      p->nwriter--;
      kexit(running->pid);
      return -1;

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
      kwakeup(&p->data);
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
    show_pipe();
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
