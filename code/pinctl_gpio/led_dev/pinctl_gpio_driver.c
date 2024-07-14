#include <linux/init.h>
#include <linux/module.h>

#include <linux/of_address.h>
#include <linux/of.h>
#include <linux/platform_device.h>
#include <linux/miscdevice.h>
#include <linux/fs.h>
#include <linux/uaccess.h>

#include <linux/gpio.h>
#include <linux/of_gpio.h>

static int led_test;
static struct device_node *test_node;

static int misc_open (struct inode *inode, struct file *file)
{
     printk(KERN_INFO "misc_open\n");
     return 0;
}

static ssize_t misc_write (struct file *file, const char __user *ubuf, size_t size, loff_t *loff_t)
{
     printk(KERN_INFO "misc_write\n");
     char bufW[2] = {0};

     if (copy_from_user (bufW, ubuf, size) != 0) {
          printk(KERN_INFO "copy_from_user error\n");
          return -1;
     }

     if(bufW[0] == 1) {
          gpio_set_value(led_test, 1);
     } else if(bufW[0] == 0) {
          gpio_set_value(led_test, 0);
     } else{
          printk(KERN_INFO "defult\n");
     }
     return 0;
}

static int misc_release (struct inode *inode, struct file *file)
{
     printk(KERN_INFO "misc_release\n");
     return 0;
}

struct file_operations misc_fops = {
     .owner = THIS_MODULE,
     .open = misc_open,
     .write = misc_write,
     .release = misc_release
};

struct miscdevice misc_dev = {
     .minor = MISC_DYNAMIC_MINOR,
     .name = "pinctl_gpio",
     .fops = &misc_fops
};

int test_probe(struct platform_device *pedv)
{
     printk(KERN_INFO "platform_driver_register test_probe\n");
     /* get device node */
     test_node = of_find_node_by_path("/led1test");
     if(test_node == NULL){
          printk("of_find_node_by_path error\n");
          return -1;
     }
     printk(KERN_INFO "of_find_node is %s\n", test_node->name);

     led_test = of_get_named_gpio(test_node, "gpio-ledtest", 0);
     if(led_test < 0) {
          printk(KERN_INFO "of_get_named_gpio error\n");
          return -1;
     }
     printk(KERN_INFO "of_get_named_gpio led_test is %d\n", led_test);

     int ret;
     ret = gpio_request(led_test, "led_test");
     if(ret < 0){
          printk(KERN_INFO "gpio_request error \n");
          return -1;
     }

     /* 设置GPIO方向为输出 
      * gpio_direction_output 第二个参数表示驱动加载时的默认参数，
      * 如果设置为0，且引脚是低电平有效，加载时LED就会点亮
      */
     gpio_direction_output(led_test, 0);

     ret = misc_register(&misc_dev);
     if(ret < 0)
          return -1;

     return 0;
}

int test_remove(struct platform_device *pedv)
{
     printk(KERN_INFO "platform_driver_register test_remove\n");
     return 0;
}

const struct platform_device_id test_id_table = {
     .name = "test"
};

const struct of_device_id of_match_table_test[] = {
     {.compatible = "led-test"},
     {}
};

struct platform_driver test_driver = {
     .probe = test_probe,
     .remove = test_remove,
     .driver = {
          .owner = THIS_MODULE,
          .name = "test",
          .of_match_table = of_match_table_test
     },
     .id_table = &test_id_table
};

static int __init test_init(void)
{
     int ret;

     ret = platform_driver_register(&test_driver);
     if(ret < 0){
          printk(KERN_INFO "platform_driver_register error\n");
     }
     printk(KERN_INFO "platform_driver_register successed\n");

     return 0;
}

static void __exit test_exit(void)
{
     gpio_free(led_test);
     misc_deregister(&misc_dev);
     platform_driver_unregister(&test_driver);
     printk(KERN_INFO "platform_driver_register exit\n");
}

module_init(test_init);
module_exit(test_exit);
 
MODULE_LICENSE("GPL v2");
