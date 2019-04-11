#ifndef __MYIOCTL_H
#define __MYIOCTL_H

#include <asm-generic/ioctl.h>

#define TYPE 'A'

#define ONE _IO(TYPE,1)
#define TWO _IO(TYPE,2)



#endif 


