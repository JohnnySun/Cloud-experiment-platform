#include <unistd.h>  
#include <stdlib.h>  
#include <stdio.h>  
#include <string.h>  
#include <sys/msg.h>  
#include <errno.h>  
  
#define MAX_TEXT 128  
struct msg_st  //array stuct
{  
	long int msg_type;  
	char Message[4][MAX_TEXT]; //Message[0]->mail address
	//Message[1]->path of error.log
	//Message[2][0]->similarity flag ("0" means flase)
	//Message[3][0]->successful compilation flag ("0" means flase)
};  
  
int main(int argc,char** argv)  
{  
	struct msg_st data;  
	char buffer[BUFSIZ];// buffer memory
	int msgid;    	    // Message queue flag
	/*if(argc==6)
	{ 
		msgid=atoi(argv[5]);  
	}*/
	msgid=-1;
 	if(msgid == -1)  
	{   
		msgid = msgget((key_t)1234, 0666 | IPC_CREAT);// creat a message queue
	}  
	if(msgid == -1)  
	{  
	        fprintf(stderr, "msgget failed with error: %d\n", errno);
		exit(EXIT_FAILURE);  
	}   
        data.msg_type = 1;
        strcpy(data.Message[0], argv[1]); 
 	strcpy(data.Message[1], argv[2]); 
	data.Message[2][0]=argv[3][0];
	data.Message[3][0]=argv[4][0];
        //send message
        if(msgsnd(msgid, (void*)&data, 512, 0) == -1)  
        {  
		fprintf(stderr, "msgsnd failed\n");  
 		exit(EXIT_FAILURE);  
        }  
	printf("%d\n",msgid);  
    return 0;  
} 
