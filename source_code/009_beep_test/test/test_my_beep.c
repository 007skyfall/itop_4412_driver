#include <sys/types.h>
#include <stdio.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>
#include <errno.h>

int main(int argc, const char *argv[])
{
	int fd;
	fd = open("/dev/beep" , O_RDWR);
	if(fd < 0)
	{
		perror("open");
	}
	sleep(3);

	return 0;
}
