#ifndef SERIAL_H
#define SERIAL_H
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <termios.h>
#include <errno.h>
#include <limits.h>
#include "bytesfactory.h"

extern int serial_init(int port);
extern int serial_send(int fd,char sendbuf[],int len);
extern int serial_rev(int fd,char revdata[],int num);

#endif
