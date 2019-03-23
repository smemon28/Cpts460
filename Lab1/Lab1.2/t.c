
/*******************************************************
*                  @t.c file                          *
*******************************************************/

typedef unsigned char  u8;
typedef unsigned short u16;
typedef unsigned long  u32;

#define TRK 18
#define CYL 36
#define BLK 1024

#include "ext2.h"
typedef struct ext2_group_desc  GD;
typedef struct ext2_inode       INODE;
typedef struct ext2_dir_entry_2 DIR;

void prints(char *s)
{
	while (*s)
	{
	putc(*s);
	s++;
	}
	return;
}

void gets(char s[])
{

	while((*s = getc() ) != '\r')
	{
		putc(*s);
		s++;

	}
	*s = '\0';
	return;
}

u16 NSEC = 2;
char buf1[BLK], buf2[BLK];

u16 getblk(blk, buf) u16 blk; char *buf;
{
  // readfd( (2*blk)/CYL, ( (2*blk)%CYL)/TRK, ((2*blk)%CYL)%TRK, buf);

    readfd( blk/18, ((blk)%18)/9, ( ((blk)%18)%9)<<1, buf);
    //readfd( blk/18, ((2*blk)%36)/18, ((2*blk)%36)%18, buf);
}

u16 search(INODE *ip, char *name)
{
   int i; char c;
   DIR  *dp;
   char *cp;
   for (i=0; i<12; i++){
       if ((u16)ip->i_block[i] ){
          getblk((u16)ip->i_block[i], buf2);
          dp = (DIR *)buf2;
          cp = buf2;
          while (cp < buf2 + 1024){
              c = dp->name[dp->name_len];  // save last byte

              dp->name[dp->name_len] = 0;
	      prints(dp->name); putc(' ');

              if ( strcmp(dp->name, name) == 0 ){
                 prints("\n\r");
                 return((u16)dp->inode);
              }
              dp->name[dp->name_len] = c; // restore that last byte
              cp += dp->rec_len;
              dp  = (DIR*)cp;
	}
     }
   }
   error();
}

main()
{
  u16    i;
  u16    ino, blk, iblk;
  char   *cp, *name[2], filename[64];
  u32    *up;

  GD    *gp;
  INODE *ip;
  DIR   *dp;

  

  prints("Enter your booter path\n");
  gets(filename);
  

  /* read blk#2 to get group descriptor 0 */
  getblk(2, buf1);
  gp = (GD *)buf1;
  iblk = (u16)gp->bg_inode_table;
//   putc(iblk+'0'); getc();
  getblk(iblk, buf1);  /* read first inode block block */

  ip = (INODE *)buf1 + 1;   // ip->root inode #2


  ino = search(ip, "boot") - 1;
  getblk(iblk+(ino/8), buf1);      /* read block inode of ino number */
  ip = (INODE *)buf1 + (ino % 8);

  ino = search(ip, "mtx") - 1;
  getblk(iblk+(ino/8), buf1);      /* read block inode of ino number */
  ip = (INODE *)buf1 + (ino % 8);

  /* read indirect block into b2 */
  getblk((u16)ip->i_block[12], buf2);

  setes(0x1000);

  for (i=0; i<12; i++){
      getblk((u16)ip->i_block[i], 0);
      putc('*');
      inces();
  }

  if (ip->i_block[12]){
     up = (u32 *)buf2;
     while(*up){
       getblk((u16)*up, 0); putc('.');
        inces();
        up++;
     }
  }
  prints("\n\rReady?");
  getc();
}
