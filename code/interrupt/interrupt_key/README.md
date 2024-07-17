interrupt:

Documentation/devicetree/bindings/arm/gic.txt

申请GPIO中断号有两个函数
（1）调用 gpio_to_irq 函数直接通过GPIO获取中断号，不需要在设备树中配置
（2）调用 irq_of_parse_and_map 函数申请GPIO中断号，这个函数需要事先在设备树中配置，配置如下：
	test_key{
		......
		interrupt-parent = <&gpio5>;
		interrupts = <4 IRQ_TYPE_EDGE_BOTH>;
	};