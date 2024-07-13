用cdev_init和cdev_add注册完字符设备后并不会自动生成设备节点，
需要使用 mknod 命令创建一个设备节点。

格式：mknod  [name] [type] [major] [minor]
eg:  mknod  /dev/chardev c 246 0

=================================================
root@ELF1:~/driver# ./chardev_test
open error:
: No such file or directory
root@ELF1:~/driver#
root@ELF1:~/driver#
root@ELF1:~/driver# mknod  /dev/chardev c 246 0
root@ELF1:~/driver#
root@ELF1:~/driver# ls /dev/chardev
/dev/chardev
root@ELF1:~/driver#
root@ELF1:~/driver# ./chardev_test
chardev_open
root@ELF1:~/driver#
