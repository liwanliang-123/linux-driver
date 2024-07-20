#include <stdio.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>

#include <linux/input.h>

int main(int argc ,char **argv)
{
    int fd;
    struct input_event test_event;

    fd = open("/dev/input/event2",O_RDWR);
    if(fd < 0) {
        perror("open error: ");
        return -1;
    }

    while(1) {
    	read(fd,&test_event,sizeof(test_event));
	    if(test_event.type == EV_KEY) {
    		printf("read type is [ %#x ]\n",test_event.type);
            printf("read value is [ %#x ]\n",test_event.value);
            printf("read code is [ %#x ]\n",test_event.code);
        }
    }

    close(fd);
    return 0;
}
