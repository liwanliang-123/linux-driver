#include <stdio.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>
#include <stdlib.h>

int main(int argc ,char **argv)
{
    int fd;
    char bufW[2] = {0};
    fd = open("/dev/pinctl_gpio",O_RDWR);
    if(fd < 0)
        perror("open error: \n");

    bufW[0] = atoi(argv[1]);

    write(fd,bufW,sizeof(bufW));

    close(fd);
    return 0;
}
