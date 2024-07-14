
pinctl 和 gpio 子系统：

文档查看：linux-4.1.15/Documentation/devicetree/bindings

pinctrl 子系统： 设置引脚的电气属性和复用关系.

GPIO 子系统： 当 pinctrl 子系统将GPIO的复用关系设置为GPIO之后，使用 GPIO 子系统来操作GPIO (设置高低电平，设置输入输出).


1、 pinctrl 子系统设置复用关系：修改设备树
2、编写驱动代码
