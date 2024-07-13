#include <linux/init.h>
#include <linux/module.h>

#include <linux/platform_device.h>

struct resource led_res[] = {
     [0] = {
          .start = 0x20AC000,
          .end = 0x20AC003,
          .flags = IORESOURCE_MEM,
          .name = "GPIO5_DR"
     }
};

void led_release(struct device *dev)
{
     printk(KERN_INFO "platform device release\n");
}

struct platform_device led_device = {
	.name = "led",
	.id = -1,
	.num_resources = ARRAY_SIZE(led_res),
	.resource = led_res,
     .dev = {
          .release = led_release
     }
};

static int __init device_init(void)
{
     printk(KERN_INFO "platform device enter\n");

     return platform_device_register(&led_device);
}

static void __exit device_exit(void)
{
     printk(KERN_INFO "platform device exit\n");

     return platform_device_unregister(&led_device);
}

module_init(device_init);
module_exit(device_exit);
 
MODULE_LICENSE("GPL v2");
