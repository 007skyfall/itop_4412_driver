#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <sys/types.h>
#include <fcntl.h>
#include <sys/stat.h>
#include <string.h>

#define  N  128

int main(int argc, const char *argv[])
{
	int fd;
	char buf[N] = "This is a write test.";
	char rbuf[N] = {};
	int ret = 0;

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
	
	ret = write(fd, buf, strlen(buf));
	printf("wrote done,%d bytes.\n", ret);

	strcpy(buf, "++++++++++");
	ret = write(fd, buf, strlen(buf));
	printf("wrote done,%d bytes.\n", ret);


	getchar();

	ret = read(fd, rbuf, 10);
	printf("read %d bytes -->%s\n", ret, rbuf);
	getchar();
	bzero(rbuf, sizeof(rbuf));

	ret = read(fd, rbuf, 5);
	printf("read %d bytes -->%s\n", ret, rbuf);
	getchar();
	bzero(rbuf, sizeof(rbuf));

	ret = read(fd, rbuf, N);
	printf("read %d bytes -->%s\n", ret, rbuf);

	close(fd);


	return 0;
}
