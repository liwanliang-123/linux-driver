设备树编译:

source_code/linux-4.1.15-elf1$ make dtbs
  HOSTCC  scripts/kconfig/conf.o
  HOSTCC  scripts/kconfig/zconf.tab.o
In file included from scripts/kconfig/zconf.tab.c:2534:
scripts/kconfig/confdata.c: In function ‘conf_write’:
scripts/kconfig/confdata.c:773:19: warning: ‘%s’ directive writing likely 7 or more bytes into a region of size between 1 and 4097 [-Wformat-overflow=]
  773 |  sprintf(newname, "%s%s", dirname, basename);
      |                   ^~~~~~
scripts/kconfig/confdata.c:773:19: note: assuming directive output of 7 bytes
In file included from /usr/include/stdio.h:867,
                 from scripts/kconfig/zconf.tab.c:84:
/usr/include/x86_64-linux-gnu/bits/stdio2.h:36:10: note: ‘__builtin___sprintf_chk’ output 1 or more bytes (assuming 4104) into a destination of size 4097
   36 |   return __builtin___sprintf_chk (__s, __USE_FORTIFY_LEVEL - 1,
      |          ^~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
   37 |       __bos (__s), __fmt, __va_arg_pack ());
      |       ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
In file included from scripts/kconfig/zconf.tab.c:2534:
scripts/kconfig/confdata.c:776:20: warning: ‘.tmpconfig.’ directive writing 11 bytes into a region of size between 1 and 4097 [-Wformat-overflow=]
  776 |   sprintf(tmpname, "%s.tmpconfig.%d", dirname, (int)getpid());
      |                    ^~~~~~~~~~~~~~~~~
In file included from /usr/include/stdio.h:867,
                 from scripts/kconfig/zconf.tab.c:84:
/usr/include/x86_64-linux-gnu/bits/stdio2.h:36:10: note: ‘__builtin___sprintf_chk’ output between 13 and 4119 bytes into a destination of size 4097
   36 |   return __builtin___sprintf_chk (__s, __USE_FORTIFY_LEVEL - 1,
      |          ^~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
   37 |       __bos (__s), __fmt, __va_arg_pack ());
      |       ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
  HOSTLD  scripts/kconfig/conf
scripts/kconfig/conf  --silentoldconfig Kconfig
*
* Restart config...
*
*
* Linux/x86 4.1.15 Kernel Configuration
*
64-bit kernel (64BIT) [Y/n/?] (NEW) ^Cmake[2]: *** [scripts/kconfig/Makefile:38: silentoldconfig] Interrupt
make[1]: *** [Makefile:543: silentoldconfig] Interrupt

source_code/linux-4.1.15-elf1$ make ARCH=arm CROSS_COMPILE=arm-linux-gnueabihf-  dtbs
  CHK     include/config/kernel.release
  CHK     include/generated/uapi/linux/version.h
  CHK     include/generated/utsrelease.h


单独编译一个设备树文件：
source_code/linux-4.1.15-elf1$ make ARCH=arm CROSS_COMPILE=arm-linux-gnueabihf- imx6ull-elf1-emmc.dtb


更新系统设备树文件：
source_code/linux-4.1.15-elf1/arch/arm/boot/dts$ scp imx6ull-elf1-emmc.dtb root@192.168.0.101:~
mv ~/ imx6ull-elf1-emmc.dtb  /run/media/mmcblk1p1 
sync
reboot
即可更新设备树文件


查看设备树：
cd /proc/device 目录就能够看到添加的节点

====================================================

设备树DT: device tree
暂开设备树FDT: Flattened Device Tree
设备树代码DTS: device tree source
通用设备树代码DTSI：device tree source include 
DTS编译得到DTB(device tree blob)
设备树编译器DTC(device tree compiler)


