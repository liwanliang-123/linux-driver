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
#include <linux/of_irq.h>

#include <linux/interrupt.h>

#include <linux/wait.h>
#include <linux/sched.h>

static int irq;
static int key_test;
static struct device_node *key_node;
static int value = 0;

static int wq_flags = 0; //定义一个wq的等待条件
DECLARE_WAIT_QUEUE_HEAD(key_wq); //初始化一个wq

static int misc_open (struct inode *inode, struct file *file)
{
     return 0;
}

static ssize_t misc_read (struct file *file, char __user *ubuf, size_t size, loff_t *loff_t)
{
     //只有当 wq_flags 为真时才会唤醒wq继续往下执行，否则休眠
     //等待 wq_flags 变为真
     wait_event_interruptible(key_wq, wq_flags);

     if(copy_to_user(ubuf, &value, sizeof(value)) != 0)
          printk(KERN_INFO "copy_to_user error\n");
     
     wq_flags = 0;
     return 0;
}

static int misc_release (struct inode *inode, struct file *file)
{
     return 0;
}

struct file_operations misc_fops = {
     .owner = THIS_MODULE,
     .open = misc_open,
     .read = misc_read,
     .release = misc_release
};

struct miscdevice misc_dev = {
     .minor = MISC_DYNAMIC_MINOR,
     .name = "wq_test",
     .fops = &misc_fops
};

irq_handler_t test_key(int irq, void*args)
{
     value = !value;

     //先将 wq_flags 设置为真
     wq_flags = 1;

     //然后唤醒 key_wq
     // wake_up(&key_wq);
     wake_up_interruptible(&key_wq);

     return IRQ_HANDLED;  
}

int test_probe(struct platform_device *pedv)
{
     printk(KERN_INFO "platform_driver_register test_probe\n");
     /* get device node */
     key_node = of_find_node_by_path("/test_key");
     if(key_node == NULL){
          printk("of_find_node_by_path error\n");
          return -1;
     }
     printk(KERN_INFO "of_find_node is %s\n", key_node->name);

     key_test = of_get_named_gpio(key_node, "gpios", 0);
     if(key_test < 0) {
          printk(KERN_INFO "of_get_named_gpio error\n");
          return -1;
     }
     printk(KERN_INFO "of_get_named_gpio key_test is %d\n", key_test);

     //设置GPIO的方向为输入
     gpio_direction_input(key_test);

     /* 获取GPIO中断的中断号 ------(1)*/
     irq = irq_of_parse_and_map(key_node, 0);
     printk(KERN_INFO "irq_of_parse_and_map interrupt number is %d \n", irq);

     /* 申请中断 */
     int ret;
     ret = request_irq(irq, test_key, IRQF_TRIGGER_RISING, "test_key", NULL);
     if(ret < 0)
          printk(KERN_INFO " request_irq error \n");

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
     {.compatible = "test_key"},
     {}
};

struct platform_driver test_driver = {
     .probe = test_probe,
     .remove = test_remove,
     .driver = {
          .owner = THIS_MODULE,
          .name = "key",
          .of_match_table = of_match_table_test
     },
     .id_table = &test_id_table
};

static int __init test_init(void)
{
     int ret;
     misc_register(&misc_dev);
     ret = platform_driver_register(&test_driver);
     if(ret < 0){
          printk(KERN_INFO "platform_driver_register error\n");
     }
     printk(KERN_INFO "platform_driver_register successed\n");

     return 0;
}

static void __exit test_exit(void)
{
     free_irq(irq, NULL);
     misc_deregister(&misc_dev);
     platform_driver_unregister(&test_driver);
     printk(KERN_INFO "misc driver exit\n");
     printk(KERN_INFO "platform_driver_register exit\n");
}

module_init(test_init);
module_exit(test_exit);
 
MODULE_LICENSE("GPL v2");



























