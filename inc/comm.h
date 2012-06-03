/* ************************************************************************
 *       Filename:  comm.h
 *    Description:  
 *        Version:  1.0
 *        Created:  2012年06月02日 16时42分34秒
 *       Revision:  none
 *       Compiler:  gcc
 *         Author:  YOUR NAME (), 
 *        Company:  
 * ************************************************************************/

#ifndef __COMM_H__
#define __COMM_H__

#define HEAD1	0x55
#define HEAD2	0xaa



/* **********************CMD**************/
#define OTHER			0x00			//其他 
#define SETSYSPARA		0x01		    //设置系统参数
#define SETTIME			0x02			//设置时间
#define READTIME		0x03			//读时间
#define READTH			0x04			//读温度湿度
#define IRDASEND		0x05			//红外发码 
/* ***************************************/

#define COMMHEAD1	0x55
#define COMMHEAD2	0xAA

#define CMDSETPARA	0x01



/************************返回状态**********************/
#define CMDSUCCESS				0x0f
#define CMDFAILED				0xf0
/*****************************************************/


/************************设置参数**********************/
#define SETPARALEN			10
/* ****************************************************/


/************************时间长度**********************/
#define TIMELEN				7
#define SETTIMEFRAMELEN		10
#define READTIMEFRAMELEN	17
/* ***************************************************/

/***********************温湿度长度********************/
#define THDATALEN			5
#define THFRAMELEN			15
/* ***************************************************/

/***********************红外长度**********************/
#define IADRSENDLEN			10
/*****************************************************/


#endif


