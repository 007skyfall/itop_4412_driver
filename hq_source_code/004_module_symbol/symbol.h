#ifndef __SYMBOL_HEAD_H
#define __SYMBOL_HEAD_H

struct object {
	int val;
	int (*func)(void);
};


#endif
