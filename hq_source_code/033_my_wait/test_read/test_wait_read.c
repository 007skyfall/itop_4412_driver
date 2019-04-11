#include <sys/types.h>
#include <unistd.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <stdio.h>

int main(int argc, const char *argv[])
{
	int fd;
	char read_buf[64] = {0};
	fd = open("/dev/my_wait0" , O_RDWR);
	if(fd < 0){
		perror("open");
	}
	
	if(read(fd,read_buf,64) < 0){
		perror("read");
	}
	printf("read_buf:%s\n",read_buf);

	close(fd);
	return 0;
}
