/* ************************************************************************
 *       Filename:  terminal_user.h
 *    Description:  
 *        Version:  1.0
 *        Created:  2012年05月28日 21时12分58秒
 *       Revision:  none
 *       Compiler:  gcc
 *         Author:  YOUR NAME (), 
 *        Company:  
 * ************************************************************************/

#ifndef __TERMINAL_USER__
#define __TERMINAL_USER__



#define TIMEOUT		400	//1000 MS

typedef struct _PortInfo
{
	int tty;	//0-3 : ttySAC0-ttySAC3
	int baudrate;	//baudrate
	int parity;		//0:none, 1:old, 2:even
	int databit;	//5-8
	int stopbit;	//1-2
	int fclt;		//flow control, 0:noen, 1:hardware, 2:software
	int reservd;	//reserve enable ,set 0;
}PortInfo;



int PortSend(char *data, int datalen);
int PortReciveSelect(char *data, int datalen);
int PortRecive(char *data, int datalen);
int PortInit(void);
void PortClose();







#endif


