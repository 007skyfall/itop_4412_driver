#include <unistd.h>
#include <stdio.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <string.h>
#include <sys/select.h>

#define  N  128

int main(int argc, const char *argv[])
{
	int fd;
	char buf[N] = {};
	char rbuf[N] = {};

	fd_set  readfds;
	int maxfds;

	fd = open("/dev/module_select_io", O_RDWR|O_NONBLOCK);
	if(fd < 0)
	{
		perror("Failed to open.");
		return -1;
	}
	else
	{
		printf("open success.\n");
	}

	maxfds = fd;

	FD_ZERO(&readfds);
	
	while(1)
	{
		FD_SET(0, &readfds);
		FD_SET(fd, &readfds);


		if(select(maxfds+1, &readfds, NULL, NULL, NULL) < 0)
		{
			perror("Failed to select.");
			return -1;
		}		
		if(FD_ISSET(0, &readfds))
		{
			fgets(buf, N, stdin);
			printf("buf:%s\n", buf);
		}
		if(FD_ISSET(fd, &readfds))
		{
			read(fd,rbuf, N);
			printf("rbuf:%s\n", rbuf);
		}

	}


	close(fd);

	return 0;
}
