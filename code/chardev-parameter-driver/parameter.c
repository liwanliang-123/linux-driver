#include <linux/init.h>
#include <linux/module.h>

#include <linux/fs.h>
#include <linux/kdev_t.h>

#define DEVIVE_NUMBER 1    //次设备号的个数
#define DEVIVE_MINOR_NUMBER 0
#define DEVICE_SNAME "static_chardev"
#define DEVICE_ANAME "a_chardev"

static int major_num,minor_num;

module_param(major_num, int, S_IRUSR);
module_param(minor_num, int, S_IRUSR);
// module_param_array(name, type, nump, perm);   

static int __init char_init(void)
{
     printk(KERN_INFO "char driver enter\n");
     printk(KERN_INFO "major_num = [%d] ,minor_num = [%d]\n",major_num, minor_num);

     dev_t dev_num;
     //dev_t:保存主次设备号
     int ret;

     if(major_num){
          dev_num = MKDEV(major_num,minor_num);
          //静态分配设备号
          ret = register_chrdev_region(dev_num,DEVIVE_NUMBER,DEVICE_SNAME);
          if(ret < 0)
               printk(KERN_INFO "register_chrdev_region error\n");
     }else{
          //动态分配设备号
          ret = alloc_chrdev_region(&dev_num, DEVIVE_MINOR_NUMBER, DEVIVE_NUMBER, DEVICE_ANAME);
          if(ret < 0)
               printk(KERN_INFO "register_chrdev_region error\n");

          major_num = MAJOR(dev_num);
          minor_num = MINOR(dev_num);
          printk(KERN_INFO "alloc_chardev_region major_num = [%d] ,minor_num = [%d]\n",major_num, minor_num);
     }

     return 0;
}

static void __exit char_exit(void)
{
     printk(KERN_INFO "char driver exit,\n");
     unregister_chrdev_region(MKDEV(major_num,minor_num), DEVIVE_NUMBER);
}

module_init(char_init);
module_exit(char_exit);

MODULE_LICENSE("GPL v2");
