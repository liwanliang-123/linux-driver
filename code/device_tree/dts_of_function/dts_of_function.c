#include <linux/init.h>
#include <linux/module.h>

#include <linux/of.h>

static int size;

static struct device_node *test_node;
static struct property *test_property;

static int __init dts_nod_init(void)
{
     printk(KERN_INFO "%s called\n", __func__);
     /* 1 find node*/
     test_node = of_find_node_by_path("/test");
     if(test_node == NULL){
          printk(KERN_INFO "of_find_node_by_path error\n");
          return -1;
     }
     printk(KERN_INFO "of_find_node is %s\n", test_node->name);

     /* 2 get compatible property */
     test_property = of_find_property(test_node,"compatible",&size);
     if(test_property == NULL){
          printk(KERN_INFO "of_find_property error\n");
	     return -1;
     }
     printk(KERN_INFO "of_find_property is %s value is %s size is %d\n", 
                                   test_property->name,(char*)test_property->value,size);

     /* 3 get reg property */
     int ret;
     int out_values[2] = {0};
     ret = of_property_read_u32_array(test_node,"reg",out_values,2);
     if(ret < 0){
          printk(KERN_INFO "of_property_read_u32_array error\n");
          return -1;
     }
     printk(KERN_INFO "of_property_read_u32 \nreg array[1] is %08x \nreg array[2] is %08x\n", 
                                   out_values[0],out_values[1]);

     /* 4 get status property */
     int ret2;
     const char *out_string;
     ret2 = of_property_read_string(test_node, "status", &out_string);
     if(ret2 < 0){
          printk(KERN_INFO "of_property_read_string error\n");
          return -1;
     }
     printk(KERN_INFO "of_property_read_string is %s\n",out_string);

     return 0;
}

static void __exit dts_nod_exit(void)
{
     printk(KERN_INFO "%s called\n", __func__);
}

module_init(dts_nod_init);
module_exit(dts_nod_exit);
 
MODULE_LICENSE("GPL");
