#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <fcntl.h>
#include <sys/ioctl.h>
#include <linux/watchdog.h>
#define WDT "/dev/watchdog"

int wdt_fd = -1;

int wdt_init(int timeout)
{
    wdt_fd = open(WDT, O_WRONLY);
    if (wdt_fd == -1) {
        printf("fail to open "WDT "!\n");
    }
    printf(WDT " is opened!\n");

    ioctl(wdt_fd, WDIOC_SETTIMEOUT, &timeout);
    printf("The timeout is %d seconds\n", timeout);
}

int wdt_feed()
{
    write(wdt_fd, "\0", 1);
}
