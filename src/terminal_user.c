/* ************************************************************************
 *       Filename:  terminal_user.c
 *    Description:  
 *        Version:  1.0
 *        Created:  2012年05月28日 21时08分58秒
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

#include "terminal_user.h"
#include "485enable_user.h"


static int fd;

static const char *TTY_Dev[] = {"/dev/ttySAC0", "/dev/ttySAC1", "/dev/ttySAC2", "/dev/ttySAC3"};

static const  PortInfo PortInformation = {2, 9600, 0, 8, 1, 0, 0};	//ttyACS0 9600 N81 flow:none 



static const char *PortGetDev(const PortInfo *Port)
{
	return TTY_Dev[Port->tty];
}

static int PortGetBaudRate(const PortInfo *Port)
{
	int Tmp;

	switch (Port->baudrate)
	{
		case 4800 :
			Tmp = B4800;
		break;

		case 9600 :
			Tmp = B9600;
		break;

		case 19200 :
			Tmp = B19200;
		break;

		case 115200 :
			Tmp = B115200;
		break;

		default :
			Tmp = B9600;
		break;
	}

	return Tmp;
}

static int PortGetDataBit(const PortInfo *Port)
{
	int Tmp;

	switch (Port->databit)
	{
		case 5 :
			Tmp = CS5;
		break;

		case 6 :
			Tmp = CS6;
		break;

		case 7 :
			Tmp = CS7;
		break;

		case 8 :
			Tmp = CS8;
		break;

		default :
			Tmp = CS8;
		break;
	}

	return Tmp;
}

static int PortSetPara(const PortInfo *Port)
{
	struct termios termios_old, termios_new;
	int baudrate, databit, fclt;
	int parity, stopbit;
	int res;

	memset(&termios_old, 0, sizeof(termios_old));
	memset(&termios_new, 0, sizeof(termios_new));
	
	cfmakeraw(&termios_new);
	tcgetattr(fd, &termios_old);

	//set baud rate
	baudrate = //PortGetBaudRate(Port);
	cfsetispeed(&termios_new, baudrate);
	cfsetospeed(&termios_new, baudrate);

	termios_new.c_cflag |= CLOCAL;
	termios_new.c_cflag |= CREAD;

	//set flow control mode
	fclt = Port->fclt;
	switch (fclt)
	{
		case 0 :
			termios_new.c_cflag &= ~CRTSCTS;
		break;

		case 1 :
			termios_new.c_cflag |= CRTSCTS;
		break;

		case 2 :
			termios_new.c_cflag |= IXON | IXOFF | IXANY;
		break;

		default :
			termios_new.c_cflag &= ~CRTSCTS;
		break;
	}

	//set data bits
	termios_new.c_cflag &= ~CSIZE;
	databit = PortGetDataBit(Port);
	termios_new.c_cflag |= databit;

	//set parity check
	parity = Port->parity;
	switch (parity)
	{
		case 0 :
			termios_new.c_cflag &= ~PARENB;
		break;

		case 1 :
			termios_new.c_cflag |= PARENB;
			termios_new.c_cflag &= ~PARODD;
		break;

		case 2 :
			termios_new.c_cflag |= PARENB;
			termios_new.c_cflag |= PARODD;
		break;

		default :
			termios_new.c_cflag &= ~PARENB;
		break;
	}
	
	stopbit = Port->stopbit;
	switch (stopbit)
	{
		case 1 :
			termios_new.c_cflag &= ~CSTOPB;
		break;

		case 2 :
			termios_new.c_cflag |= CSTOPB;
		break;

		default :
			termios_new.c_cflag &= ~CSTOPB;
		break;
	}
	
	//other attributions
	termios_new.c_cflag &= ~OPOST;
	termios_new.c_cc[VMIN] = 1;
	termios_new.c_cc[VTIME] = 1;

	tcflush(fd, TCIFLUSH);
	res = tcsetattr(fd, TCSANOW, &termios_new);
	tcgetattr(fd, &termios_old);

	return res;
}

static int PortOpen(const PortInfo *Port)
{
	const char *dev;
	int fd;

	dev = PortGetDev(Port);
	fd = open(dev, O_RDWR | O_NOCTTY | O_NONBLOCK);

	return fd;
}

static int PortSelect(int fd, long time)
{
	fd_set readfd;
	struct timeval timeout;
	int res;

	timeout.tv_sec = time / 1000;
	timeout.tv_usec = time % 1000;

	FD_ZERO(&readfd);
	FD_SET(fd, &readfd);
	res = select(fd + 1, &readfd, NULL, NULL, &timeout);

	if (res < 0)
	{
		fprintf(stderr, "select failed in PortSelect\n");
	}
	else if (res == 0)
	{
		fprintf(stderr, "select timeout int PortSelect\n");
	}
	else
	{
		res = FD_ISSET(fd, &readfd);
		if (res)
		{
			return 0;
		}
	}

	return -1;
}

void PortClose()
{
	close(fd);
}

int PortSend(char *data, int datalen)
{
	int len;
	
	RS485ENSend();
	len = write(fd, data, datalen);

	usleep(500);
	RS485ENRecive();
	if (len == datalen)
	{
		return len;
	}
	else
	{
		tcflush(fd, TCOFLUSH);

		return -1;
	}
}

int PortReciveSelect(char *data, int datalen)
{
	int len = 0;
	int res;
	
	RS485ENRecive();

	res = PortSelect(fd, TIMEOUT);
	if (!res)
	{
		usleep(40000);
		len = read(fd, data, datalen);
	}
	printf("port recive select len %d\n", len);
	return len;
}

int PortRecive(char *data, int datalen)
{
	int len = 0;
	
	RS485ENRecive();

	len = read(fd, data, datalen);

	printf("port recive len %d\n", len);
	return len;
}

int PortInit(void)
{
	int res;
 
	RS485IOENInit();
	RS485ENSend();

	fd = PortOpen(&PortInformation);
	if (fd < 0)
	{
		fprintf(stderr, "Open failed\n");
		
		return fd;
	}

	res = PortSetPara(&PortInformation);
	if (res < 0)
	{
		fprintf(stderr, "PortSetPara failed\n");

		return res;
	}

	return 0;
}


