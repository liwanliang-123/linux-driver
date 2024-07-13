驱动加载的传递参数和字符驱动主次设备号:

module_param(data, int, S_IRUSR) （传递 char int 类型）
module_param_array(arr,  int, 16, S_IRUSR) （传递数组类型到驱动）
==========================================
root@ELF1:~/driver# insmod misc_parameter.ko data=1000
misc driver enter, data = [1000]
==========================================
root@ELF1:~/driver# insmod misc_parameter.ko arr=1,2,3,4,5
arr[0] = [1] cnt = 5
arr[1] = [2] cnt = 5
arr[2] = [3] cnt = 5
arr[3] = [4] cnt = 5
arr[4] = [5] cnt = 5
=============================================
root@ELF1:~/driver# insmod misc_parameter.ko arr=1,2,3,4,5 data=1010
misc driver enter, data = [1010]
arr[0] = [1] cnt = 5
arr[1] = [2] cnt = 5
arr[2] = [3] cnt = 5
arr[3] = [4] cnt = 5
arr[4] = [5] cnt = 5
root@ELF1:~/driver#
root@ELF1:~/driver# rmmod misc_parameter.ko
misc driver exit, data = [1010]
=============================================
root@ELF1:~/driver# insmod parameter.ko major_num=123 minor_num=4
char driver enter
major_num = [123] ,minor_num = [4]
root@ELF1:~/driver#

root@ELF1:~/driver# insmod parameter.ko
char driver enter
major_num = [0] ,minor_num = [0]
alloc_chardev_region major_num = [246] ,minor_num = [0]
root@ELF1:~/driver#

root@ELF1:~/driver# insmod parameter.ko major_num=123
char driver enter
major_num = [123] ,minor_num = [0]
root@ELF1:~/driver#
root@ELF1:~/driver# cat /proc/devices
Character devices:
......
123 static_chardev
......

==============================================
并且在会在 /sys/module/ 目录下生成一个和驱动名称一样的文件 ，进入/sys/module/misc_parameter/parameters 即可看到
data 变量，可以看到变量的权限和 cat 查看值。
