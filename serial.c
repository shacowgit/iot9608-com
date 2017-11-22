#include "serial.h"



int setopt(int fd,int nSpeed,int nBits,char nEvent,int nStop)
{
    struct termios opt;

    if(tcgetattr(fd,&opt) < 0){
        return -1;
    }

    printf("config parametters ... \n");
    opt.c_lflag     &=  ~(ECHO|ICANON|IEXTEN|ISIG);
    opt.c_iflag     &=  ~(BRKINT|ICRNL|INPCK|ISTRIP|IXON);
    opt.c_oflag     &=  ~(OPOST);
    opt.c_cflag     &=  ~CSIZE;

    switch(nEvent){
        case 'O':
            opt.c_cflag |= PARENB;
            opt.c_cflag |= PARODD;
            opt.c_iflag |= (INPCK | ISTRIP);
            break;
        case 'E':
            opt.c_cflag |= PARENB;
            opt.c_cflag &= ~PARODD;
            opt.c_iflag |= (INPCK | ISTRIP);
            break;
        case 'N':
            opt.c_cflag &= ~PARENB;
            break;
    }

    switch(nBits){
        case 7:
            opt.c_cflag |= CS7;
            break;
        case 8:
            opt.c_cflag |= CS8;
            break;
    }    


    opt.c_cc[VMIN]  =   0;
    opt.c_cc[VTIME] =   15;
    
    switch(nSpeed){
        case 1200:
            cfsetispeed(&opt,B1200);
            cfsetospeed(&opt,B1200);
            break;
        case 2400:
            cfsetispeed(&opt,B2400);
            cfsetospeed(&opt,B2400);
            break;
        case 4800:
            cfsetispeed(&opt,B4800);
            cfsetospeed(&opt,B4800);
            break;
        case 9600:
            cfsetispeed(&opt,B9600);
            cfsetospeed(&opt,B9600);
            break;
        case 19200:
            cfsetispeed(&opt,B19200);
            cfsetospeed(&opt,B19200);
            break;
        case 57600:
            cfsetispeed(&opt,B57600);
            cfsetospeed(&opt,B57600);
            break;
        case 115200:
            cfsetispeed(&opt,B115200);
            cfsetospeed(&opt,B115200);
            break;
    }   

    if(nStop == 1){
        opt.c_cflag &= ~CSTOPB;
    }
    else if(nStop == 2){
        opt.c_cflag |= CSTOPB;
    }

    if(tcsetattr(fd,TCSANOW,&opt)<0){
        perror("com set error!");
        return -1;
    }

    tcflush(fd,TCIOFLUSH);

    printf("set com done!\n");
    return 0;
}

int openport(int fd,int comport)
{
    char *dev[] = {"/dev/ttyS0","/dev/ttyS1","/dev/ttyS2","/dev/ttyS3","/dev/ttyO5","/dev/ttyO4","/dev/ttyO3","/dev/ttyO1"};
    int port;

    printf("open device .... \n");
    fd = open(dev[comport - 1],O_RDWR|O_NOCTTY);
   
    if(fd < 0){
        perror(dev[comport - 1]);
        return -1;
    }

    printf("fd-open=%d\n",fd);
    return fd;
}

int serial_init(int port)
{
    int fd,i;
    fd = openport(fd,port);
    if(fd < 0){
        perror("open port error!");
        return -1;
    }

    i = setopt(fd,9600,8,'E',1);
    if(i < 0){
        perror("set port error!");
        return -1;
    }
    
    return fd;
}

int serial_send(int fd,char sendbuf[],int len)
{
    int result,i;
    printf("send data:");
    for(i=0;i<len;i++){
        printf("%x,",sendbuf[i]);
    }
    printf("...\n"); 
    result = write(fd,sendbuf,len);
    return result;
}

int serial_rev(int fd,char revdata[],int num)
{
    char revbuf[256];
    int revlen = 0;
    int revnum = 0;
    int i;

    printf("wait for data ... \n");

    while(revnum < num){
        revlen = read(fd,revbuf,140);
        if(revlen <= 0){
            break;
        }
            
        for(i=0;i<revlen;i++){
            revdata[revnum + i] = revbuf[i];
        }
        revnum = revnum + revlen;
    }

    printf("get date:%d\n",revnum);

    for(i=0;i<revnum;i++){
        printf("%x,",revdata[i]);
    }
    printf("\n");
    
    return revnum;
}

