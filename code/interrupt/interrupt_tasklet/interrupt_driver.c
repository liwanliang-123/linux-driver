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

static int irq;
static int key_test;
static struct device_node *key_node;

struct tasklet_struct tasklet_key_test;

void tasklet_test(unsigned long data)
{
     int i;
/*data 是 tasklet_init函数传递的第三个参数 */
     printk(KERN_INFO "data = %lu \n", data);
     for(i = data; i > 0; i--) //在中断下半段打印100句话
          printk(KERN_INFO " i = %d \n", i);
}

irq_handler_t test_key(int irq, void*args)
{
     printk(KERN_INFO "start !!!\n");
     tasklet_schedule(&tasklet_key_test); //(2)调用 tasklet_schedule，启动中断下文
     printk(KERN_INFO "end !!!\n");

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

     /* 获取GPIO中断的中断号 */
     irq = gpio_to_irq(key_test);  
     printk(KERN_INFO "gpio_to_irq interrupt number is %d \n", irq);

     /* 申请中断 */
     int ret;
     ret = request_irq(irq, test_key, IRQF_TRIGGER_RISING, "test_key", NULL);
     if(ret < 0)
          printk(KERN_INFO " request_irq error \n");

     //(1) 初始化 tasklet
     tasklet_init(&tasklet_key_test, tasklet_test, 100);

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
     tasklet_kill(&tasklet_key_test);
     platform_driver_unregister(&test_driver);
     printk(KERN_INFO "platform_driver_register exit\n");
}

module_init(test_init);
module_exit(test_exit);
 
MODULE_LICENSE("GPL v2");
