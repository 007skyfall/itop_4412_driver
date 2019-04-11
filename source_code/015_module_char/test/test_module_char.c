#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <sys/types.h>
#include <fcntl.h>
#include <sys/stat.h>


int main(int argc, const char *argv[])
{
	int fd;

	fd = open("/dev/hello", O_RDWR);
	if(fd < 0)
	{
		printf("Failed to open.\n");
		return -1;
	}
	else
	{
		printf("open success.\n");
	}

	getchar();

	close(fd);


	return 0;
}
