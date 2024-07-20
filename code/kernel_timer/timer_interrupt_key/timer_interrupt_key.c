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
#include <linux/timer.h>

static int irq;
static int key_test;
static struct device_node *key_node;

static void timer_function(unsigned long data);

DEFINE_TIMER(test_timer, timer_function, 0, 0);

/* 
 * 使用 flag 的作用是为了防止按一次按键但有可能是按了
 * 两次，而进入了两次中断服务函数，出现重复设置定时器的错误 
 */
static int flag;

static void timer_function(unsigned long data)
{
     printk(KERN_INFO "%s,line: %d\n", __func__, __LINE__);
     flag = 0;
     // if(GPIO == 0){
     //      //在这里判断GPIO的状态
     // }
}

irq_handler_t test_key(int irq, void*args)
{
     printk(KERN_INFO "callback %s , %d \n",__func__, __LINE__);
     flag++;
     if(flag > 1){
          printk(KERN_INFO "flag > 1 \n");
          return IRQ_HANDLED;
     }

/* 使用定时器实现按键销抖代码 */
     // test_timer.expires = jiffies + 1*HZ;
     test_timer.expires = jiffies + msecs_to_jiffies(10);  //设置20ms定时时间

     add_timer(&test_timer);

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
     del_timer(&test_timer);
     platform_driver_unregister(&test_driver);
     printk(KERN_INFO "platform_driver_register exit\n");
}

module_init(test_init);
module_exit(test_exit);
 
MODULE_LICENSE("GPL v2");
