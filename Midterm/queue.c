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

char *mystatus[ ] = {"FREE", "READY", "SLEEP", "ZOMBIE"};
int enqueue(PROC **queue, PROC *p)
{
  PROC *q  = *queue;
  if (q==0){
    *queue = p;
    p->next = 0;
    return;
  }
  if ((*queue)->priority < p->priority){
    p->next = *queue;
    *queue = p;
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
  printf("sleepList   = ");
   while(p){
     kprintf("[%devent=%d]->", p->pid,p->event);
     p = p->next;
  }
  kprintf("NULL\n"); 
}

int printList(char *name, PROC *p)
{
  kprintf("%s = ", name);
   while(p){
     kprintf("[%d%d]->", p->pid, p->priority);
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
int addChild(PROC *kid)
{
	PROC *hold = running->child;
	if(hold)
	{
		while(hold->sibling)
		{
			hold = hold->sibling;
		}
		hold->sibling = kid;
		kid->parent = running;
		return 1;
	}
	running->child = kid;
	kid->parent = running;
	return 1;
}

void queueChild(PROC *p, PROC *child)
{
   if (p->child)
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





