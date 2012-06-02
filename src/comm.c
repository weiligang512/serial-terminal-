/* ************************************************************************
 *       Filename:  comm.c
 *    Description:  
 *        Version:  1.0
 *        Created:  2012年06月02日 16时32分15秒
 *       Revision:  none
 *       Compiler:  gcc
 *         Author:  YOUR NAME (), 
 *        Company:  
 * ************************************************************************/
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <sys/select.h>
#include <sys/time.h>
#include <fcntl.h>
#include <termios.h>
#include <errno.h>

#include "comm.h"
#include "terminal_user.h"
#include "485enable_user.h"




/* ****************************************************************
 * *FunctionName	: SendRecive()
 * *Description	: 发送包和接收包
 * *EntryParameter	: s: 发送缓冲 r: 接收缓冲
 * *ReturnValue	: 0: 成功 FLASE -1：失败
 * ****************************************************************/
static int SendRecive(char *s, char *r)
{
	int len, oldlen;
	int count;

	len = 0;
	count = 0;
SendAgain:
	len += PortSend(&s[len], s[2] - len);
	if (len < s[2])
	{
		count++;
		if (count > 10)
		{
			fprintf(stderr, "PortSend failed in SendRecive\n");
		}
		goto SendAgain;

		return -1;
	}

	len = 0;
	count = 0;
ReciveAgain:
	len += PortRecive(&r[len], 32);
	if (len >= 3)
	{
		if (r[0] == 0x55 && r[1] == 0xaa)
		{
			if (len == r[2])
			{
				return 0;
			}
			else
			{	
				count++;
				if (count > 10)
				{
					fprintf(stderr, "Recive Package Time Out\n");
					return -1;
				}
				goto ReciveAgain;
			}
		}
	}
	else
	{
		count++;
		if (count > 10)
		{
			fprintf(stderr, "Recive Package Time Out\n");
			return -1;
		}
		goto ReciveAgain;
	}
}

int main(int argc, char *argv[])
{
	PortInit();

	RS485IOENClose();
	PortClose();
}
