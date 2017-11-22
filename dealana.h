#include "netclient.h"
#include "fileoperator.h"

#define FIXED_FRAME_LENGTH 18
#define FIXED_FRAME_HEDA   0x10
#define FIXED_FRAME_FOOT   0x16
#define FIXED_CONTROL      1
#define FIXED_ADDRESS      3
#define FIXED_DATA         11
#define ACT_FRAME_SEG      0x68
#define ACT_CONTROL        4
#define ACT_ADDRESS        6
#define ACT_DATA           14

extern int deal_revbufana(char databuf[]);
extern int deal_sendrtdata(char fname[]);
extern int deal_setcolinfo(char databuf[]);
