#include <sys/types.h>
#include <unistd.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <stdlib.h>
#include <stdio.h>


int main(int argc, const char *argv[])
{
	int fd;
	char write_buf[64] = "i am from user\n";
	fd = open("/dev/my_wait0" , O_RDWR);
	if(fd < 0){
		perror("open");
	}

	if(write(fd,write_buf,sizeof(write_buf)) <  0 ){
		perror("write");
	}

	close(fd);
	return 0;
}
