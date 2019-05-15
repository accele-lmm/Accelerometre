UCLINUX_PATH=~/olimex/uClinux-dist-lpc_2478_stk_modified
UCLIBC_DIR=$(UCLINUX_PATH)/lib/uClibc
INCLUDEDIR_DIST=$(UCLINUX_PATH)/include
INCLUDEDIR_UCLINUX=$(UCLINUX_PATH)/linux-2.6.x/include
INCLUDEDIR_UCLIBC=$(UCLINUX_PATH)/lib/uClibc/include

#ARM_LINUX_PATH=/usr/local/arm-linux
#ARM_LINUX_INCLUDE=$(ARM_LINUX_PATH)/include
#ARM_LINUX_LIB=$(ARM_LINUX_PATH)/lib

ARM_ELF_PATH=/usr/local/arm-elf
ARM_ELF_INCLUDE=$(ARM_ELF_PATH)/include
ARM_ELF_LIB=$(ARM_ELF_PATH)/lib
#RUNTIME=$(UCLIBC_DIR)/lib/crt0.o $(UCLIBC_DIR)/lib/crti.o $(UCLIBC_DIR)/lib/crtn.o

CC=arm-elf-gcc
CFLAGS= -Wall -W
LDFLAGS= -Wl, -elf2flt

#CFLAGS = -g -Dlinux -D__linux__ -Dunix -D__uClinux__ -DEMBED -D__PIC__ -fpic -msoft-float -fno-builtin -nostartfiles -I$(ARM_ELF_INCLUDE) -I.
#LDFLAGS = -Wl, --fatal-warnings -Wl, -elf2flt -msoft-float
#LDLIBS = -L$(ARM_ELF_LIB) -lm -L$(UCLIBC_DIR)/. -L$(UCLIBC_DIR)/lib 

SRC = main.c smb380_drv.c i2c0_drv.c sys.c fb_Graphic.c
OBJS = main.o smb380_drv.o i2c0_drv.o sys.o fb_Graphic.o
EXEC = acce05

OBJ=$(SRC:%.c=%.o)

$(EXEC): $(OBJ)
	$(CC) $(CFLAGS) -o $(EXEC) $(OBJ) $(LDFLAGS)

#all: $(EXEC)

#$(EXEC): $(OBJS)
#	$(CC) $(LDFLAGS) -o $@ $(OBJS) $(LDLIBS)

#all:
#	$(CC) $(CFLAGS) -o $(OBJS) -c $(SRC) $(LDLIBS)
#	$(CC) $(CFLAGS) $(RUNTIME) $(LDFLAGS) -o $(OBJS) $(EXEC)

#romf s :
#	$ (ROMFSINST) / b i n /$ (EXEC)

clean:
	rm -f $(EXEC)  *.gdb *.o *~ romfs_5.img vmlinux.bin
