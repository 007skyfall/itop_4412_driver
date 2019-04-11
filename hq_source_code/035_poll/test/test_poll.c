#include <stdio.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>
#include <string.h>
#include <poll.h>
#include <string.h>

#define PATH "/dev/demo0"
char buf[64] = {0};

int main(int argc, const char *argv[])
{
	int fd,ret;
	fd = open(PATH,O_RDWR);
	if(fd < 0){
		perror("open");
	}
	struct pollfd pfd[] = {
		[0] = {
			.fd = fd,
			.events = POLLIN,
		},
		[1] = {
			.fd = 0,
			.events = POLLOUT,
		}
	};

	while(1){
		ret = poll(pfd,2,5000);
		if(ret > 0)	{
			if(pfd[0].revents & POLLIN){
				if(read(fd,buf,64) < 0){
					perror("read");
				}
				printf("read buf :%s\n",buf);
				bzero(buf,64);
			}

			if(pfd[1].revents & POLLOUT){
				printf("please input :\t");
				fgets(buf,64,stdin);
				buf[strlen(buf) - 1] = '\0';

				if(write(fd,buf,64) < 0){
					perror("write");
				}
			}
		}else{
			return -1;
		}
	}

	close(fd);
	return 0;
}
