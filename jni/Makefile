include ./Rules.make
_USER_ := $(shell whoami)
_HOST_ := $(shell hostname)
__CC__1:= $(shell $(CC) -dumpversion)
__CC__ := $(CC) $(__CC__1)
__LD__ := $(LD)

TARGET = drv_test
SRC = \
		MediaLib.c \
		v4l2subdev.c \
		mediactl.c \
		ion.c \
		ovmem-ion.c \
		vbmem_lib.c \
		CameraAPI.c\
		drv_test.c
all:$(TARGET)
	$(CC) $(CFLAGS) $(SRC) -o $(TARGET)
	$(STRIP) $(TARGET)

clean:
	rm -f $(TARGET)

