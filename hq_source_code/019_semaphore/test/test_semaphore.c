#include <stdio.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>

#define PATH "/dev/test_semaphore_demo"

int main(int argc, const char *argv[])
{
	int fd;
	fd = open(PATH,O_RDWR);
	if(fd < 0)
	{
		perror("open");
	}

	sleep(20);

	close(fd);
return 0;
}
