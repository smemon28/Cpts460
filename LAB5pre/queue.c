/********************************************************************
Copyright 2010-2017 K.C. Wang, <kwang@eecs.wsu.edu>
This program is free software: you can redistribute it and/or modify
it under the terms of the GNU General Public License as published by
the Free Software Foundation, either version 3 of the License, or
(at your option) any later version.

This program is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
GNU General Public License for more details.

You should have received a copy of the GNU General Public License
along with this program.  If not, see <http://www.gnu.org/licenses/>.
********************************************************************/

// queue.c file


extern PROC *freeList;
char *mystatus[ ] = {"FREE", "READY", "SLEEP", "ZOMBIE"};
PROC *getproc()
{
  PROC *p = freeList;
  if (p){
    freeList = p->next;
  }
  return p;
}

int putproc(PROC *p)
{
  p->next = freeList;
  freeList = p;
}

int enqueue(PROC **queue, PROC *p)
{
  PROC *q  = *queue;
  if (q==0 || p->priority > q->priority){
    *queue = p;
    p->next = q;
    return;
  }
  while (q->next && p->priority <= q->next->priority){
    q = q->next;
  }
  p->next = q->next;
  q->next = p;
}

PROC *dequeue(PROC **queue)
{
  PROC *p = *queue;
  if (p)
    *queue = p->next;
  return p;
}

void queueChild(PROC *p, PROC *child)
{
   if (!p->child)
   {
      p->child = child;
      //p->child->sibling = NULL;
      p->child->parent = p;
   }
   else
   {
      //child exist
      PROC *childExist = p->child;
      while(childExist->sibling)
      {
         childExist = childExist->sibling;
      }
      childExist->sibling = child;
      childExist->sibling->parent = p;
   }
   
}

int printQ(PROC *p)
{
  kprintf("readyQueue = ");
  while(p){
    kprintf("[%d%d]->", p->pid,p->priority);
    p = p->next;
  }
  kprintf("NULL\n");
}

int printSleepList(PROC *p)
{
  printf("sleepList  = ");
   while(p){
     kprintf("[%d%d]->", p->pid,p->event);
     p = p->next;
  }
  kprintf("NULL\n"); 
}

int printList(PROC *p)
{
   kprintf("freeList   = ");
   while(p){
     kprintf("[%d]->", p->pid);
     p = p->next;
  }
  kprintf("NULL\n"); 
}
int printChildList(char*name , PROC *p) 
{
  printf("%s = ", name);
  if (p->child)
  {
  
   PROC *childExist = p->child;
   while(childExist)
   {
      printf("[%d %s]->", childExist->pid, mystatus[childExist->status]);
      childExist = childExist->sibling;
   }
  
  }
  printf("NULL\n");
}


int printanyList(char *name, PROC *p)
{
  kprintf("%s = ", name);
   while(p){
     kprintf("[%d%d]->", p->pid, p->priority);
     p = p->next;
  }
  kprintf("NULL\n"); 
}

void addOrphanChild(PROC *zproc, PROC *p1)
{
   int i=0;
   PROC *childOfZombie= zproc->child;

   if (childOfZombie) {
         
      queueChild(p1, childOfZombie); // add child to Proc 1  
      zproc->child = 0;  
   }
       printChildList("ChildList of P1 after adding", p1);

}

