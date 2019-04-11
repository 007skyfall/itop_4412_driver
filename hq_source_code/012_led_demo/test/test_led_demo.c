#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <stdio.h>
#include <unistd.h>

int main(int argc, const char *argv[])
{
	int fd;
	fd = open("/dev/led_demo" , O_RDWR);
	if(fd < 0)
	{
		perror("open");
	}

	sleep(20);

	close(fd);
return 0;
}
