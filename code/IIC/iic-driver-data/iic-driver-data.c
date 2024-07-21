#include <linux/init.h>
#include <linux/module.h>

#include <linux/i2c.h>

static struct i2c_client *ft5x06_client;

static void ft5x06_write_reg(u8 reg_addr, u8 data, u8 len)
{
     u8 buf[256];
     struct i2c_msg msgs[] = {
          [0] = {
               .addr    = ft5x06_client->addr,
               .flags   = 0,
               .len     = len + 1,
               .buf     = buf,
          },
     };
     buf[0] = reg_addr;
     memcpy(&buf[1], &data, len);

     i2c_transfer(ft5x06_client->adapter, msgs, 1);
}

static int ft5x06_read_reg(u8 reg_addr)
{
     u8 data;
     struct i2c_msg msgs[] = {
          [0] = {
               .addr    = ft5x06_client->addr,
               .flags   = 0,
               .len     = sizeof(reg_addr),
               .buf     = &reg_addr,
          },
          [1] = {
               .addr    = ft5x06_client->addr,
               .flags   = 1,
               .len     = sizeof(data),
               .buf     = &data,
          },

     };

     i2c_transfer(ft5x06_client->adapter, msgs, 2);

     return data;
}

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

int ft5x06_probe(struct i2c_client *client, const struct i2c_device_id *id)
{
     int ret;

     printk(KERN_INFO "%s Exit\n", __func__);

     //因为要在其它函数中使用这个client, 所以要把它复制出来
     ft5x06_client = client;

     //往地址为 0x80的寄存器里面写入数据0x4b
     ft5x06_write_reg(0x80, 0x4b, 1);

     //读取地址为0x80的数据
     ret = ft5x06_read_reg(0x80);

     printk(KERN_INFO "ft5x06_read_reg ret = %#x\n", ret);

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
