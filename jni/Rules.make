#compiler setting
TOPDIR := ./

ARCH             := arm
CROSS_COMPILE	:=	arm-linux-gnueabihf-
ASM		:=	$(CROSS_COMPILE)as
LD		:=	$(CROSS_COMPILE)gcc
CC		:=	$(CROSS_COMPILE)gcc
CXX		:=	$(CROSS_COMPILE)g++
AR		:=	$(CROSS_COMPILE)ar
STRIP		:=	$(CROSS_COMPILE)strip
OBJCOPY		:=	$(CROSS_COMPILE)objcopy
OBJDUMP		:=	$(CROSS_COMPILE)objdump
RANLIB		:=	$(CROSS_COMPILE)ranlib

INCLUDE		:= \
			-I$(TOPDIR)/include/uapi/ \
			-I$(TOPDIR)/include/ 
