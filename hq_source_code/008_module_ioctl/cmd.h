#ifndef __CMD_HEAD_H
#define __CMD_HEAD_H

struct node {
	int val;
	char buf[128];
};

#define  CMD   _IO('c', 0)
#define  CMDR  _IOR('c', 1, struct node)
#define  CMDW  _IOW('c', 2, struct node)


#endif
