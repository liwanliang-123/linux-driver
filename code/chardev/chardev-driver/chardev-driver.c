#include <linux/init.h>
#include <linux/module.h>

#include <linux/fs.h>
#include <linux/kdev_t.h>
#include <linux/cdev.h>

#define DEVIVE_NUMBER 1
#define DEVIVE_MINOR_NUMBER 0
#define DEVICE_ANAME "chardev"

static dev_t dev_num;
static int major_num,minor_num;

struct cdev cdev;

static int chardev_open (struct inode *inode, struct file *file)
{
     printk(KERN_INFO "chardev_open\n");
     return 0;
}

struct file_operations chardev_ops = {
     .owner = THIS_MODULE,
     .open = chardev_open
};

static int __init char_init(void)
{
     printk(KERN_INFO "char driver enter\n");
     int ret;

     ret = alloc_chrdev_region(&dev_num, DEVIVE_MINOR_NUMBER, DEVIVE_NUMBER, DEVICE_ANAME);
     if(ret < 0)
          printk(KERN_INFO "register_chrdev_region error\n");

     major_num = MAJOR(dev_num);
     minor_num = MINOR(dev_num);
     printk(KERN_INFO "alloc_chardev_region major_num = [%d] ,minor_num = [%d]\n",major_num, minor_num);

     cdev.owner = THIS_MODULE;

     //将file_ops写到cdev->ops
     cdev_init(&cdev, &chardev_ops);
     //cdev_add: 将字符设备cdev注册到内核
     cdev_add(&cdev, dev_num, DEVIVE_NUMBER);

     return 0;
}

static void __exit char_exit(void)
{
     printk(KERN_INFO "char driver exit,\n");

     unregister_chrdev_region(MKDEV(major_num,minor_num), DEVIVE_NUMBER);
     cdev_del(&cdev);
}

module_init(char_init);
module_exit(char_exit);

MODULE_LICENSE("GPL v2");
