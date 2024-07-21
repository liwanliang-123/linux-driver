#include <linux/init.h>
#include <linux/module.h>

#include <linux/i2c.h>

//分配一个I2C适配器指针
struct i2c_adapter *i2c_ada;

//分配一个I2C_client指针
struct i2c_client *i2c_client;

/* 支持的i2c设备列表 */
struct i2c_board_info ft5x06_info[] = {
     //每一项都代表一个I2C设备，这句代码就是说这个设备名字是ft5x06_test, 器件地址是0x38
     {I2C_BOARD_INFO("ft5x06_test", 0x38)},
     {}
};

static int __init iic_client_init(void)
{
     printk(KERN_INFO "%s Enter\n", __func__);

     //调用i2c_get_adapter，或者获得一个I2C总线，因为器件ft5x06是挂载在该 I2C  2 总线上面,
     //所以这个参数就是1, 所以这句代码的意思就是把这个触摸芯片挂载到 I2C总线上面
     i2c_ada = i2c_get_adapter(1);

     //把i2c client 和 i2c 器件关联起来
     i2c_client = i2c_new_device(i2c_ada, ft5x06_info);

     //释放i2c控制器
     i2c_put_adapter(i2c_ada);

     return 0;
}

static void __exit iic_client_exit(void)
{
     printk(KERN_INFO "%s Exit\n", __func__);

     i2c_unregister_device(i2c_client);
}

module_init(iic_client_init);
module_exit(iic_client_exit);

MODULE_LICENSE("GPL");
