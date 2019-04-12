int fork()
{
	int i;
	char *PA, *CA;
	PROC *p = getproc();
	if (p==0)
		return -1;
	p->ppid = running->pid;
	p->parent = running;
	p->status = READY;
	p->priority = 1;
	p->sibling = running->child;
	running->child = p;
    // build p's pgtable 
    p->pgdir = (int *)(0x600000 + (p->pid - 1)*0x4000);
    int * ptable = p->pgdir;
    // initialize pgtable
    for (i=0; i<4096; i++)
        ptable[i] = 0;
    int pentry = 0x412;
    for (i=0; i<258; i++){
        ptable[i] = pentry;
    	pentry += 0x100000;
  	}
	ptable[2048] = 0x800000 + (p->pid - 1)*0x100000|0xC32;
	PA = (char*)((unsigned int)running->pgdir[2048] & 0xFFFF0000);
	CA = (char*)((unsigned int)p->pgdir[2048] & 0xFFFF0000);
	memcpy(CA, PA, 0x100000);
	for(i = 1; i <= 14; i++)
		p->kstack[SSIZE - i] = running->kstack[SSIZE - i];
	p->kstack[SSIZE - 14] = 0;
	p->kstack[SSIZE - 15] = (int)goUmode;
	p->ksp = &(p->kstack[SSIZE - 28]);
	p->usp = running->usp;
	p->cpsr = running->cpsr;
	enqueue(&readyQueue, p);
	return p->pid;
}

int exec(char * cmdline)
{
	int i, upa, usp;
	char * cp, kline[128], file[32], filename[32];
	PROC * p = running;
	strcpy(kline, cmdline);
	cp = kline;
	i = 0;
	while(*cp != ' ')
	{
		filename[i] = *cp;
		i++;
		cp++;
	}
	filename[i] = 0;
	file[0] = 0;
	upa = (int*)(p->pgdir[2048] & 0xFFFF0000);
	if(!load(filename, p))
		return -1;
	usp = upa + 0x100000 - 128;
	strcpy((char *)usp, kline);
	p->usp = (int*)VA(0x100000 - 128);
	for(i = 2; i < 14; i++)
		p->kstack[SSIZE - i] = 0;
	p->kstack[SSIZE - 1] = (int)VA(0);
	return (int)p->usp;
}
