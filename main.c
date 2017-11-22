#include "netclient.h"
#include "fileoperator.h"
#include "dealana.h"
#include <pthread.h>

pthread_t       rev_thread;      //receive thread
pthread_mutex_t revtex;          //the signal for receive buffer operator

char revdata[1500];              //net receive buffer

void *revthread_f(void *arg)     //net receive thread
{
    int  revnum,i,res;
    char databuf[MAXLINE + 1];

    res = pthread_setcancelstate(PTHREAD_CANCEL_ENABLE, NULL);  //set cancel enable
    if (res != 0)  
    {  
        perror("Thread setcancelstate failed!");  
        exit(EXIT_FAILURE);  
    }  
  
    res = pthread_setcanceltype(PTHREAD_CANCEL_ASYNCHRONOUS, NULL); //set cancel asy, cancel immediately 
    if (res != 0)  
    {  
        perror("Thread setcanceltype failed!");  
        exit(EXIT_FAILURE);  
    }  
    
    while(1)
    {
        revnum = netclient_recv(databuf);
        if(revnum > 0)
        {
            pthread_mutex_lock(&revtex);           
            if(revnum > 1500)
                revnum = 1500;
            if(revdata[0] != 0xff)          //flag changed,already work for it,update receive buffer
            {
                revdata[0] = 0xff;
                printf("receive command from the remote server:");
                for(i=0;i<revnum;i++)
                {
                    revdata[i+1] = databuf[i];
                    printf("%d,",databuf[i]);
                }
                printf("\n");
            }
            else
            {
                printf("the last message is not answer yet! \n");
            }
            pthread_mutex_unlock(&revtex); 
        }
    }
}

void test_revthread()
{
    int pthread_kill_err;
    int res;

    pthread_kill_err = pthread_kill(rev_thread,0);
    
    if(pthread_kill_err == ESRCH)
    {
        printf("rev thread is not alive !\n");
        res = pthread_create(&rev_thread, NULL, revthread_f, NULL); 
        if (res != 0)  
        {  
            perror("Thread create failed");  
            exit(EXIT_FAILURE);  
        } 
        wdt_feed();
    }
    else if(pthread_kill_err == EINVAL)
    {
        printf("rev thread has error! \n");
        res = pthread_cancel(rev_thread);
        if(res != 0)
        {
            perror("Thread cancel failed");
            exit(EXIT_FAILURE);
        }
    }
    else
    {
        printf("rev thread is alive\n");
    }
}

void init_revthread()
{
    int res;

    res = pthread_mutex_init(&revtex, NULL);                      //create signal for thread operator
    if (res != 0)  
    {  
        perror("Mutex init failed!");  
        exit(EXIT_FAILURE);  
    } 

    res = pthread_create(&rev_thread, NULL, revthread_f, NULL); 
    if (res != 0)  
    {  
        perror("Thread create failed");  
        exit(EXIT_FAILURE);  
    } 
}

int setinfo_check()
{
    int  res,i;
    char setinfo[111];
    char inbuf[10];
    char su[5];
    res = fgetsetdata(setinfo);
    for(i=0;i<110;i++)
    {
        setinfo[i] = 0;
    }
    if(res != 110)
    {
        loop: fflush(stdin);
        printf("pleas input collector name: ... \n");
        scanf("%s",&inbuf);
        for(i=0;i<10;i++)
        {
            setinfo[10 + i] = inbuf[i];
        }
        fflush(stdin);
        printf("the collector name is : %s\n",inbuf);
        printf("are you sure ? [y/n] \n");
        scanf("%s",&su);
        if(su[0] == 'y')
        {
            res = fsetsetdata(setinfo);
            if(res < 0)
                exit(EXIT_FAILURE);  
        }
        else 
            goto loop;
    }

}

int senddatatoserver()
{

    int    i,j,res;   
    char   setinfo[F_SETFILE_LENGTH];
    int    chnum[8];    
    char   fname[] = {'.','.','/','d','s','m','/','1','1','\0'};

    if(res = fgetsetdata(setinfo) != F_SETFILE_LENGTH)
    {
        return -1;
    }
    printf("send data to server!\n");  
             
    chnum[0] = chartoi(setinfo,30,2);
    chnum[1] = chartoi(setinfo,40,2);
    chnum[2] = chartoi(setinfo,50,2);
    chnum[3] = chartoi(setinfo,60,2);
    chnum[4] = chartoi(setinfo,70,2);
    chnum[5] = chartoi(setinfo,80,2);
    chnum[6] = chartoi(setinfo,90,2);
    chnum[7] = chartoi(setinfo,100,2);
          
    for(i=0;i<8;i++)
    {
        if(chnum[i] > 0)
        {
            printf("the chanel %d has %d elecs !\n",i+1,chnum[i]);
            for(j=0;j<chnum[i];j++)
            {
                fname[7] = i+0x31;
                fname[8] = j+0x31;
                deal_sendrtdata(fname);
            }
        }      
    }   
       
    return 0;
}

void revdataana()
{
    pthread_mutex_lock(&revtex);     //check the receive buffer
    if(revdata[0] == 0xff)
    {
        deal_revbufana(revdata);
        revdata[0] = 0x00;
    }
    pthread_mutex_unlock(&revtex); 
}

void datacommunicate(time_t *lasttime)
{
    int       worktype;
    int       sendspan,timespan;
    time_t    nowtime;
    worktype = fgetworktype();      //check work type
    if(worktype == 1)
    {
        sendspan = fgetsendspan();  
        if(sendspan > 0)
        {          
            time(&nowtime);
            timespan = difftime(nowtime,*lasttime);
            if(timespan > sendspan)
            {
                *lasttime = nowtime;
                senddatatoserver();
            }
        }
        else
            senddatatoserver();
    }
}

int main(int argc,char* argv[])
{   
    int       i,res;   
    time_t    lasttime;

    chdir((char *)dirname(argv[0]));
    setinfo_check();
    netclient_init();                       //net connection
    init_revthread();                           
    wdt_init(60);                           //start watch dog
    
    time(&lasttime);                        //get now time
    while(1)
    {   
        test_revthread();                   //rev thread state check
        revdataana();                       //rev data analysis 

        if(netclient_serverstate == 0)
            datacommunicate(&lasttime);        
        else
            netclient_init();
        
        wdt_feed();
        sleep(1); 
    }
    return 0;
}
