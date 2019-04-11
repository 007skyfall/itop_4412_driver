#include <unistd.h>
#include <stdio.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <string.h>

#define  N  128

int main(int argc, const char *argv[])
{
	int fd;
	char buf[N] = {};
	pid_t pid;

	fd = open("/dev/module_nonblock_io", O_RDWR|O_NONBLOCK);
	if(fd < 0)
	{
		perror("Failed to open.");
		return -1;
	}
	else
	{
		printf("open success.\n");
	}

	if((pid = fork()) < 0)
	{
		perror("Failed to fork.");
		return -1;
	}
	else if(pid == 0)
	{
		if(read(fd, buf, N) < 0)
		{
			perror("Failed to read.");
			return -1;
		}
		printf("buf:%s\n", buf);
	}
	else
	{
		sleep(10);
		write(fd, "This is a waitqueue test.", 25);
		printf("wrote done.\n");
	
	}


	close(fd);

	return 0;
}
