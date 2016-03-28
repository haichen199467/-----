/*
*********************************************************************************************************
*
*	模块名称 : 串行Flash读写演示模块。
*	文件名称 : sfDemo.h
*	版    本 : V1.0
*	说    明 : 头文件
*	修改记录 :
*		版本号  日期       作者    说明
*		v1.0    2011-05-13 armfly  ST固件库V3.5.0版本。
*
*	Copyright (C), 2010-2011, 安富莱电子 www.armfly.com
*
*********************************************************************************************************
*/

#ifndef _SFDEMO_H
#define _SFDEMO_H

/* 供外部调用的函数声明 */
void DemoSpiFlash(void);
void F2CA(unsigned char * charArray,float floatVariable);
void CA2F(unsigned char * charArray,float floatVariable);
#endif


