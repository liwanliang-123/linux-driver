1、在Linux中怎么自动创建一个设备节点？
一般使用 mdev 来实现设备节点文件的自动创建和删除。

2、什么是 mdev ?
mdev 是 udev 的简化版本，是busybox中所带的程序。

3、什么是 udev ?
udev 是一种工具，它能够根据系统中的硬件设备的状态更新设备文件，包括设备文件的创建、删除等。设备文件一般放在 /dev 目录下，在使用 udev 之后在  /dev 目录就只包含系统中真正存在的设备。

4、怎么自动创建设备节点？
（1）、使用 class_create 函数创建一个 class 的类。  （在/sys/class查看）

（2）、使用 device_create 函数在创建的类下面创建一个设备。

5、创建和销毁函数
class:
    class_create 
    class_destroy
device:
    device_create
    device_destroy