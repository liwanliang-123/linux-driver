#include <linux/init.h>
#include <linux/module.h>

#include <linux/platform_device.h>

static int led_probe(struct platform_device *pdev)
{
     printk(KERN_INFO "platform driver led_probe function\n");

     // >>>>>> get device resource -1 <<<<<
     printk(KERN_INFO "platform_device led_probe resource is [%s]\n", pdev->resource[0].name);
     printk(KERN_INFO "led_resource_start is[ 0x%x ]\n", pdev->resource[0].start);
     printk(KERN_INFO "led_resource_end is[ 0x%x ]\n", pdev->resource[0].end);

     // >>>>>>> get device resource -2 <<<<<< (推荐)
     struct resource *led_mem;
     struct resource *led_mem_tmp;

     led_mem = platform_get_resource(pdev, IORESOURCE_MEM, 0);
     if(led_mem == NULL){
          printk(KERN_INFO "platform_get_resource function error\n");
          return -EBUSY;
     }
     printk(KERN_INFO "platform_device led_probe resource is [%s]\n", led_mem->name);
     printk(KERN_INFO "led_resource_start is[ 0x%x ]\n", led_mem->start);
     printk(KERN_INFO "led_resource_end is[ 0x%x ]\n", led_mem->end);

#if 0
     led_mem_tmp = request_mem_region(led_mem->start,led_mem->end - led_mem->start +1 ,"led");
     if(led_mem_tmp == NULL){
          printk(KERN_INFO "request_mem_region function error\n");
          goto err_region;
     }

err_region:
     release_mem_region(led_mem->start,led_mem->end - led_mem->start +1);
     return -EBUSY
#endif
     return 0;
}

static int led_remove(struct platform_device *pedv)
{
     printk(KERN_INFO "platform driver led_remove function\n");
     return 0;
}

const struct platform_device_id led_idtable = {
     .name = "led" //first probe
};

struct platform_driver led_driver = {
     .probe = led_probe,
     .remove = led_remove,
     .driver = {
          .name = "ledtest",
          .owner = THIS_MODULE
     },
     .id_table = &led_idtable
};

static int __init led_driver_init(void)
{
     printk(KERN_INFO "platform driver enter\n");

     return platform_driver_register(&led_driver);
}

static void __exit led_driver_exit(void)
{
     printk(KERN_INFO "platform driver exit\n");

     return platform_driver_unregister(&led_driver);
}

module_init(led_driver_init);
module_exit(led_driver_exit);

MODULE_LICENSE("GPL v2");
