#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <stdio.h>
#include <unistd.h>
#include <fcntl.h>
#include <signal.h>

int fd;
char read_buf[64] = {0};
void sighandler_read(int data)
{
	if(read(fd,read_buf,64) < 0){
		perror("read");
	}
	printf("read_buf:%s\n",read_buf);
}

int main(int argc, const char *argv[])
{
	int flags ;
	fd = open("/dev/my_fasync0" , O_RDWR);
	if(fd < 0){
		perror("open");
	}
	fcntl(fd,F_SETOWN,getpid());

	flags = fcntl(fd,F_GETFL);
	flags |= FASYNC;
	fcntl(fd,F_SETFL,flags);

	signal(SIGIO,sighandler_read);
	while(1);
	close(fd);
	return 0;
}
