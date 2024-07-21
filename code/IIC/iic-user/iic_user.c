#include <stdio.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>

#include <linux/i2c.h>
#include <linux/i2c-dev.h>
#include <sys/ioctl.h>

/*
 *  通过应用层代码读取硬件I2c的数据
 */
int fd;

int i2c_read_data(unsigned int slave_addr, unsigned char reg_addr)
{
    int ret;
    unsigned char data;
    struct i2c_rdwr_ioctl_data i2c_read;

    struct i2c_msg msg[2] = {
        [0] = {
            .addr = slave_addr,
            .flags = 0,
            .buf = &reg_addr,
            .len = sizeof(reg_addr)
        },
        [1] = {
            .addr = slave_addr,
            .flags = 1,
            .buf = &data,
            .len = sizeof(data)
        },
    };

    i2c_read.msgs = msg;
    i2c_read.nmsgs = 2;

    ret = ioctl(fd, I2C_RDWR, &i2c_read);
    if(ret < 0){
        printf("ioctl error\n");
        perror("why:");
        return ret;
    }

    return data;
}

int main(int argc ,char **argv)
{
    int TD_STATUS;
    fd = open("/dev/i2c-0",O_RDWR);
    if(fd < 0) {
        perror("open error: ");
        return -1;
    }

    while(1) {
        TD_STATUS = i2c_read_data(0x38, 0x00);
        printf("TD_STATUS data is %d\n", TD_STATUS);
        sleep(1);
    }

    close(fd);
    return 0;
}
