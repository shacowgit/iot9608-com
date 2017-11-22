#include "dealana.h"

int deal_revbufana(char databuf[])              //receive buffer analysis
{
    int type;
    type = frametypecharge(databuf);

    if(type < 0)
    {
        printf("receive nothing!\n");
    }

    if(type > 0)
    {
        printf("there is a active frame:");
        switch(databuf[18])
        {
            case 0x04:
                printf("it is a set collector info cmd!\n");
                deal_setcolinfo(databuf);
                break;
            default:
                printf("i can't understand now!");
                break;
        }
    }
    else
    {
        printf("it is a fixed frame!");
        switch(databuf[15])                                   //the first byte is a flag
        {
            case 0x03:
                printf("it is query elec head cmd!\n");
                sendelechisinfo(databuf);
                break;
            case 0x04:
                printf("it is query collector info cmd!\n");
                sendsetinfo();
                break;
            case 0x05:
                printf("it is query history data cmd!\n");
                sendhisdata(databuf);
                break;
            default:
                printf("i can't understand now!");
                break;
        }
    }
}

int deal_sendrtdata(char fname[])                    //send realtime data to remote server
{
    char elecdata[RECORDDATALEN],senddata[RECORDDATALEN + 21];
    int  datanum,i,len;

    datanum = fgetrtdata(fname,elecdata);
    
    len = RECORDDATALEN + 10 + 4;
    if(datanum > 0)
    {
        senddata[0]  = 0x68;
        senddata[1]  = len >> 8;       
        senddata[2]  = len;       
        senddata[3]  = 0x68;

        senddata[4]  = 0x00;
        senddata[5]  = 0x03;

        senddata[6]  = 0x00;       
        senddata[7]  = 0x00;       
        senddata[8]  = 0x00;
        senddata[9]  = 0x00;
        senddata[10] = 0x00;
        senddata[11] = 0x00;       
        senddata[12] = 0x00;       
        senddata[13] = 0x00;

        senddata[14] = 0x01;
        senddata[15] = 0x01;
        senddata[16] = 0x00;
        senddata[17] = 0x10; 

        senddata[RECORDDATALEN + 18] = 0x00;
        senddata[RECORDDATALEN + 19] = 0x00;
        senddata[RECORDDATALEN + 20] = 0x68;
        for(i = 0;i<RECORDDATALEN;i++)
        {
            senddata[i+18] = elecdata[i];
        }
        netclient_send(senddata,RECORDDATALEN + 21);  
    }
}

int frametypecharge(char databuf[])    // charge the frame type
{
    int i;
    printf("command type charge ! \n");
    for(i=0;i<1500;i++)
    {
        if(databuf[i] == 0x10)
        {
            if((i+17) < 1500 && databuf[i+17] == 0x16)
                return 0;
        }
        if(databuf[i] == 0x68)
        {
            if((i+3) < 1500 && databuf[i + 3] == 0x68)
                return 1;
        }
    }
    return -1;
}

int sendelechisinfo(char databuf[])
{
    int  datanum,i;
    char data[301];
    char buf[71];
    char fname[] = {'.','.','/','d','s','m','/','1','1','\0'};
    fname[7] = databuf[12] + 0x30;
    fname[8] = databuf[13] + 0x30;
    datanum = fgethddata(fname,data);
    if(datanum > 0){
        for(i=0;i<54;i++){
            buf[i+18] = data[i];
        }
        buf[0]  = 0x68;
        buf[1]  = 0x00;       
        buf[2]  = 0x40;       
        buf[3]  = 0x68;

        buf[4]  = 0x00;
        buf[5]  = 0x03;

        buf[6]  = 0x00;       
        buf[7]  = 0x00;       
        buf[8]  = 0x00;
        buf[9]  = 0x00;
        buf[10] = 0x00;
        buf[11] = 0x00;       
        buf[12] = 0x00;       
        buf[13] = 0x00;

        buf[14] = databuf[12];
        buf[15] = databuf[13];
        buf[16] = 0x00;
        buf[17] = 0x03; 

        buf[68] = 0x00;
        buf[69] = 0x00;
        buf[70] = 0x68;
        netclient_send(buf,71);    
    }
}

int sendhisdata(char databuf[])
{
    int  datanum,dataindex,i;
    char data[RECORDLEN + 10];
    char buf[1451];
    char fname[] = {'.','.','/','d','s','m','/','1','1','\0'};
    fname[7] = databuf[12] + 0x30;
    fname[8] = databuf[13] + 0x30;

    dataindex = chartoi(databuf,4,4);
    printf("it is query history data cmd!the data number is : %d\n",dataindex);

    datanum =  fgethisdata(fname,dataindex,data);

    if(datanum > 0){
        for(i=0;i<RECORDLEN;i++){
            buf[i+18] = data[i];
        }
        buf[0]  = 0x68;
        buf[1]  = (datanum + 10 + 3) >> 8;       
        buf[2]  = (datanum + 10 + 3);       
        buf[3]  = 0x68;

        buf[4]  = 0x00;
        buf[5]  = 0x05;

        buf[6]  = 0x00;       
        buf[7]  = 0x00;       
        buf[8]  = 0x00;
        buf[9]  = 0x00;
        buf[10] = 0x00;
        buf[11] = 0x00;       
        buf[12] = 0x00;       
        buf[13] = 0x00;

        buf[14] = databuf[12];
        buf[15] = databuf[13];
        buf[16] = 0x00;
        buf[17] = 0x05; 

        buf[1448] = 0x00;
        buf[1449] = 0x00;
        buf[1450] = 0x68;
        
        netclient_send(buf,1451); 
        printf("send history data to server\n");   
    }
}

int sendsetinfo()
{
    int datanum,i;
    char fdata[120];
    char sdata[131];
    
    for(i=0;i<131;i++)
    {
        sdata[i] = 0;
    }
    
    datanum = fgetsetdata(fdata);
    if(datanum > 0)
    {
        for(i=0;i<110;i++){
            sdata[i+18] = fdata[i];
        }
        sdata[0]  = 0x68;
        sdata[1]  = 0x00;       
        sdata[2]  = 0x7C;       
        sdata[3]  = 0x68;

        sdata[4]  = 0x00;
        sdata[5]  = 0x03;

        sdata[6]  = 0x00;       
        sdata[7]  = 0x00;       
        sdata[8]  = 0x00;
        sdata[9]  = 0x00;
        sdata[10] = 0x00;
        sdata[11] = 0x00;       
        sdata[12] = 0x00;       
        sdata[13] = 0x00;

        sdata[14] = 0x01;
        sdata[15] = 0x01;
        sdata[16] = 0x00;
        sdata[17] = 0x04;

        sdata[128] = 0x00;
        sdata[129] = 0x00;
        sdata[130] = 0x68;
        netclient_send(sdata,131);    
    }
    else
        netclient_send(sdata,131);
}

int deal_setcolinfo(char databuf[])
{
    int  i;
    char wrbuf[111];

    for(i=0;i<110;i++)
    {
        wrbuf[i] = databuf[i + 19];
        printf("%d",wrbuf[i]);
    }  
  
    printf("\n");
    fsetsetdata(wrbuf);
}
