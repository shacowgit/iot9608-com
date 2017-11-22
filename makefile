TARGET=communicate
SRCS=main.c serial.c bytesfactory.c  netclient.c wdt.c fileoperator.c dealana.c
OBJS=$(SRCS:.c=.o)

CROSS=arm-none-linux-gnueabi-
CC=$(CROSS)gcc
STRIP=$(CROSS)strip
GFLAGS=-g -O2 -lm

$(TARGET):$(OBJS)
	$(CC) -lpthread $(GFLAGS) -o $@ $(OBJS)
	$(STRIP) $@

clean:
	rm -rf $(TARGET) $(OBJS)

