/*======================================================================
    A test program to access /dev/second
    This example is to help understand async IO 
    
    The initial developer of the original code is Baohua Song
    <author@linuxdriver.cn>. All Rights Reserved.
======================================================================*/
#include <sys/types.h>
#include <sys/stat.h>
#include <stdio.h>
#include <fcntl.h>
#include <signal.h>
#include <string.h>
#include <unistd.h>

int fd;
char buf[64];

/*���յ��첽���źź�Ķ���*/
void input_handler(int signum)
{
  printf("receive a signal, signalnum:%d\n",signum);
  bzero(buf, 64);
  read(fd, buf, 64);
  printf("read from device : %s\n", buf);
}

int main()
{
  int oflags;
  fd = open("/dev/demo0", O_RDWR);
  if (fd < 0)
  {
	  perror("fai to open");
	  return -1;
  }
  //�����ź���������
  signal(SIGIO, input_handler); //��input_handler()����SIGIO�ź�
  fcntl(fd, F_SETOWN, getpid());
  oflags = fcntl(fd, F_GETFL);
  fcntl(fd, F_SETFL, oflags | FASYNC);
  while(1)
  {
	  sleep(100);
  }

  return 0;
}
