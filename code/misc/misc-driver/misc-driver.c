#include <linux/init.h>
#include <linux/module.h>
#include <linux/miscdevice.h>
#include <linux/fs.h>
#include <linux/uaccess.h>

#define MISC_NAME "misc_test"

static int misc_open(struct inode *inode, struct file *file)
{
     printk(KERN_INFO "misc_open\n");
     return 0;
}

static ssize_t misc_read(struct file *file, char __user *ubuf, size_t size, loff_t *loff_t)
{
     char bufR[] = "misc_kernel";
     printk(KERN_INFO "misc_read\n");

     if(copy_to_user(ubuf, bufR, sizeof(bufR)) != 0)
          printk(KERN_INFO "copy_to_user error\n");
     return 0;
}

static ssize_t misc_write(struct file *file, const char __user *ubuf, size_t size, loff_t *loff_t)
{
     char bufW[32] = {0};
     printk(KERN_INFO "misc_write\n");

     if (copy_from_user (bufW, ubuf, size) != 0)
          printk(KERN_INFO "copy_from_user error\n");
     
     printk(KERN_INFO "copy_from_user data is [ %s ]\n", bufW);
     return 0;
}

static int misc_release(struct inode *inode, struct file *file)
{
     printk(KERN_INFO "misc_release\n");
     return 0;
}

struct file_operations misc_fops = {
     .owner = THIS_MODULE,
     .open = misc_open,
     .read = misc_read,
     .write = misc_write,
     .release = misc_release
};

struct miscdevice misc_dev = {
     .minor = MISC_DYNAMIC_MINOR,
     .name = MISC_NAME,
     .fops = &misc_fops
};

static int ret;
static int __init misc_init(void)
{
     ret = misc_register(&misc_dev);
     if(ret < 0)
          printk(KERN_INFO "misc_register error\n");

     printk(KERN_INFO "misc driver enter\n");
     return 0;
}

static void __exit misc_exit(void)
{
     ret = misc_deregister(&misc_dev);
     if(ret < 0)
          printk(KERN_INFO "misc_deregister enter\n");

     printk(KERN_INFO "misc driver exit\n");
}

module_init(misc_init);
module_exit(misc_exit);
 
MODULE_LICENSE("GPL");