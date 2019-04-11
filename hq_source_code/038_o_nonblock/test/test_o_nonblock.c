#include <stdio.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>
#include <string.h>

#define PATH "/dev/demo0"

pid_t pid;
char buf[64] = {0};

int main(int argc, const char *argv[])
{
	int fd;
	fd = open(PATH,O_RDWR|O_NONBLOCK);
	if(fd < 0){
		perror("open");
	}

	pid = fork();
	if(pid < 0){
		perror("fork");
	}else if(pid == 0){
		if(read(fd,buf,64) < 0){
			perror("read");
		}
		printf("buf:%s\n",buf);

	}else{
		printf("please input:\t");
		fgets(buf,64,stdin);
		buf[strlen(buf) - 1] = '\0';
		if(write(fd,buf,strlen(buf)) < 0){
			perror("write");
		}
	}

	close(fd);
	return 0;
}
