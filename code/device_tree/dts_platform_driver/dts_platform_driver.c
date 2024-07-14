#include <linux/init.h>
#include <linux/module.h>

#include <linux/of_address.h>
#include <linux/of.h>
#include <linux/io.h>
#include <linux/platform_device.h>

static unsigned int *vir_GPIO_dr;
static struct device_node *test_node;

int dts_probe(struct platform_device *pedv)
{
     /* get device node 1 */
     const char *node_name = pedv->dev.of_node->name;
     printk(KERN_INFO "node is %s\n",node_name);

     /*===================*/

     /* get device node 2 */
     test_node = of_find_node_by_path("/test");
     if(test_node == NULL){
          printk(KERN_INFO "of_find_node_by_path error\n");
          return -1;
     }
     printk(KERN_INFO "of_find_node is %s\n", test_node->name);

     int ret;
     int out_values[2] = {0};
     ret = of_property_read_u32_array(test_node,"reg",out_values,2);
     if(ret < 0){
          printk(KERN_INFO "of_property_read_u32_array error\n");
          return -1;
     }
     printk(KERN_INFO "of_property_read_u32 \nreg array[1] is %08x \nreg array[2] is %08x\n", 
                                   out_values[0],out_values[1]);

     /*
          of_iomap:用于直接PA -> VA内存映射，表示reg属性中要完成内存映射的段，如果reg属性只有一段则为0
          reg = <0x20ac000 0x0000004
                 0x20ac004 0x0000004
               >; 表示reg的属性有两段，则 of_iomap 第二个参数为1
     */
     vir_GPIO_dr = of_iomap(test_node, 0);
     if(vir_GPIO_dr == NULL){
          printk(KERN_INFO "of_iomap error\n");
          return -1;
     }

     printk(KERN_INFO "platform_driver_register %s\n", __func__);
     return 0;
}

int dts_remove(struct platform_device *pedv)
{
     printk(KERN_INFO "platform_driver_register %s\n", __func__);
     iounmap(vir_GPIO_dr);
     return 0;
}

const struct platform_device_id dts_id_table = {
     .name = "test" //然后匹配
};

const struct of_device_id of_match_table_test[] = {
     {.compatible = "test-1"}, //最先匹配
     {}
};

struct platform_driver dts_driver = {
     .probe = dts_probe,
     .remove = dts_remove,
     .driver = {
          .owner = THIS_MODULE,
          .name = "test", //最后匹配
          .of_match_table = of_match_table_test
     },
     .id_table = &dts_id_table
};

static int __init dts_init(void)
{
     int ret;

     ret = platform_driver_register(&dts_driver);
     if(ret < 0){
          printk(KERN_INFO "platform_driver_register error\n");
     }
     printk(KERN_INFO "platform_driver_register successed\n");

     return 0;
}

static void __exit dts_exit(void)
{
     platform_driver_unregister(&dts_driver);
     printk(KERN_INFO "platform_driver_register exit\n");
}

module_init(dts_init);
module_exit(dts_exit);
 
MODULE_LICENSE("GPL");
