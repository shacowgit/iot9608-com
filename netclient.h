#include <stdio.h> 
#include <stdlib.h>
#include <unistd.h>
#include <errno.h> 
#include <string.h> 
#include <netdb.h> 
#include <sys/types.h> 
#include <netinet/in.h> 
#include <sys/socket.h> 
#include <arpa/inet.h>
#include <signal.h>
#include "wdt.h"
#include "fileoperator.h"

#define PORT 51230                      // The port which is communicate with server
#define MAXLINE 4096
//#define SERVERIP "172.16.113.88"
#define SERVERIP "172.16.2.10"

extern int netclient_serverstate;
extern void netclient_init();
extern int netclient_send(char data[],int len);
extern int netclient_recv(char data[]);
