/***************** queue.c file *****************/
char *mystatus[ ] = {"FREE", "READY", "SLEEP", "ZOMBIE"};
int enqueue(PROC **queue, PROC *p) 
{
  PROC *q = *queue;
  if (q == 0 || p->priority > q->priority){
     *queue = p;
     p->next = q;
  }
  else{
     while (q->next && p->priority <= q->next->priority)
            q = q->next;
     p->next = q->next;
     q->next = p;
  }
}
PROC *dequeue(PROC **queue) 
{
     PROC *p = *queue;
     if (p)
        *queue = (*queue)->next;
     return p;
}

void addOrphanChild(PROC *zproc, PROC *p1)
{
   int i=0;
   PROC *childOfZombie= zproc->child;

   if (childOfZombie) {
         
      queueChild(p1, childOfZombie); // add child to Proc 1  
      zproc->child = NULL;  
   }
       printChildList("ChildList of P1 after adding", p1);

}

void queueChild(PROC *p, PROC *child)
{
   if (p->child == NULL)
   {
      p->child = child;
      //p->child->sibling = NULL;
      p->child->parent = p;
   }
   else
   {
      //child exist
      PROC *childExist = p->child;
      while(childExist->sibling != NULL)
      {
         childExist = childExist->sibling;
      }
      childExist->sibling = child;
      childExist->sibling->parent = p;
   }
   
}

int printList(char *name, PROC *p) 
{
  printf("%s = ", name);
  while(p){
     printf("[%d %d]->", p->pid, p->priority);
     p = p->next;
  }
  printf("NULL\n");
}
int printChildList(char*name , PROC *p) 
{
  printf("%s = ", name);
  if (p->child != NULL)
  {
  
   PROC *childExist = p->child;
   while(childExist != NULL)
   {
      printf("[%d %s]->", childExist->pid, mystatus[childExist->status]);
      childExist = childExist->sibling;
   }
  
  }
  printf("NULL\n");
}

int printSleep(char *name, PROC *p) 
{
  printf("%s = ", name);
  while(p){
     printf("[%d event=%d]->", p->pid, p->event);
     p = p->next;
  }
  printf("NULL\n");
}
