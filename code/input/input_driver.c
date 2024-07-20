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

#include <linux/input.h>

static int irq;
static int key_test;
static struct device_node *key_node;
static struct input_dev *test_dev;

static void timer_function(unsigned long data);
DEFINE_TIMER(test_timer, timer_function, 0, 0);

static int flag;

static void timer_function(unsigned long data)
{
    int val = !gpio_get_value(key_test);
    printk(KERN_INFO "%s,line: %d\n", __func__, __LINE__);

    /*上报事件*/
    input_report_key(test_dev, KEY_1, val);

    input_sync(test_dev);
    
    flag = 0;
}

irq_handler_t test_key(int irq, void*args)
{
    printk(KERN_INFO "callback %s , %d \n",__func__, __LINE__);   
    flag++;
    if(flag > 1){
        flag = 0;
        printk(KERN_INFO "flag > 1 \n");   
        return IRQ_HANDLED;
    }

    test_timer.expires = jiffies + msecs_to_jiffies(10);  //设置10ms定时时间

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

    irq = irq_of_parse_and_map(key_node, 0);
    printk(KERN_INFO "irq_of_parse_and_map interrupt number is %d \n", irq);

    /* 申请中断 */
    int ret;
    ret = request_irq(irq, test_key, IRQF_TRIGGER_RISING | IRQF_TRIGGER_FALLING, "test_key", NULL);
    if(ret < 0) {
        printk(KERN_INFO " request_irq error \n");
        goto irq_err;
    }

    test_dev = input_allocate_device();

    test_dev->name = "test_key";

    __set_bit(EV_KEY, test_dev->evbit);
    __set_bit(KEY_1, test_dev->keybit);

    ret = input_register_device(test_dev);
    if(ret < 0){
        printk(KERN_INFO " input_register_device error \n");
        goto input_err;
    }

    return 0;

input_err:
    input_unregister_device(test_dev);
irq_err:
    free_irq(irq, NULL);
    return -1;
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
    input_unregister_device(test_dev);
    platform_driver_unregister(&test_driver);
    printk(KERN_INFO "platform_driver_register exit\n");
}

module_init(test_init);
module_exit(test_exit);
 
MODULE_LICENSE("GPL v2");
