/* ************************************************************************
 *       Filename:  485enable_user.h
 *    Description:  
 *        Version:  1.0
 *        Created:  2012年05月29日 19时32分57秒
 *       Revision:  none
 *       Compiler:  gcc
 *         Author:  YOUR NAME (), 
 *        Company:  
 * ************************************************************************/

#ifndef __485ENABLE_H__
#define __485ENABLE_H__


#define RS485IOENDEV	"/dev/485EN"

#define GPM     0x6000
#define GPN     0x7000	
#define GPK     0x8000		
#define GPP     0x9000	
#define GPIO_IN					0 
#define GPIO_OUT				1
#define GPIO_SET				7
#define GPIO_CLR				3 

#define OP_GPIO					4

#define GPIO_UP_ON				5

#define GPIO_UP_OFF				6
#define GPIO_PIN_BIT(startbit,endbit)			((startbit)|((endbit)<<4))

int RS485IOENInit(void);

void RS485ENSend(void);

void RS485ENRecive(void);

void RS485IOENClose(void);



#endif


