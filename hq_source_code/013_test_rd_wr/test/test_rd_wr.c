#include <stdio.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <errno.h>
 #include <unistd.h>

#define PATH "/dev/test_rd_wr"

int main(int argc, const char *argv[])
{
	int fd ;
	char use_buf_r[64] = {0};
	char use_buf_w[64] = {"i am from user..."};
	fd = open(PATH,O_RDWR);
	if(fd < 0)
	{
		perror("open");
	}
	if(write(fd , use_buf_w,sizeof(use_buf_w)) < 0)
	{
		perror("write");
	}

	if(read(fd,use_buf_r,64) < 0)
	{
		perror("read");
	}
	printf("use_buf_r:%s\n",use_buf_r);
	close(fd);
return 0;
}
