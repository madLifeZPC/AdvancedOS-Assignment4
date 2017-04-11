#include <stdio.h>
#include <linux/ioctl.h>
#include <stdlib.h>
#include <fcntl.h>
#include <unistd.h>
#include <string.h>

#define BUFSIZE 5242880
#define MAXSIZE 4194304

int main(int argc, char *argv[]) {

	char *buf = (char *)malloc(sizeof(char)*BUFSIZE);
	int i=0,fd,result;
	while(i<(MAXSIZE-1)){
		buf[i] = 'a';
		i++;
	}
	while(i<(BUFSIZE-1)){
		buf[i] = 'b';
		i++;
	}
	buf[i] = '\0';

	fd = open("/dev/fourmb_device",O_RDWR);
	if(fd < 0) {
		perror("open");
		return -1;
	}
	result = write(fd, buf, strlen(buf));
	printf("Wrote %d bytes\n", result);
}
