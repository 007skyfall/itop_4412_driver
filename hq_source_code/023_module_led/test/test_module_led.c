#include <stdio.h>
#include <sys/types.h>
#include <unistd.h>
#include <sys/stat.h>
#include <fcntl.h>


int main(int argc, const char *argv[])
{
	int fd;

	fd = open("/dev/module_led", O_RDWR);
	if(fd < 0)
	{
		perror("Failed to open.");
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

