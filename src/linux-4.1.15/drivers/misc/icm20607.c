#include <linux/module.h>
#include <linux/init.h>
#include <linux/fs.h>           // 包含文件系统相关函数的头文件
#include <linux/uaccess.h>      // 包含用户空间数据访问函数的头文件
#include <linux/cdev.h>         //包含字符设备头文件
#include <linux/device.h>
#include <linux/delay.h>
#include <linux/spi/spi.h>
#include <linux/regmap.h>
#include <linux/of.h>
#include <linux/of_address.h>
#include <linux/of_gpio.h>
#include <linux/icm20607.h>

#define ICM20607_REG_WHOAMI      0x75
#define ICM20607_WHOAMI_VALUE    0xAF

#define DEVICE_NAME "icm20607"  // 设备名称
static dev_t dev_num;   //分配的设备号
int major;  //主设备号
int minor;  //次设备号


struct icm20607_dev {
	struct spi_device *spi_dev;	/* spi设备 */
	dev_t dev_num;			/* 设备号 	 */
	struct cdev cdev;		/* cdev 	*/
	struct class *class;		/* 类 		*/
	struct device *device;		/* 设备 	 */
	struct device_node	*nd; 	/* 设备节点 */
	int cs_gpio;				/* 片选所使用的GPIO编号	*/
	signed int gyro_x_adc;		/* 陀螺仪X轴原始值 	 */
	signed int gyro_y_adc;		/* 陀螺仪Y轴原始值		*/
	signed int gyro_z_adc;		/* 陀螺仪Z轴原始值 		*/
	signed int accel_x_adc;		/* 加速度计X轴原始值 	*/
	signed int accel_y_adc;		/* 加速度计Y轴原始值	*/
	signed int accel_z_adc;		/* 加速度计Z轴原始值 	*/
	signed int temp_adc;		/* 温度原始值 			*/
	struct regmap *spi_regmap;		/* regmap */
	struct regmap_config regmap_config;	
};

void icm20607_readdata(struct icm20607_dev *dev)
{
	u8 ret;
	unsigned char data[14];

	ret = regmap_bulk_read(dev->spi_regmap, ICM20_ACCEL_XOUT_H, data, 14);/*读多个寄存器的值*/

	dev->accel_x_adc = (signed short)((data[0] << 8) | data[1]); 
	dev->accel_y_adc = (signed short)((data[2] << 8) | data[3]); 
	dev->accel_z_adc = (signed short)((data[4] << 8) | data[5]); 
	dev->temp_adc    = (signed short)((data[6] << 8) | data[7]); 
	dev->gyro_x_adc  = (signed short)((data[8] << 8) | data[9]); 
	dev->gyro_y_adc  = (signed short)((data[10] << 8) | data[11]);
	dev->gyro_z_adc  = (signed short)((data[12] << 8) | data[13]);
}

static int icm20607_spi_read(struct icm20607_dev *dev, unsigned int reg, unsigned int *val)
{
	
    return regmap_read(dev->spi_regmap, reg, val);
}

static int icm20607_spi_write(struct icm20607_dev *dev, unsigned int reg, unsigned int val)
{
	
    return regmap_write(dev->spi_regmap, reg, val);
}

static const struct regmap_config spi_regmap_config = {
    .reg_bits = 8,
    .val_bits = 8,
    .read_flag_mask = 0x80,
    .reg_read = icm20607_spi_read,
    .reg_write = icm20607_spi_write,
    .max_register = ICM20607_REG_WHOAMI,
};

static int device_open(struct inode *inode, struct file *file)
{
// 在这里处理设备打开的操作
printk(KERN_INFO "This is device_open.\n");
    return 0;
}
static int device_release(struct inode *inode, struct file *file)
{
// 在这里处理设备关闭的操作
printk(KERN_INFO "This is device_release.\n");

    return 0;
}

static ssize_t device_read(struct file *file, char __user *buffer, size_t length, loff_t *offset)
{
signed int data[7];
	long err = 0;
	struct cdev *cdev = file->f_path.dentry->d_inode->i_cdev;
	struct icm20607_dev *dev = container_of(cdev, struct icm20607_dev, cdev);
            
	icm20607_readdata(dev);
	data[0] = dev->gyro_x_adc;
	data[1] = dev->gyro_y_adc;
	data[2] = dev->gyro_z_adc;
	data[3] = dev->accel_x_adc;
	data[4] = dev->accel_y_adc;
	data[5] = dev->accel_z_adc;
	data[6] = dev->temp_adc;
	err = copy_to_user(buffer, data, sizeof(data));
	return 0;
}

#if 0
static ssize_t device_read(struct file *file, char __user *buffer, size_t length, loff_t *offset)
{
	u8 ret;
	unsigned char data[14];
	unsigned char value[7];
printk("--------------------\n");
// 在这里处理设备读取的操作
struct cdev *cdev = file->f_path.dentry->d_inode->i_cdev;
struct icm20607_dev *dev = container_of(cdev, struct icm20607_dev, cdev);

ret = regmap_bulk_read(dev->spi_regmap, ICM20_ACCEL_XOUT_H, data, 14);/*读多个寄存器的值*/
if(ret < 0) {
	return ret;
}
dev->accel_x_adc = (signed short)((data[0] << 8) | data[1]);
dev->accel_y_adc = (signed short)((data[2] << 8) | data[3]);
dev->accel_z_adc = (signed short)((data[4] << 8) | data[5]);
dev->temp_adc    = (signed short)((data[6] << 8) | data[7]);
dev->gyro_x_adc  = (signed short)((data[8] << 8) | data[9]);
dev->gyro_y_adc  = (signed short)((data[10] << 8) | data[11]);
dev->gyro_z_adc  = (signed short)((data[12] << 8) | data[13]);

value[0] = dev->gyro_x_adc;
value[0] = dev->gyro_x_adc;
value[2] = dev->gyro_z_adc;
value[3] = dev->accel_x_adc;
value[4] = dev->accel_y_adc;
value[5] = dev->accel_z_adc;
value[6] = dev->temp_adc;

ret = copy_to_user(buffer, value, sizeof(value));

printk(KERN_INFO "This is device_read.\n");
    return 0;
}
#endif
static ssize_t device_write(struct file *file, const char __user *buffer, size_t length, loff_t *offset)
{
// 在这里处理设备写入的操作
printk(KERN_INFO "This is device_write.\n");
    return 0;
}
void icm20607_reginit(struct icm20607_dev *dev)
{
        u8 value = 0;

        icm20607_spi_write(dev, ICM20_PWR_MGMT_1, 0x80);
        mdelay(50);
        icm20607_spi_write(dev, ICM20_PWR_MGMT_1, 0x01);
        mdelay(50);

        icm20607_spi_read(dev, ICM20_WHO_AM_I,&value);
        printk("ICM20607 ID = %#X\r\n", value);

        icm20607_spi_write(dev, ICM20_SMPLRT_DIV, 0x00);        /* 输出速率是内部采样率                                 */
        icm20607_spi_write(dev, ICM20_GYRO_CONFIG, 0x18);       /* 陀螺仪±2000dps量程                           */
        icm20607_spi_write(dev, ICM20_ACCEL_CONFIG, 0x18);      /* 加速度计±16G量程                                             */
        icm20607_spi_write(dev, ICM20_CONFIG, 0x04);            /* 陀螺仪低通滤波BW=20Hz                                */
        icm20607_spi_write(dev, ICM20_ACCEL_CONFIG2, 0x04); /* 加速度计低通滤波BW=21.2Hz                        */
        icm20607_spi_write(dev, ICM20_PWR_MGMT_2, 0x00);        /* 打开加速度计和陀螺仪所有轴                           */
        icm20607_spi_write(dev, ICM20_LP_MODE_CFG, 0x00);       /* 关闭低功耗                                           */
        icm20607_spi_write(dev, ICM20_FIFO_EN, 0x00);           /* 关闭FIFO                                             */
}
static struct file_operations fops = {
    .owner = THIS_MODULE,
    .open = device_open,
    .release = device_release,
    .read = device_read,
    .write = device_write,
};
static int icm20607_probe(struct spi_device *spi)
{
    int ret;
    unsigned int whoami;
    struct icm20607_dev *icm20607dev;


    /* 分配icm20607dev对象的空间 */
    icm20607dev = devm_kzalloc(&spi->dev, sizeof(*icm20607dev), GFP_KERNEL);
    if(!icm20607dev)
	    return -ENOMEM;
    // 创建 ICM20607 设备的 regmap
    icm20607dev->spi_regmap = regmap_init_spi(spi, &spi_regmap_config);
    if (IS_ERR(icm20607dev->spi_regmap)) {
        dev_err(&spi->dev, "Failed to initialize regmap: %ld\n", PTR_ERR(icm20607dev->spi_regmap));
        return PTR_ERR(icm20607dev->spi_regmap);
    }

    // 读取 WHOAMI 寄存器
/*    ret = regmap_read(icm20608dev->spi_regmap, ICM20607_REG_WHOAMI, &whoami);
    if (ret) {
        dev_err(&spi->dev, "Failed to read WHOAMI register: %d\n", ret);
        return ret;
    }

    if (whoami != ICM20607_WHOAMI_VALUE) {
        dev_err(&spi->dev, "Invalid WHOAMI value: 0x%X\n", whoami);
        return -ENODEV;
    }
 */

    // 注册字符设备驱动程序
    ret = alloc_chrdev_region(&icm20607dev->dev_num,0,1,DEVICE_NAME);
     if (ret < 0) {
	     printk(KERN_ALERT "Failed to allocate device number: %d\n", ret);
	     return ret;
     }
     major=MAJOR(icm20607dev->dev_num);
     minor=MINOR(icm20607dev->dev_num);
     printk(KERN_INFO "major number: %d\n",major);
     printk(KERN_INFO "minor number: %d\n",minor);

     icm20607dev->cdev.owner = THIS_MODULE;
     cdev_init(&icm20607dev->cdev,&fops);
     cdev_add(&icm20607dev->cdev,icm20607dev->dev_num,1);

     // 创建设备类
     icm20607dev->class = class_create(THIS_MODULE, DEVICE_NAME);
     if (IS_ERR(icm20607dev->class)) {
	     pr_err("Failed to create class\n");
	     return PTR_ERR(icm20607dev->class);
     }

     // 创建设备节点并关联到设备类
     icm20607dev->device = device_create(icm20607dev->class, NULL, MKDEV(major, minor), NULL, DEVICE_NAME);
     if (IS_ERR(icm20607dev->device)) {
	     pr_err("Failed to create device\n");
	     class_destroy(icm20607dev->class);
	     return PTR_ERR(icm20607dev->device);
     }
     /*初始化spi_device */
    icm20607dev->spi_dev = spi;
     spi->mode = SPI_MODE_0;
     spi_setup(spi);
     /* 初始化ICM20607内部寄存器 */
     icm20607_reginit(icm20607dev);
     /* 保存icm20607dev结构体 */
     spi_set_drvdata(spi, icm20607dev);
    pr_info("ICM20607 SPI device probed successfully\n");

    return 0;
}

static int icm20607_remove(struct spi_device *spi)
{
	struct icm20607_dev *icm20607dev = spi_get_drvdata(spi);
    // 在此处执行 ICM20607 设备的清理操作
	//删除cdev
    cdev_del(&icm20607dev->cdev);
	//注销设备号
	unregister_chrdev_region(icm20607dev->dev_num, 1);
	
	//注销设备
	device_destroy(icm20607dev->class, icm20607dev->dev_num);
	//注销类
	class_destroy(icm20607dev->class);
	//删除regmap
	regmap_exit(icm20607dev->spi_regmap);
    pr_info("ICM20607 SPI device removed successfully\n");

    return 0;
}
static const struct of_device_id of_platform_match[] = {
        { .compatible = "elf,icm20607", },
        {},
};

static struct spi_driver icm20607_driver = {
    .driver = {
        .name = "icm20607",
        .owner = THIS_MODULE,
	.of_match_table = of_platform_match,
    },
    .probe = icm20607_probe,
    .remove = icm20607_remove,
};

static int __init icm20607_init(void)
{
    int ret;

    ret = spi_register_driver(&icm20607_driver);
    if (ret < 0) {
        pr_err("Failed to register ICM20607 driver: %d\n", ret);
        return ret;
    }

    pr_info("ICM20607 SPI device driver loaded\n");
    return 0;
}

static void __exit icm20607_exit(void)
{
    spi_unregister_driver(&icm20607_driver);
    pr_info("ICM20607 SPI device driver unloaded\n");
}

module_init(icm20607_init);
module_exit(icm20607_exit);

MODULE_LICENSE("GPL");
MODULE_AUTHOR("Your Name");
MODULE_DESCRIPTION("ICM20607 SPI device driver");
