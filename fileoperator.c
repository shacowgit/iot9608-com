#include "fileoperator.h"

int chartoi(char buf[],int offset, int len)
{   
    int re=0;   // 保存转换为10进制的结果 
        
    if(len == 4)
    {
        re = (int)((buf[offset + 3] & 0xff)
             |((buf[offset + 2] & 0xff) << 8)
             |((buf[offset + 1] & 0xff) << 16)
             |((buf[offset] & 0xff) << 24));
    }
    else
    {
        re = (int)((buf[offset + 1] & 0xff)
             |((buf[offset] & 0xff) << 8));
    }

    return re;
}

double getrealvalue(char valuebuf[],int valueoff,int len,int fnum)
{
    int i =0;
    int ivalue;
    double dvalue;
    
    ivalue = chartoi(valuebuf,valueoff,len);
    dvalue = ivalue;

    for(i=0;i<fnum;i++)
    {
        dvalue = dvalue / 10;    
    }
}

int insertsavedatar(char savedata[],int saveoff, double value,int len,int fnum)
{
    int i = 0;
    int ivalue;
    char str[25];
    for(i=0;i<fnum;i++)
    {
        value = value * 10;
    }
    ivalue = (int)value;

    if(len == 4)
    {
        str[0] = (char)(ivalue >> 24);
        str[1] = (char)(ivalue >> 16);
        str[2] = (char)(ivalue >> 8);
        str[3] = (char)ivalue;
    }
    else
    {
        str[0] = (char)(ivalue >> 8);
        str[1] = (char)ivalue;
    }

    for(i=0;i<len;i++)
    {
        savedata[saveoff + i] = str[i]; 
    } 
    savedata[saveoff + len] = ',';
}

int insertsavedata(char valuebuf[],char savedata[],int valueoff,int saveoff,int num)
{
    int i = 0;
    for(i=0;i<num;i++)
    {
        savedata[saveoff + i] = valuebuf[valueoff + i];
    }
    savedata[saveoff + num] = ',';
}

int fgetrtdata(char fname[],char databuf[])   //get real time data
{
    int    fd;
    int    ldate;
    double dec;
    time_t timep;                 
    struct tm *p;
    char   rdbuf[12];
    int    savespan;

    fd = open(fname, O_RDWR);
    if(fd <= 0)                              //this file is not exist
    {
        close(fd);
        perror("get realtime data open file error");
        return -1;
    }

    time(&timep);                            //get now date
    lseek(fd,F_NEWIDDATE,SEEK_SET);
    read(fd, rdbuf, 4);
    ldate = chartoi(rdbuf, 0, 4);
    dec = difftime(timep,ldate);

    savespan = fgetsavespan();
    if(dec > savespan)
    {
        printf("realtime data overtime,%x\n",fname);   //realtime data overtime,there is no valid realtime data 
        close(fd);
        return 0;                                
    }
    else
    {
        lseek(fd, F_REALTIME, SEEK_SET);               //get realtime data
        read(fd, databuf, RECORDDATALEN);
        close(fd);
        return RECORDDATALEN;
    }

}

int fgethddata(char fname[],char databuf[])   //get real time data
{
    int    fd;
    fd = open(fname, O_RDWR);
    if(fd <= 0)                              //this file is not exist
    {
        close(fd);
        perror("get file  head info open file error");
        return -1;
    }
    lseek(fd, 0, SEEK_SET);                  //get file head
    read(fd, databuf, F_REALTIME);
    close(fd);
    return F_REALTIME;
}

int fgethisdata(char fname[],int index,char databuf[])   //get history data
{
    int    fd,offset;
    fd = open(fname, O_RDWR);
    if(fd <= 0)                              //this file is not exist
    {
        close(fd);
        perror("gei history data open file error");
        return -1;
    }
    offset = F_HISTORY + (index - 1) * RECORDLEN;
    lseek(fd, offset, SEEK_SET);
    read(fd, databuf, RECORDLEN);
    close(fd);
    return RECORDLEN;
}

int fgetsetdata(char databuf[])
{
    int    fd;
      
    fd = open("../dsm/set", O_RDWR);
    if(fd <= 0)                              //open file error
    {
        close(fd);
        perror("get set info  open file error");
        return -1;
    }

    lseek(fd, 0, SEEK_SET);                  //get file head
    read(fd, databuf, 110);
    close(fd);
    return 110;
}

int fsetsetdata(char databuf[])
{
    int    fd; 
   
    fd = open("../dsm/set", O_RDWR|O_CREAT|O_EXCL,777);  
    if(fd == -1)                                     //this file already exist
    {
        fd = open("../dsm/set", O_RDWR);
    } 
      
    if(fd <= 0)                              //open file error
    {
        close(fd);
        perror("set set info open file error");
        return -1;
    }
    lseek(fd, 0, SEEK_SET);
    write(fd, databuf, 110);
    close(fd);
    return 0;
}

int fgetcolname(char cname[])
{
    int    fd;
    fd = open("../dsm/set", O_RDWR);
    if(fd <= 0)                              //this file is not exist
    {
        close(fd);
        perror("get collector name open file error");
        return -1;
    }
    lseek(fd, 10, SEEK_SET);                  //get file head
    read(fd, cname, 10);
    close(fd);

    return 0;
}

int fgetworktype()
{
    int    fd;
    char   databuf[2];
    fd = open("../dsm/set", O_RDWR);
    if(fd <= 0)                              //this file is not exist
    {
        close(fd);
        perror("get work type open file error");
        return 1;
    }
    lseek(fd, 20, SEEK_SET);                  //get file head
    read(fd, databuf, 2);
    close(fd);

    return (int)databuf[1];
}

int fgetsendspan()
{
    int    fd;
    char   databuf[2];
    fd = open("../dsm/set", O_RDWR);
    if(fd <= 0)                              //this file is not exist
    {
        close(fd);
        perror("get sendspan open file error");
        return -1;
    }
    lseek(fd, 22, SEEK_SET);                  //get file head
    read(fd, databuf, 2);
    close(fd);

    return chartoi(databuf,0,2);
}

int fgetsavespan()
{
    int    fd;
    char   databuf[2];
    fd = open("../dsm/set", O_RDWR);
    if(fd <= 0)                              //this file is not exist
    {
        close(fd);
        perror("get save span open file error");
        return -1;
    }
    lseek(fd, 24, SEEK_SET);                  //get file head
    read(fd, databuf, 2);
    close(fd);

    return chartoi(databuf,0,2);
}

int wrfile(char fname[],char databuf[])     //write a data into file
{
    int    fd;
    char   wrbuf[12];
    char   rdbuf[12];
    int    index,total,offset;
    int    year,mon,day,hour,min;
    int    ldate;
    double dec;

    time_t timep;                 //get now date
    struct tm *p;
    time(&timep);
    p=gmtime(&timep);
    year = p->tm_year + 1900;
    mon = p->tm_mon + 1;
    day = p->tm_mday;
    hour = p->tm_hour;
    min = p->tm_min;
    
    fd = open(fname, O_RDWR|O_CREAT|O_EXCL,777);  
    if(fd == -1)                         //this file already exist
    {
        fd = open(fname, O_RDWR);
        lseek(fd,F_NEWIDDATE,SEEK_SET);
        read(fd, rdbuf, 4);
        ldate = chartoi(rdbuf, 0, 4);
        dec = difftime(timep,ldate);
    }  
    else                   //init the file header
    {
        lseek(fd, F_ELECNAME, SEEK_SET);
        write(fd, fname, 2);

        lseek(fd, F_NEWID, SEEK_SET);
        wrbuf[0] = 0;
        wrbuf[1] = 0;
        wrbuf[2] = 0;
        wrbuf[3] = 1;
        write(fd, wrbuf, 4);

        lseek(fd, F_TOTALNUMBER, SEEK_SET);
        wrbuf[0] = 0;
        wrbuf[1] = 0;
        wrbuf[2] = 0;
        wrbuf[3] = 1;
        write(fd, wrbuf, 4);
        
        dec = 30;
        index = 1;
        total = 1;
    }

    lseek(fd, F_REALTIME, SEEK_SET);                 //update realtime data
    write(fd, databuf, RECORDDATALEN);

    if(dec >= 30)
    {
        lseek(fd, F_NEWID, SEEK_SET);         // refresh the write index
        read(fd, rdbuf, 4); 
        index = chartoi(rdbuf, 0, 4); 
        if(index == TOTALCNT)
            index = 1;
        else
            index = index + 1;
        wrbuf[0] = (char)(index >> 24);
        wrbuf[1] = (char)(index >> 16);
        wrbuf[2] = (char)(index >> 8);
        wrbuf[3] = (char)index;
        lseek(fd, F_NEWID, SEEK_SET);
        write(fd, wrbuf, 4);

        lseek(fd, F_TOTALNUMBER, SEEK_SET);   // refresh the total number
        read(fd, rdbuf, 4); 
        total = chartoi(rdbuf, 0, 4); 
        if(total != TOTALCNT)
        {
            total = total + 1;
            wrbuf[0] = (char)(total >> 24);
            wrbuf[1] = (char)(total >> 16);
            wrbuf[2] = (char)(total >> 8);
            wrbuf[3] = (char)total;
            lseek(fd, F_TOTALNUMBER, SEEK_SET);
            write(fd, wrbuf, 4);
        }
        
        wrbuf[0] = (char)(year >> 24);                //write data time
        wrbuf[1] = (char)(year >> 16);
        wrbuf[2] = (char)(year >> 8);
        wrbuf[3] = (char)year;
        wrbuf[4] = (char)(mon >> 8);
        wrbuf[5] = (char)mon;
        wrbuf[6] = (char)(day >> 8);
        wrbuf[7] = (char)day;
        wrbuf[8] = (char)(hour >> 8);
        wrbuf[9] = (char)hour;
        wrbuf[10] = (char)(min >> 8);
        wrbuf[11] = (char)min;
        offset = F_HISTORY + (index - 1) * RECORDLEN;
        lseek(fd, offset, SEEK_SET);
        write(fd, wrbuf, RECORDTIMELEN);

        offset = offset + 12;                  //write data
        lseek(fd, offset, SEEK_SET);
        write(fd, databuf, RECORDDATALEN);

        lseek(fd,F_NEWIDDATE,SEEK_SET);         //update newid date
        wrbuf[0] = (char)(timep >> 24);                
        wrbuf[1] = (char)(timep >> 16);
        wrbuf[2] = (char)(timep >> 8);
        wrbuf[3] = (char)timep;
        write(fd, wrbuf, 4);      
    }
    close(fd);
}

int savedatainit(char savedata[])
{
    insertsavedatar(savedata,V_A,0,4,0);  //A相电压 
    insertsavedatar(savedata,V_B,0,4,0);  //B相电压 
    insertsavedatar(savedata,V_C,0,4,0);  //C相电压 
    insertsavedatar(savedata,C_A,0,4,0);  //A相电流 
    insertsavedatar(savedata,C_B,0,4,0);  //B相电流 
    insertsavedatar(savedata,C_C,0,4,0);  //C相电流 
    insertsavedatar(savedata,Act_powerA,0,4,0);  //A相有功
    insertsavedatar(savedata,Act_powerB,0,4,0);  //B相有功
    insertsavedatar(savedata,Act_powerC,0,4,0);  //C相有功
    insertsavedatar(savedata,Act_powerT,0,4,0);  //总有功
    insertsavedatar(savedata,Rea_powerA,0,4,0);  //A相无功
    insertsavedatar(savedata,Rea_powerB,0,4,0);  //B相无功
    insertsavedatar(savedata,Rea_powerC,0,4,0);  //C相无功
    insertsavedatar(savedata,Rea_powerT,0,4,0);  //总无功
    insertsavedatar(savedata,App_powerA,0,4,0);  //A相视在
    insertsavedatar(savedata,App_powerB,0,4,0);  //B相视在
    insertsavedatar(savedata,App_powerC,0,4,0);  //C相视在
    insertsavedatar(savedata,App_powerT,0,4,0);  //总视在
    insertsavedatar(savedata,powerfac_A,0,2,0);  //A相PF
    insertsavedatar(savedata,powerfac_B,0,2,0);  //B相PF
    insertsavedatar(savedata,powerfac_C,0,2,0);  //C相PF
    insertsavedatar(savedata,powerfac_T,0,2,0);  //总PF
    insertsavedatar(savedata,frequency,0,2,0);   //总PF
    insertsavedatar(savedata,V_fundA,0,4,0);  //A相基波电压 
    insertsavedatar(savedata,V_fundB,0,4,0);  //B相基波电压 
    insertsavedatar(savedata,V_fundC,0,4,0);  //C相基波电压 
    insertsavedatar(savedata,C_fundA,0,4,0);  //A相基波电流 
    insertsavedatar(savedata,C_fundB,0,4,0);  //B相基波电流 
    insertsavedatar(savedata,C_fundC,0,4,0);  //C相基波电流

    insertsavedatar(savedata,V_har_coA2,0,2,0);   //A相谐波电压含有率（2）
    insertsavedatar(savedata,V_har_coA3,0,2,0);   //A相谐波电压含有率（3）
    insertsavedatar(savedata,V_har_coA4,0,2,0);   //A相谐波电压含有率（4）
    insertsavedatar(savedata,V_har_coA5,0,2,0);   //A相谐波电压含有率（5）
    insertsavedatar(savedata,V_har_coA6,0,2,0);   //A相谐波电压含有率（6）
    insertsavedatar(savedata,V_har_coA7,0,2,0);   //A相谐波电压含有率（7）
    insertsavedatar(savedata,V_har_coA8,0,2,0);   //A相谐波电压含有率（8）
    insertsavedatar(savedata,V_har_coA9,0,2,0);   //A相谐波电压含有率（9）
    insertsavedatar(savedata,V_har_coA10,0,2,0);  //A相谐波电压含有率（10）
    insertsavedatar(savedata,V_har_coA11,0,2,0);  //A相谐波电压含有率（11）
    insertsavedatar(savedata,V_har_coA12,0,2,0);  //A相谐波电压含有率（12）
    insertsavedatar(savedata,V_har_coA13,0,2,0);  //A相谐波电压含有率（13）
    insertsavedatar(savedata,V_har_coA14,0,2,0);  //A相谐波电压含有率（14）
    insertsavedatar(savedata,V_har_coA15,0,2,0);  //A相谐波电压含有率（15）
    insertsavedatar(savedata,V_har_coA16,0,2,0);  //A相谐波电压含有率（16）
    insertsavedatar(savedata,V_har_coA17,0,2,0);  //A相谐波电压含有率（17）
    insertsavedatar(savedata,V_har_coA18,0,2,0);  //A相谐波电压含有率（18）
    insertsavedatar(savedata,V_har_coA19,0,2,0);  //A相谐波电压含有率（19）
    insertsavedatar(savedata,V_har_coA20,0,2,0);  //A相谐波电压含有率（20）
    insertsavedatar(savedata,V_har_coA21,0,2,0);  //A相谐波电压含有率（21）
    insertsavedatar(savedata,V_har_coA22,0,2,0);  //A相谐波电压含有率（22）
    insertsavedatar(savedata,V_har_coA23,0,2,0);  //A相谐波电压含有率（23）
    insertsavedatar(savedata,V_har_coA24,0,2,0);  //A相谐波电压含有率（24）
    insertsavedatar(savedata,V_har_coA25,0,2,0);  //A相谐波电压含有率（25）
    insertsavedatar(savedata,V_har_coA26,0,2,0);  //A相谐波电压含有率（26）
    insertsavedatar(savedata,V_har_coA27,0,2,0);  //A相谐波电压含有率（27）
    insertsavedatar(savedata,V_har_coA28,0,2,0);  //A相谐波电压含有率（28）
    insertsavedatar(savedata,V_har_coA29,0,2,0);  //A相谐波电压含有率（29）
    insertsavedatar(savedata,V_har_coA30,0,2,0);  //A相谐波电压含有率（30）
    insertsavedatar(savedata,V_har_coA31,0,2,0);  //A相谐波电压含有率（31）
    insertsavedatar(savedata,V_har_coA32,0,2,0);  //A相谐波电压含有率（32）
    insertsavedatar(savedata,V_har_coA33,0,2,0);  //A相谐波电压含有率（33）
    insertsavedatar(savedata,V_har_coA34,0,2,0);  //A相谐波电压含有率（34）
    insertsavedatar(savedata,V_har_coA35,0,2,0);  //A相谐波电压含有率（35）
    insertsavedatar(savedata,V_har_coA36,0,2,0);  //A相谐波电压含有率（36）
    insertsavedatar(savedata,V_har_coA37,0,2,0);  //A相谐波电压含有率（37）
    insertsavedatar(savedata,V_har_coA38,0,2,0);  //A相谐波电压含有率（38）
    insertsavedatar(savedata,V_har_coA39,0,2,0);  //A相谐波电压含有率（39）
    insertsavedatar(savedata,V_har_coA40,0,2,0);  //A相谐波电压含有率（40）
    insertsavedatar(savedata,V_har_coA41,0,2,0);  //A相谐波电压含有率（41）
    insertsavedatar(savedata,V_har_coA42,0,2,0);  //A相谐波电压含有率（42）
    insertsavedatar(savedata,V_har_coA43,0,2,0);  //A相谐波电压含有率（43）
    insertsavedatar(savedata,V_har_coA44,0,2,0);  //A相谐波电压含有率（44）
    insertsavedatar(savedata,V_har_coA45,0,2,0);  //A相谐波电压含有率（45）
    insertsavedatar(savedata,V_har_coA46,0,2,0);  //A相谐波电压含有率（46）
    insertsavedatar(savedata,V_har_coA47,0,2,0);  //A相谐波电压含有率（47）
    insertsavedatar(savedata,V_har_coA48,0,2,0);  //A相谐波电压含有率（48）
    insertsavedatar(savedata,V_har_coA49,0,2,0);  //A相谐波电压含有率（49）
    insertsavedatar(savedata,V_har_coA50,0,2,0);  //A相谐波电压含有率（50）

    insertsavedatar(savedata,V_har_coB2,0,2,0);   //B相谐波电压含有率（2）
    insertsavedatar(savedata,V_har_coB3,0,2,0);   //B相谐波电压含有率（3）
    insertsavedatar(savedata,V_har_coB4,0,2,0);   //B相谐波电压含有率（4）
    insertsavedatar(savedata,V_har_coB5,0,2,0);   //B相谐波电压含有率（5）
    insertsavedatar(savedata,V_har_coB6,0,2,0);   //B相谐波电压含有率（6）
    insertsavedatar(savedata,V_har_coB7,0,2,0);   //B相谐波电压含有率（7）
    insertsavedatar(savedata,V_har_coB8,0,2,0);   //B相谐波电压含有率（8）
    insertsavedatar(savedata,V_har_coB9,0,2,0);   //B相谐波电压含有率（9）
    insertsavedatar(savedata,V_har_coB10,0,2,0);  //B相谐波电压含有率（10）
    insertsavedatar(savedata,V_har_coB11,0,2,0);  //B相谐波电压含有率（11）
    insertsavedatar(savedata,V_har_coB12,0,2,0);  //B相谐波电压含有率（12）
    insertsavedatar(savedata,V_har_coB13,0,2,0);  //B相谐波电压含有率（13）
    insertsavedatar(savedata,V_har_coB14,0,2,0);  //B相谐波电压含有率（14）
    insertsavedatar(savedata,V_har_coB15,0,2,0);  //B相谐波电压含有率（15）
    insertsavedatar(savedata,V_har_coB16,0,2,0);  //B相谐波电压含有率（16）
    insertsavedatar(savedata,V_har_coB17,0,2,0);  //B相谐波电压含有率（17）
    insertsavedatar(savedata,V_har_coB18,0,2,0);  //B相谐波电压含有率（18）
    insertsavedatar(savedata,V_har_coB19,0,2,0);  //B相谐波电压含有率（19）
    insertsavedatar(savedata,V_har_coB20,0,2,0);  //B相谐波电压含有率（20）
    insertsavedatar(savedata,V_har_coB21,0,2,0);  //B相谐波电压含有率（21）
    insertsavedatar(savedata,V_har_coB22,0,2,0);  //B相谐波电压含有率（22）
    insertsavedatar(savedata,V_har_coB23,0,2,0);  //B相谐波电压含有率（23）
    insertsavedatar(savedata,V_har_coB24,0,2,0);  //B相谐波电压含有率（24）
    insertsavedatar(savedata,V_har_coB25,0,2,0);  //B相谐波电压含有率（25）
    insertsavedatar(savedata,V_har_coB26,0,2,0);  //B相谐波电压含有率（26）
    insertsavedatar(savedata,V_har_coB27,0,2,0);  //B相谐波电压含有率（27）
    insertsavedatar(savedata,V_har_coB28,0,2,0);  //B相谐波电压含有率（28）
    insertsavedatar(savedata,V_har_coB29,0,2,0);  //B相谐波电压含有率（29）
    insertsavedatar(savedata,V_har_coB30,0,2,0);  //B相谐波电压含有率（30）
    insertsavedatar(savedata,V_har_coB31,0,2,0);  //B相谐波电压含有率（31）
    insertsavedatar(savedata,V_har_coB32,0,2,0);  //B相谐波电压含有率（32）
    insertsavedatar(savedata,V_har_coB33,0,2,0);  //B相谐波电压含有率（33）
    insertsavedatar(savedata,V_har_coB34,0,2,0);  //B相谐波电压含有率（34）
    insertsavedatar(savedata,V_har_coB35,0,2,0);  //B相谐波电压含有率（35）
    insertsavedatar(savedata,V_har_coB36,0,2,0);  //B相谐波电压含有率（36）
    insertsavedatar(savedata,V_har_coB37,0,2,0);  //B相谐波电压含有率（37）
    insertsavedatar(savedata,V_har_coB38,0,2,0);  //B相谐波电压含有率（38）
    insertsavedatar(savedata,V_har_coB39,0,2,0);  //B相谐波电压含有率（39）
    insertsavedatar(savedata,V_har_coB40,0,2,0);  //B相谐波电压含有率（40）
    insertsavedatar(savedata,V_har_coB41,0,2,0);  //B相谐波电压含有率（41）
    insertsavedatar(savedata,V_har_coB42,0,2,0);  //B相谐波电压含有率（42）
    insertsavedatar(savedata,V_har_coB43,0,2,0);  //B相谐波电压含有率（43）
    insertsavedatar(savedata,V_har_coB44,0,2,0);  //B相谐波电压含有率（44）
    insertsavedatar(savedata,V_har_coB45,0,2,0);  //B相谐波电压含有率（45）
    insertsavedatar(savedata,V_har_coB46,0,2,0);  //B相谐波电压含有率（46）
    insertsavedatar(savedata,V_har_coB47,0,2,0);  //B相谐波电压含有率（47）
    insertsavedatar(savedata,V_har_coB48,0,2,0);  //B相谐波电压含有率（48）
    insertsavedatar(savedata,V_har_coB49,0,2,0);  //B相谐波电压含有率（49）
    insertsavedatar(savedata,V_har_coB50,0,2,0);  //B相谐波电压含有率（50）

    insertsavedatar(savedata,V_har_coC2,0,2,0);   //C相谐波电压含有率（2）
    insertsavedatar(savedata,V_har_coC3,0,2,0);   //C相谐波电压含有率（3）
    insertsavedatar(savedata,V_har_coC4,0,2,0);   //C相谐波电压含有率（4）
    insertsavedatar(savedata,V_har_coC5,0,2,0);   //C相谐波电压含有率（5）
    insertsavedatar(savedata,V_har_coC6,0,2,0);   //C相谐波电压含有率（6）
    insertsavedatar(savedata,V_har_coC7,0,2,0);   //C相谐波电压含有率（7）
    insertsavedatar(savedata,V_har_coC8,0,2,0);   //C相谐波电压含有率（8）
    insertsavedatar(savedata,V_har_coC9,0,2,0);   //C相谐波电压含有率（9）
    insertsavedatar(savedata,V_har_coC10,0,2,0);  //C相谐波电压含有率（10）
    insertsavedatar(savedata,V_har_coC11,0,2,0);  //C相谐波电压含有率（11）
    insertsavedatar(savedata,V_har_coC12,0,2,0);  //C相谐波电压含有率（12）
    insertsavedatar(savedata,V_har_coC13,0,2,0);  //C相谐波电压含有率（13）
    insertsavedatar(savedata,V_har_coC14,0,2,0);  //C相谐波电压含有率（14）
    insertsavedatar(savedata,V_har_coC15,0,2,0);  //C相谐波电压含有率（15）
    insertsavedatar(savedata,V_har_coC16,0,2,0);  //C相谐波电压含有率（16）
    insertsavedatar(savedata,V_har_coC17,0,2,0);  //C相谐波电压含有率（17）
    insertsavedatar(savedata,V_har_coC18,0,2,0);  //C相谐波电压含有率（18）
    insertsavedatar(savedata,V_har_coC19,0,2,0);  //C相谐波电压含有率（19）
    insertsavedatar(savedata,V_har_coC20,0,2,0);  //C相谐波电压含有率（20）
    insertsavedatar(savedata,V_har_coC21,0,2,0);  //C相谐波电压含有率（21）
    insertsavedatar(savedata,V_har_coC22,0,2,0);  //C相谐波电压含有率（22）
    insertsavedatar(savedata,V_har_coC23,0,2,0);  //C相谐波电压含有率（23）
    insertsavedatar(savedata,V_har_coC24,0,2,0);  //C相谐波电压含有率（24）
    insertsavedatar(savedata,V_har_coC25,0,2,0);  //C相谐波电压含有率（25）
    insertsavedatar(savedata,V_har_coC26,0,2,0);  //C相谐波电压含有率（26）
    insertsavedatar(savedata,V_har_coC27,0,2,0);  //C相谐波电压含有率（27）
    insertsavedatar(savedata,V_har_coC28,0,2,0);  //C相谐波电压含有率（28）
    insertsavedatar(savedata,V_har_coC29,0,2,0);  //C相谐波电压含有率（29）
    insertsavedatar(savedata,V_har_coC30,0,2,0);  //C相谐波电压含有率（30）
    insertsavedatar(savedata,V_har_coC31,0,2,0);  //C相谐波电压含有率（31）
    insertsavedatar(savedata,V_har_coC32,0,2,0);  //C相谐波电压含有率（32）
    insertsavedatar(savedata,V_har_coC33,0,2,0);  //C相谐波电压含有率（33）
    insertsavedatar(savedata,V_har_coC34,0,2,0);  //C相谐波电压含有率（34）
    insertsavedatar(savedata,V_har_coC35,0,2,0);  //C相谐波电压含有率（35）
    insertsavedatar(savedata,V_har_coC36,0,2,0);  //C相谐波电压含有率（36）
    insertsavedatar(savedata,V_har_coC37,0,2,0);  //C相谐波电压含有率（37）
    insertsavedatar(savedata,V_har_coC38,0,2,0);  //C相谐波电压含有率（38）
    insertsavedatar(savedata,V_har_coC39,0,2,0);  //C相谐波电压含有率（39）
    insertsavedatar(savedata,V_har_coC40,0,2,0);  //C相谐波电压含有率（40）
    insertsavedatar(savedata,V_har_coC41,0,2,0);  //C相谐波电压含有率（41）
    insertsavedatar(savedata,V_har_coC42,0,2,0);  //C相谐波电压含有率（42）
    insertsavedatar(savedata,V_har_coC43,0,2,0);  //C相谐波电压含有率（43）
    insertsavedatar(savedata,V_har_coC44,0,2,0);  //C相谐波电压含有率（44）
    insertsavedatar(savedata,V_har_coC45,0,2,0);  //C相谐波电压含有率（45）
    insertsavedatar(savedata,V_har_coC46,0,2,0);  //C相谐波电压含有率（46）
    insertsavedatar(savedata,V_har_coC47,0,2,0);  //C相谐波电压含有率（47）
    insertsavedatar(savedata,V_har_coC48,0,2,0);  //C相谐波电压含有率（48）
    insertsavedatar(savedata,V_har_coC49,0,2,0);  //C相谐波电压含有率（49）
    insertsavedatar(savedata,V_har_coC50,0,2,0);  //C相谐波电压含有率（50）

    insertsavedatar(savedata,C_harA2,0,4,0);       //A相谐波电流（2）
    insertsavedatar(savedata,C_harA3,0,4,0);       //A相谐波电流（3）
    insertsavedatar(savedata,C_harA4,0,4,0);       //A相谐波电流（4）
    insertsavedatar(savedata,C_harA5,0,4,0);       //A相谐波电流（5）
    insertsavedatar(savedata,C_harA6,0,4,0);       //A相谐波电流（6）
    insertsavedatar(savedata,C_harA7,0,4,0);       //A相谐波电流（7）
    insertsavedatar(savedata,C_harA8,0,4,0);       //A相谐波电流（8）
    insertsavedatar(savedata,C_harA9,0,4,0);       //A相谐波电流（9）
    insertsavedatar(savedata,C_harA10,0,4,0);      //A相谐波电流（10）
    insertsavedatar(savedata,C_harA11,0,4,0);      //A相谐波电流（11）
    insertsavedatar(savedata,C_harA12,0,4,0);      //A相谐波电流（12）
    insertsavedatar(savedata,C_harA13,0,4,0);      //A相谐波电流（13）
    insertsavedatar(savedata,C_harA14,0,4,0);      //A相谐波电流（14）
    insertsavedatar(savedata,C_harA15,0,4,0);      //A相谐波电流（15）
    insertsavedatar(savedata,C_harA16,0,4,0);      //A相谐波电流（16）
    insertsavedatar(savedata,C_harA17,0,4,0);      //A相谐波电流（17）
    insertsavedatar(savedata,C_harA18,0,4,0);      //A相谐波电流（18）
    insertsavedatar(savedata,C_harA19,0,4,0);      //A相谐波电流（19）
    insertsavedatar(savedata,C_harA20,0,4,0);      //A相谐波电流（20）
    insertsavedatar(savedata,C_harA21,0,4,0);      //A相谐波电流（21）
    insertsavedatar(savedata,C_harA22,0,4,0);      //A相谐波电流（22）
    insertsavedatar(savedata,C_harA23,0,4,0);      //A相谐波电流（23）
    insertsavedatar(savedata,C_harA24,0,4,0);      //A相谐波电流（24）
    insertsavedatar(savedata,C_harA25,0,4,0);      //A相谐波电流（25）
    insertsavedatar(savedata,C_harA26,0,4,0);      //A相谐波电流（26）
    insertsavedatar(savedata,C_harA27,0,4,0);      //A相谐波电流（27）
    insertsavedatar(savedata,C_harA28,0,4,0);      //A相谐波电流（28）
    insertsavedatar(savedata,C_harA29,0,4,0);      //A相谐波电流（29）
    insertsavedatar(savedata,C_harA30,0,4,0);      //A相谐波电流（30）
    insertsavedatar(savedata,C_harA31,0,4,0);      //A相谐波电流（31）
    insertsavedatar(savedata,C_harA32,0,4,0);      //A相谐波电流（32）
    insertsavedatar(savedata,C_harA33,0,4,0);      //A相谐波电流（33）
    insertsavedatar(savedata,C_harA34,0,4,0);      //A相谐波电流（34）
    insertsavedatar(savedata,C_harA35,0,4,0);      //A相谐波电流（35）
    insertsavedatar(savedata,C_harA36,0,4,0);      //A相谐波电流（36）
    insertsavedatar(savedata,C_harA37,0,4,0);      //A相谐波电流（37）
    insertsavedatar(savedata,C_harA38,0,4,0);      //A相谐波电流（38）
    insertsavedatar(savedata,C_harA39,0,4,0);      //A相谐波电流（39）
    insertsavedatar(savedata,C_harA40,0,4,0);      //A相谐波电流（40）
    insertsavedatar(savedata,C_harA41,0,4,0);      //A相谐波电流（41）
    insertsavedatar(savedata,C_harA42,0,4,0);      //A相谐波电流（42）
    insertsavedatar(savedata,C_harA43,0,4,0);      //A相谐波电流（43）
    insertsavedatar(savedata,C_harA44,0,4,0);      //A相谐波电流（44）
    insertsavedatar(savedata,C_harA45,0,4,0);      //A相谐波电流（45）
    insertsavedatar(savedata,C_harA46,0,4,0);      //A相谐波电流（46）
    insertsavedatar(savedata,C_harA47,0,4,0);      //A相谐波电流（47）
    insertsavedatar(savedata,C_harA48,0,4,0);      //A相谐波电流（48）
    insertsavedatar(savedata,C_harA49,0,4,0);      //A相谐波电流（49）
    insertsavedatar(savedata,C_harA50,0,4,0);      //A相谐波电流（50）

    insertsavedatar(savedata,C_harB2,0,4,0);       //B相谐波电流（2）
    insertsavedatar(savedata,C_harB3,0,4,0);       //B相谐波电流（3）
    insertsavedatar(savedata,C_harB4,0,4,0);       //B相谐波电流（4）
    insertsavedatar(savedata,C_harB5,0,4,0);       //B相谐波电流（5）
    insertsavedatar(savedata,C_harB6,0,4,0);       //B相谐波电流（6）
    insertsavedatar(savedata,C_harB7,0,4,0);       //B相谐波电流（7）
    insertsavedatar(savedata,C_harB8,0,4,0);       //B相谐波电流（8）
    insertsavedatar(savedata,C_harB9,0,4,0);       //B相谐波电流（9）
    insertsavedatar(savedata,C_harB10,0,4,0);      //B相谐波电流（10）
    insertsavedatar(savedata,C_harB11,0,4,0);      //B相谐波电流（11）
    insertsavedatar(savedata,C_harB12,0,4,0);      //B相谐波电流（12）
    insertsavedatar(savedata,C_harB13,0,4,0);      //B相谐波电流（13）
    insertsavedatar(savedata,C_harB14,0,4,0);      //B相谐波电流（14）
    insertsavedatar(savedata,C_harB15,0,4,0);      //B相谐波电流（15）
    insertsavedatar(savedata,C_harB16,0,4,0);      //B相谐波电流（16）
    insertsavedatar(savedata,C_harB17,0,4,0);      //B相谐波电流（17）
    insertsavedatar(savedata,C_harB18,0,4,0);      //B相谐波电流（18）
    insertsavedatar(savedata,C_harB19,0,4,0);      //B相谐波电流（19）
    insertsavedatar(savedata,C_harB20,0,4,0);      //B相谐波电流（20）
    insertsavedatar(savedata,C_harB21,0,4,0);      //B相谐波电流（21）
    insertsavedatar(savedata,C_harB22,0,4,0);      //B相谐波电流（22）
    insertsavedatar(savedata,C_harB23,0,4,0);      //B相谐波电流（23）
    insertsavedatar(savedata,C_harB24,0,4,0);      //B相谐波电流（24）
    insertsavedatar(savedata,C_harB25,0,4,0);      //B相谐波电流（25）
    insertsavedatar(savedata,C_harB26,0,4,0);      //B相谐波电流（26）
    insertsavedatar(savedata,C_harB27,0,4,0);      //B相谐波电流（27）
    insertsavedatar(savedata,C_harB28,0,4,0);      //B相谐波电流（28）
    insertsavedatar(savedata,C_harB29,0,4,0);      //B相谐波电流（29）
    insertsavedatar(savedata,C_harB30,0,4,0);      //B相谐波电流（30）
    insertsavedatar(savedata,C_harB31,0,4,0);      //B相谐波电流（31）
    insertsavedatar(savedata,C_harB32,0,4,0);      //B相谐波电流（32）
    insertsavedatar(savedata,C_harB33,0,4,0);      //B相谐波电流（33）
    insertsavedatar(savedata,C_harB34,0,4,0);      //B相谐波电流（34）
    insertsavedatar(savedata,C_harB35,0,4,0);      //B相谐波电流（35）
    insertsavedatar(savedata,C_harB36,0,4,0);      //B相谐波电流（36）
    insertsavedatar(savedata,C_harB37,0,4,0);      //B相谐波电流（37）
    insertsavedatar(savedata,C_harB38,0,4,0);      //B相谐波电流（38）
    insertsavedatar(savedata,C_harB39,0,4,0);      //B相谐波电流（39）
    insertsavedatar(savedata,C_harB40,0,4,0);      //B相谐波电流（40）
    insertsavedatar(savedata,C_harB41,0,4,0);      //B相谐波电流（41）
    insertsavedatar(savedata,C_harB42,0,4,0);      //B相谐波电流（42）
    insertsavedatar(savedata,C_harB43,0,4,0);      //B相谐波电流（43）
    insertsavedatar(savedata,C_harB44,0,4,0);      //B相谐波电流（44）
    insertsavedatar(savedata,C_harB45,0,4,0);      //B相谐波电流（45）
    insertsavedatar(savedata,C_harB46,0,4,0);      //B相谐波电流（46）
    insertsavedatar(savedata,C_harB47,0,4,0);      //B相谐波电流（47）
    insertsavedatar(savedata,C_harB48,0,4,0);      //B相谐波电流（48）
    insertsavedatar(savedata,C_harB49,0,4,0);      //B相谐波电流（49）
    insertsavedatar(savedata,C_harB50,0,4,0);      //B相谐波电流（50）

    insertsavedatar(savedata,C_harC2,0,4,0);       //C相谐波电流（2）
    insertsavedatar(savedata,C_harC3,0,4,0);       //C相谐波电流（3）
    insertsavedatar(savedata,C_harC4,0,4,0);       //C相谐波电流（4）
    insertsavedatar(savedata,C_harC5,0,4,0);       //C相谐波电流（5）
    insertsavedatar(savedata,C_harC6,0,4,0);       //C相谐波电流（6）
    insertsavedatar(savedata,C_harC7,0,4,0);       //C相谐波电流（7）
    insertsavedatar(savedata,C_harC8,0,4,0);       //C相谐波电流（8）
    insertsavedatar(savedata,C_harC9,0,4,0);       //C相谐波电流（9）
    insertsavedatar(savedata,C_harC10,0,4,0);      //C相谐波电流（10）
    insertsavedatar(savedata,C_harC11,0,4,0);      //C相谐波电流（11）
    insertsavedatar(savedata,C_harC12,0,4,0);      //C相谐波电流（12）
    insertsavedatar(savedata,C_harC13,0,4,0);      //C相谐波电流（13）
    insertsavedatar(savedata,C_harC14,0,4,0);      //C相谐波电流（14）
    insertsavedatar(savedata,C_harC15,0,4,0);      //C相谐波电流（15）
    insertsavedatar(savedata,C_harC16,0,4,0);      //C相谐波电流（16）
    insertsavedatar(savedata,C_harC17,0,4,0);      //C相谐波电流（17）
    insertsavedatar(savedata,C_harC18,0,4,0);      //C相谐波电流（18）
    insertsavedatar(savedata,C_harC19,0,4,0);      //C相谐波电流（19）
    insertsavedatar(savedata,C_harC20,0,4,0);      //C相谐波电流（20）
    insertsavedatar(savedata,C_harC21,0,4,0);      //C相谐波电流（21）
    insertsavedatar(savedata,C_harC22,0,4,0);      //C相谐波电流（22）
    insertsavedatar(savedata,C_harC23,0,4,0);      //C相谐波电流（23）
    insertsavedatar(savedata,C_harC24,0,4,0);      //C相谐波电流（24）
    insertsavedatar(savedata,C_harC25,0,4,0);      //C相谐波电流（25）
    insertsavedatar(savedata,C_harC26,0,4,0);      //C相谐波电流（26）
    insertsavedatar(savedata,C_harC27,0,4,0);      //C相谐波电流（27）
    insertsavedatar(savedata,C_harC28,0,4,0);      //C相谐波电流（28）
    insertsavedatar(savedata,C_harC29,0,4,0);      //C相谐波电流（29）
    insertsavedatar(savedata,C_harC30,0,4,0);      //C相谐波电流（30）
    insertsavedatar(savedata,C_harC31,0,4,0);      //C相谐波电流（31）
    insertsavedatar(savedata,C_harC32,0,4,0);      //C相谐波电流（32）
    insertsavedatar(savedata,C_harC33,0,4,0);      //C相谐波电流（33）
    insertsavedatar(savedata,C_harC34,0,4,0);      //C相谐波电流（34）
    insertsavedatar(savedata,C_harC35,0,4,0);      //C相谐波电流（35）
    insertsavedatar(savedata,C_harC36,0,4,0);      //C相谐波电流（36）
    insertsavedatar(savedata,C_harC37,0,4,0);      //C相谐波电流（37）
    insertsavedatar(savedata,C_harC38,0,4,0);      //C相谐波电流（38）
    insertsavedatar(savedata,C_harC39,0,4,0);      //C相谐波电流（39）
    insertsavedatar(savedata,C_harC40,0,4,0);      //C相谐波电流（40）
    insertsavedatar(savedata,C_harC41,0,4,0);      //C相谐波电流（41）
    insertsavedatar(savedata,C_harC42,0,4,0);      //C相谐波电流（42）
    insertsavedatar(savedata,C_harC43,0,4,0);      //C相谐波电流（43）
    insertsavedatar(savedata,C_harC44,0,4,0);      //C相谐波电流（44）
    insertsavedatar(savedata,C_harC45,0,4,0);      //C相谐波电流（45）
    insertsavedatar(savedata,C_harC46,0,4,0);      //C相谐波电流（46）
    insertsavedatar(savedata,C_harC47,0,4,0);      //C相谐波电流（47）
    insertsavedatar(savedata,C_harC48,0,4,0);      //C相谐波电流（48）
    insertsavedatar(savedata,C_harC49,0,4,0);      //C相谐波电流（49）
    insertsavedatar(savedata,C_harC50,0,4,0);      //C相谐波电流（50）

    insertsavedatar(savedata,V_wav_disA,0,2,0);      //A相电压谐波畸变率
    insertsavedatar(savedata,V_wav_disB,0,2,0);      //B相电压谐波畸变率
    insertsavedatar(savedata,V_wav_disC,0,2,0);      //C相电压谐波畸变率
    insertsavedatar(savedata,C_wav_disA,0,2,0);      //A相电流谐波畸变率
    insertsavedatar(savedata,C_wav_disB,0,2,0);      //B相电流谐波畸变率
    insertsavedatar(savedata,C_wav_disC,0,2,0);      //C相电流谐波畸变率

    insertsavedatar(savedata,P_act_powA,0,4,0);      //A相正向有功总电能
    insertsavedatar(savedata,P_act_powB,0,4,0);      //B相正向有功总电能
    insertsavedatar(savedata,P_act_powC,0,4,0);      //C相正向有功总电能
    insertsavedatar(savedata,P_act_powT,0,4,0);      //总正向有功总电能
    insertsavedatar(savedata,P_rea_powA,0,4,0);      //A相正向无功总电能
    insertsavedatar(savedata,P_rea_powB,0,4,0);      //B相正向无功总电能
    insertsavedatar(savedata,P_rea_powC,0,4,0);      //C相正向无功总电能
    insertsavedatar(savedata,P_rea_powT,0,4,0);      //总正向无功总电能
    insertsavedatar(savedata,R_act_powA,0,4,0);      //A相反向有功总电能
    insertsavedatar(savedata,R_act_powB,0,4,0);      //B相反向有功总电能
    insertsavedatar(savedata,R_act_powC,0,4,0);      //C相反向有功总电能
    insertsavedatar(savedata,R_act_powT,0,4,0);      //总反向有功总电能
    insertsavedatar(savedata,R_rea_powA,0,4,0);      //A相反向无功总电能
    insertsavedatar(savedata,R_rea_powB,0,4,0);      //B相反向无功总电能
    insertsavedatar(savedata,R_rea_powC,0,4,0);      //C相反向无功总电能
    insertsavedatar(savedata,R_rea_powT,0,4,0);      //总反向无功总电能

    insertsavedatar(savedata,V_imdegree,0,2,0);      //电压不平衡度
    insertsavedatar(savedata,C_imdegree,0,2,0);      //电流不平衡度

    savedata[dataend] = ';';                         //the end
}
