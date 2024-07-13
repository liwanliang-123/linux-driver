#include <stdio.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>
#include <stdlib.h>

int main(int argc ,char **argv)
{
    int fd;
    char bufW[8] = {0};
    if (argc != 2){
        printf("eg: ./test 1/0 \n");
        return -1;
    }
    fd = open("/dev/misc_beep",O_RDWR);
    if(fd < 0)
        perror("open error: \n");

    bufW[0] = atoi(argv[1]);

    write(fd,bufW,sizeof(bufW));

    close(fd);
    return 0;
}