#include <unistd.h>  
#include <stdlib.h>  
#include <stdio.h>  
#include <string.h>  
#include <errno.h>  
#include <sys/msg.h>
#include "email.h"  
#define MAX_TEXT 128    
struct msg_st  
{  
	long int msg_type;  
	char Message[4][MAX_TEXT];     
};    
int main()  
{  
	int msgid = -1;  
	struct msg_st data;  
	long int msgtype = 0;
	char *content="1"; 
	char *subject = "program report"; 
	msgid = msgget((key_t)1234, 0666 | IPC_CREAT);  // create a message queue
	if(msgid == -1)  
	{  
        	fprintf(stderr, "msgget failed with error: %d\n", errno);  
        	exit(EXIT_FAILURE);  
    	}  
	while(1)// receive message every 1 second
	{
		if(msgrcv(msgid, (void*)&data, BUFSIZ, msgtype, 0) == -1)  
		{  
            		fprintf(stderr, "msgrcv failed with errno: %d\n", errno);  
           		sleep(1000);
			continue;
        	}
		if(data.Message[2][0]!='0')
		{
			content="It may be copied from others.\n";
		}
		else if(data.Message[3][0]!='0')
		{
			content="It isn't a correct program.\n";
		}
		else
		{
			content="congratuation\n";
		}		
		SendEmail(data.Message[0],content, subject);  
		
	} 
} 
