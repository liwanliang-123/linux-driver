#!/bin/sh

arm-poky-linux-gnueabi-gcc chardev-user.c --sysroot=/opt/fsl-imx-x11/4.1.15-2.0.0/sysroots/cortexa7hf-neon-poky-linux-gnueabi/ -mfloat-abi=hard -mfpu=neon -o chardev

make
