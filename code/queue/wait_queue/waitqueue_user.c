#include <stdio.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>
#include <stdlib.h>

int main(int argc ,char **argv)
{
    int fd;
    int value;
    fd = open("/dev/wq_test",O_RDWR);
    if(fd < 0)
        perror("open error: \n");

    while(1) {
        //一直读取内核中的 value值
        read(fd,&value,sizeof(value));
	    printf("value is %d\n", value);
    }
    close(fd);
    return 0;
}

