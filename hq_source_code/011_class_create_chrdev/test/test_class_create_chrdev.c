#include <stdio.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>

int main(int argc, const char *argv[])
{
	int fd;
	fd = open("/dev/test_class_create_chrdev" , O_RDWR);
	if(fd < 0)
	{
		perror("open");
	}

	close(fd);
	return 0;
}
