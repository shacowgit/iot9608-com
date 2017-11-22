#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <fcntl.h>
#include <sys/ioctl.h>
#include <linux/watchdog.h>
#define WDT "/dev/watchdog"

extern int wdt_init(int timeout);
extern int wdt_feed();
