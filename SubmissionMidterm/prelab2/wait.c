

int tswitch();

int ksleep(int event)
{
  printf("proc %d going to sleep on event=%d\n", running->pid, event);

  running->event = event;
  running->status = SLEEP;
  enqueue(&sleepList, running);
  printList("sleepList", sleepList);
  tswitch();
}

int kwakeup(int event)
{
  PROC *temp, *p;
  temp = 0;
  printList("sleepList", sleepList);

  while (p = dequeue(&sleepList))
  {
    if (p->event == event)
    {
      printf("wakeup %d\n", p->pid);
      p->status = READY;
      enqueue(&readyQueue, p);
    }
    else
    {
      enqueue(&temp, p);
    }
  }
  sleepList = temp;
  printList("sleepList", sleepList);
}

int kexit(int exitValue)
{
  printf("proc %d in kexit(), value=%d\n", running->pid, exitValue);
  PROC *p;
  //check for the children for running and then
  //if it has children teh  wakeup process 1 and pass your children to P1 and wake up parent
  // if it doesnt then simply wake up your parent

  if (running->child)
  {
    addOrphanChild(running, &proc[1]);
    kwakeup(&proc[1]);
  }

  running->exitCode = exitValue;
  running->status = ZOMBIE;
  
  printChildList("ChildList of P1 after adding", &proc[1]);

  kwakeup(running->parent);

  tswitch();
}
int kwait(int *status)
{
  if (!running->child)
  {
    return -1;
  }

  PROC *childExist = running->child;
  PROC *prev = NULL;
  int i, found = 0;
  while (1)
  {
    //found = 0;
    if (childExist->status == ZOMBIE)
    {
      printf("childExist->status: %d", childExist->status);
      found = 1;
    }
    else
    {
      while (childExist->sibling != NULL)
      {
        childExist = childExist->sibling;
        prev = childExist;
        if (childExist->status == ZOMBIE)
        {
          found = 1;
          break;
        }
      }
    }

    //if zombie child found
    if(found && prev)
    { 
      
      prev->sibling = childExist->sibling;
      childExist->sibling = NULL;
    }
    if(found && prev == NULL)
    {
      running->child = childExist->sibling;
      childExist->sibling = NULL;
    }
    if (found)
    {
      int zChildPid = childExist->pid;
      /* lay the dead child to rest */
      *status = childExist->exitCode;
      childExist->status = FREE; /* free its PROC */
      enqueue(&freeList, childExist);
      return (zChildPid); /* return its pid */
    }
    ksleep(running); //sleep on it proc address
  }
}
