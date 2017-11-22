#include "netclient.h"
int    sockfd;
int    netclient_serverstate;

void connectserver()
{
    int res;   
    struct sockaddr_in servaddr; 
  
    close(sockfd);
    sleep(1);

    //创建socket描述符  
    if((sockfd = socket(AF_INET, SOCK_STREAM, 0)) < 0){  
        printf("create socket error: %s(errno: %d)\n", strerror(errno),errno);  
        netclient_serverstate = -1;
        close(sockfd);
        return;
    }  
  
    memset(&servaddr, 0, sizeof(servaddr));  
    servaddr.sin_family = AF_INET;  
    servaddr.sin_port = htons(PORT);  
    if( inet_pton(AF_INET, SERVERIP, &servaddr.sin_addr) <= 0){  
        printf("inet_pton error for %s\n",SERVERIP);  
        netclient_serverstate = -1;
        close(sockfd);
        return;
    }  

    printf("connect to the server : %s:%d ... \n",SERVERIP,PORT);  
    if(res = connect(sockfd, (struct sockaddr*)&servaddr, sizeof(servaddr)) < 0){  
        printf("connect error: %s(errno: %d)\n",strerror(errno),errno); 
        netclient_serverstate = -1;
        close(sockfd); 
        wdt_feed();
        return;
    }  
 
    netclient_serverstate = 0;
    printf("connect remote server done ! the file number is %d\n",sockfd); 
}

void netclient_init()
{
    char devname[15];
    char senddata[15];
    int  res;

    connectserver();
   
    if(netclient_serverstate  == 0){
        usleep(100000);

        res = fgetcolname(devname);
        senddata[0] = 0x05;
        senddata[1] = 0x03;
        for(res = 0;res < 10; res ++)
        {
            senddata[res+2] = devname[res];
        }
        senddata[12] = 0x03;
        senddata[13] = 0x05;
        senddata[14] = '\0';

        printf("the device name is:%s!\n",devname);
       
        netclient_send(senddata,15);
    }
}

int netclient_send(char data[],int len)
{
    int res;

    if(netclient_serverstate == -1)
    {  
        printf("send msg error : the network is broken!\n");  
        return -1;
    }

    signal(SIGPIPE, SIG_IGN);

    res = send(sockfd, data, len, 0);
    if( res == -1 && errno == EPIPE)  {  
        printf("send msg error: %s(errno: %d)\n", strerror(errno), errno);  
        shutdown(sockfd,2);
        netclient_serverstate = -1;
        return -1;
    }  
    else
        return 0;
}

int netclient_recv(char data[])
{
    int rec_len;
    int res;  

    if(netclient_serverstate == -1)
    {  
        printf("rev msg error : the network is broken !\n");  
        sleep(1);
        return -1;
    }

    if((rec_len = recv(sockfd, data, MAXLINE,0)) == -1){                       //表示读取失败
        perror("socket recv error");  
        netclient_serverstate = -1;
        sleep(1);
        return -1;
    }  
    data[rec_len]  = '\0';  
    return rec_len;
}
