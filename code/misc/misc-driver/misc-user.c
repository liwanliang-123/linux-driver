#include <stdio.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>

int main(int argc ,char **argv)
{
    int fd;
    char bufR[32] = {0};
    char bufW[] = "misc_user";

    fd = open("/dev/misc_test",O_RDWR);
    if(fd < 0) {
        perror("open error: ");
        return -1;
    }

    read(fd,bufR,sizeof(bufR));
    printf("user read data is [ %s ]\n",bufR);

    write(fd,bufW,sizeof(bufW));

    close(fd);
    return 0;
}