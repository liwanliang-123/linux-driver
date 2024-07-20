input子系统:

查看当前设备有哪些input 设备： 
	cat /proc/bus/input/devices


测试节点是什么设备：
	hexdump /dev/input/event1

查看input的设备节点：
	ls /dev/input


input_report_key：上报事件

input_sync : 告诉linu内核input子系统上报结束
