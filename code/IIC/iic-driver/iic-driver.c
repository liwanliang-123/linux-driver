#include <linux/init.h>
#include <linux/module.h>

#include <linux/i2c.h>

static const struct i2c_device_id ft5x06_id_ts[] = {
     {"xxx",0,},
     {}
};

static const struct of_device_id ft5x06_id[] = {
     {.compatible = "edt,edt-ft5306",0,},
     {.compatible = "edt,edt-ft5x06",0,},
     {.compatible = "edt,edt-ft5406",0,},
     {}
};

int ft5x06_probe(struct i2c_client *i2c_client, const struct i2c_device_id *id)
{
     printk(KERN_INFO "%s Exit\n", __func__);
     return 0;
}

int ft5x06_remove(struct i2c_client *i2c_client)
{
     printk(KERN_INFO "%s Exit\n", __func__);
     return 0;
}

static struct i2c_driver ft5x06_driver = {

     .driver = {
          .owner = THIS_MODULE,
          .name = "ft5x06_test",
          .of_match_table = ft5x06_id,
     },
     .probe = ft5x06_probe,
     .remove = ft5x06_remove,
     .id_table = ft5x06_id_ts
};

static int __init iic_driver_init(void)
{
     int ret;
     printk(KERN_INFO "%s Enter\n", __func__);

     ret = i2c_add_driver(&ft5x06_driver);
     if(ret < 0){
          printk(KERN_INFO "i2c_add_driver error ret = %d\n", ret);
          return ret;
     }

     return 0;
}

static void __exit iic_driver_exit(void)
{
     printk(KERN_INFO "%s Exit\n", __func__);
     i2c_del_driver(&ft5x06_driver);
}

module_init(iic_driver_init);
module_exit(iic_driver_exit);

MODULE_LICENSE("GPL");
