/*
** ***************************************
** Change		:JohnySun
** Filename	:email.h
** Time		:2014/10/2 GMT+8
** ***************************************
** ***************************************
** Author		 :  GaoJianSheng
** QQ			 :  328261777
** Last Modified :  2012-09-26 18:54
** Filename		 :  email.c
** Description   :  
** ***************************************
*/
#include <stdio.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <arpa/inet.h>
#include <netinet/in.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

#ifndef __email__h__
#define __email__h__ 0

#define SENDER ("<530064174@qq.com>")	//发件人
#define SMTPSERV ("113.108.16.44")		//163邮箱的SMTP发送服务器地址
#define EMAILLEN 1024

//设置SMTP服务器
int SetSMTPServer(int *sockfd, struct sockaddr_in *servaddr)
{
	int res = -1;
	int iconn = -2;
	ssize_t retConnect = -2;
	char buf[255] = "";

	*sockfd = socket(AF_INET,SOCK_STREAM,0);//以scoket方式和邮件服务器通讯
	if(sockfd>0)
	{
		printf("socket is open");
		bzero(servaddr, sizeof(struct sockaddr_in));
		servaddr->sin_family = AF_INET;
		servaddr->sin_port = htons(25);	//绑定SMTP服务器端口
		inet_pton(AF_INET, SMTPSERV, &servaddr->sin_addr);
		iconn = connect(*sockfd, 
						(struct sockaddr *)servaddr,
						sizeof(struct sockaddr));
		if(iconn==0) {
			printf("connect to %s 25 success!", SMTPSERV);
			//获取服务器响应
			retConnect = recv(*sockfd, buf, sizeof(buf), 0);	
			if(retConnect==-1) {
				printf("Failed to receive msg from smtp port");
				res = -1;
			} else {
				printf("\nServer:%s\n",buf);
				res = 0;
			}
		} else {
			printf("connect 192.168.0.251 25 failed!");
			sleep(1);
			close(*sockfd);
			res = -1;
		}
	} else {
		printf("open socket failed!");
		res = -1;
	}
	return res;
}
//发送邮件
int SendEmail(const char *receiver, 
			const char *fileContent, 
			const char *subject)
{
	int sockfd = -1;
	int iconn = -2;
	ssize_t retConnect = -2;
	struct sockaddr_in servaddr;
	char sentmsg[2048] = "";
	char buf[255] = "";

	iconn = SetSMTPServer(&sockfd, &servaddr);	//设置SMTP服务器
	if(iconn < 0) {
		//连接错误
		return -1;
	}

	//连接SMTP服务器
	memset(sentmsg, '\0', sizeof(sentmsg));
	strcpy(sentmsg, "HELO SERVER\r\n");
	retConnect = send(sockfd, sentmsg, strlen(sentmsg), 0);
	if(retConnect == -1) {
		printf("\nFailed to send meg to smtp port in step 2.\n");
		exit(1);
	} else {
		printf("send msg:%s", sentmsg);
	}
	memset(buf, '\0', sizeof(buf));
	retConnect=recv(sockfd, buf, sizeof(buf), 0);
	if(retConnect == -1) {
		printf("\nFailed to recive meg from smtp port in step 3.\n");
		exit(1);
	} else {
		printf("%s\n", buf);
	}

	//用户准备登录
	memset(sentmsg, '\0', sizeof(sentmsg));
	strcpy(sentmsg, "AUTH LOGIN \r\n");
	retConnect = send(sockfd, sentmsg, strlen(sentmsg), 0);
	if(retConnect > 0) {
		printf("%s", sentmsg);
	}
	memset(buf, '\0', sizeof(buf));
	retConnect = recv(sockfd, buf, sizeof(buf), 0);
	if(retConnect > 0) {
		printf("%s\n", buf);
	}
	
	//用户登录包括二步
	//1、发送用户名
	//2、发送用户密码
	//用户名和密码必须用BASE64进行编码
	//发送用户名
	memset(sentmsg, '\0', sizeof(sentmsg));
	strcpy(sentmsg, "NTMwMDY0MTc0\r\n");
	retConnect = send(sockfd, sentmsg, strlen(sentmsg), 0);
	if(retConnect > 0) {
		printf("%s", sentmsg);
	}
	memset(buf, '\0', sizeof(buf));
	retConnect = recv(sockfd, buf, sizeof(buf), 0);
	if(retConnect > 0) {
		printf("%s\n", buf);
	}

	//发送密码
	memset(sentmsg, '\0', sizeof(sentmsg));
	strcpy(sentmsg, "MjUzMTcyNTMxNw==\r\n");
	retConnect = send(sockfd, sentmsg, strlen(sentmsg), 0);
	if(retConnect > 0) {
		printf("%s", sentmsg);
	}
	memset(buf, '\0', sizeof(buf));
	retConnect = recv(sockfd, buf, sizeof(buf), 0);
	if(retConnect > 0) {
		printf("%s\n", buf);
	}

	//发送发件人
	memset(sentmsg, '\0', sizeof(sentmsg));
	strcpy(sentmsg, "MAIL FROM: ");
	strcat(sentmsg, SENDER);
	strcat(sentmsg, "\r\n");
	retConnect = send(sockfd, sentmsg, strlen(sentmsg), 0);
	if(retConnect > 0) {
		printf("%s", sentmsg);
	}
	memset(buf, '\0', sizeof(buf));
	retConnect = recv(sockfd, buf, sizeof(buf), 0);
	if(retConnect > 0) {
		printf("%s\n", buf);
	}

	//发送收件人
	memset(sentmsg, '\0', sizeof(sentmsg));
	//收件人   以<>括起来
	strcpy(sentmsg, "RCPT TO: <");
	strcat(sentmsg, receiver);
	strcat(sentmsg, ">\r\n");
	retConnect = send(sockfd, sentmsg, strlen(sentmsg), 0);
	if(retConnect > 0) {
		printf("%s", sentmsg);
	}
	memset(buf, '\0', sizeof(buf));
	retConnect = recv(sockfd, buf, sizeof(buf), 0);
	if(retConnect > 0) {
		printf("%s\n", buf);
	}

	//准备发送邮件内容
	memset(sentmsg, '\0', sizeof(sentmsg));
	strcpy(sentmsg, "DATA\r\n");
	retConnect = send(sockfd, sentmsg, strlen(sentmsg), 0);
	if(retConnect > 0) {
		printf("%s", sentmsg);
	}
	memset(buf, '\0', sizeof(buf));
	retConnect = recv(sockfd, buf, sizeof(buf), 0);
	if(retConnect > 0) {
		printf("%s\n", buf);
	}

	//发送邮件内容
	//包括邮件头:发件人，收件人，主题
	//和邮件正文
	memset(sentmsg, '\0', sizeof(sentmsg));
	//发件人
	strcpy(sentmsg, "From: ");
	strcat(sentmsg, SENDER);
	strcat(sentmsg, "\r\n");
	//收件人
	strcat(sentmsg, "To: <");
	strcat(sentmsg, receiver);
	strcat(sentmsg, ">\r\n");
	//主题
	strcat(sentmsg, "Subject: ");
	strcat(sentmsg, subject);
	strcat(sentmsg, "\r\n\r\n");
	//邮件内容
	strcat(sentmsg, fileContent);
	strcat(sentmsg, "\r\n");
	//邮件内容结束 以“\r\n.\r\n”结尾
	strcat(sentmsg, "\r\n.\r\n");
	printf("%s\n", sentmsg);
	retConnect = send(sockfd, sentmsg, strlen(sentmsg), 0);

	//发送邮件结束
	memset(sentmsg, '\0', sizeof(sentmsg));
	strcpy(sentmsg, "QUIT \r\n");
	retConnect = send(sockfd, sentmsg, strlen(sentmsg), 0);
	if(retConnect > 0)
		printf("%s", sentmsg);
	memset(buf, '\0', sizeof(buf));
	retConnect = recv(sockfd, buf, sizeof(buf), 0);
	if(retConnect > 0)
		printf("%s\n", buf);

	//发送邮件结束，关闭Socket
	close(sockfd);
	return 0;
}

#endif
