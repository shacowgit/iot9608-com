#ifndef BYTESFACTORY_H
#define BYTESFACTORY_H
#include <stdio.h>

extern int bytesfactory_checkcrc16(char bytebuf[],int cnt);
extern int bytesfactory_getsendbytes(char inbuf[] ,char intbuf[]);
extern int bytesfactory_addcrc16(char bytebuf[],int offset);

#endif
