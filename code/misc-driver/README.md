misc driver:

杂项设备属于字符设备的一种，可以通过 cat /proc/misc  即可查看

特点： 除了比字符设备驱动代码简单外，还有主设备号是相同的，次设备号不同（主设备号均为10）

主设备号都相同，就被视为同一设备，（都是杂项设备）

头文件：
/home/lwl/elfboard/source_code/linux-4.1.15-elf1/include/linux/miscdevice.h

struct miscdevice  {
	int minor;    //次设备号
	const char *name;  //设备节点名字
	const struct file_operations *fops;  //文件操作
	struct list_head list;
	struct device *parent;
	struct device *this_device;
	const struct attribute_group **groups;
	const char *nodename;
	umode_t mode;
};

注册杂项&注销：
extern int misc_register(struct miscdevice *misc);
extern int misc_deregister(struct miscdevice *misc);


