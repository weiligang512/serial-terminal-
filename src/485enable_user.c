/* ************************************************************************
 *       Filename:  485enable_user.c
 *    Description:  
 *        Version:  1.0
 *        Created:  2012年05月29日 19时32分16秒
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
#include <sys/ioctl.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <errno.h>

#include "485enable_user.h"



static int fd;

static int RS485IOENOpen(void)
{
	fd = open(RS485IOENDEV, O_RDWR);
	if (fd < 0)
	{
		fprintf(stderr, "open 485IOEN failed \n");
		exit(0);
	}

	return fd;
}
	
static int RS485IOENSetMode(void)
{
	ioctl(fd, GPIO_OUT, GPIO_PIN_BIT(8,8));
    ioctl(fd, GPIO_UP_ON, GPIO_PIN_BIT(8,8));
	ioctl(fd, GPIO_SET, GPIO_PIN_BIT(8,8));
	
	return 0;
}

int RS485IOENInit(void)
{
	RS485IOENOpen();
	RS485IOENSetMode();

	return 0;
}

void RS485ENSend(void)
{
	ioctl(fd, GPIO_SET, GPIO_PIN_BIT(8,8));
}

void RS485ENRecive(void)
{
	ioctl(fd, GPIO_CLR, GPIO_PIN_BIT(8,8));
}

void RS485IOENClose(void)
{
	close(fd);
}

