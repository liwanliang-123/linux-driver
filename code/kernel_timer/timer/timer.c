#include <linux/init.h>
#include <linux/module.h>

#include <linux/timer.h>

static void timer_function(unsigned long data);

DEFINE_TIMER(test_timer, timer_function, 0, 0);

static void timer_function(unsigned long data)
{
     printk(KERN_INFO "%s,line: %d\n", __func__, __LINE__);

     mod_timer(&test_timer, jiffies + 1*HZ);  //1秒时间到了之后，再设置1秒的定时时间
}

static int __init timer_init(void)
{
     printk(KERN_INFO "%s Enter\n", __func__);

     test_timer.expires = jiffies + 1*HZ;  //设置1秒定时时间

     add_timer(&test_timer);

     return 0;
}

static void __exit timer_exit(void)
{
     printk(KERN_INFO "%s Exit\n", __func__);
     del_timer(&test_timer);
}

module_init(timer_init);
module_exit(timer_exit);

MODULE_LICENSE("GPL");
