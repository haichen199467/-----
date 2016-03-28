/*
*********************************************************************************************************
*
*	模块名称 : 主程序模块。
*	文件名称 : main.c
*	版    本 : V1.2
*	说    明 : 串行Flash读写例程例程。核心文件为 bsp_spi_flash.c
*	修改记录 :
*		版本号  日期       作者    说明
*		V1.0    2013-02-01 armfly  首发
*		V1.1    2013-06-20 armfly  更换读取串口命令的写法，不采用 getchar() 阻塞方式。
*		V1.2    2014-02-28 armfly  升级固件库到V1.3.0
*
*	Copyright (C), 2013-2014, 安富莱电子 www.armfly.com
*
*********************************************************************************************************
*/

#include "bsp.h"				/* 底层硬件驱动 */
#include "demo_spi_flash.h"
#include "mems_test.h"
#include "ad7705_test.h"
/* 定义例程名和例程发布日期 */
#define EXAMPLE_NAME	"毕业设计"
#define EXAMPLE_DATE	"2014-02-28"
#define DEMO_VER		"1.2"

/* 仅允许本文件内调用的函数声明 */
static void PrintfLogo(void);

/*
*********************************************************************************************************
*	函 数 名: main
*	功能说明: c程序入口
*	形    参：无
*	返 回 值: 错误代码(无需处理)
*********************************************************************************************************
*/
int main(void)
{
	/*
		ST固件库中的启动文件已经执行了 SystemInit() 函数，该函数在 system_stm32f4xx.c 文件，主要功能是
	配置CPU系统的时钟，内部Flash访问时序，配置FSMC用于外部SRAM
	*/
	extern uint16_t adc1, adc2;
	extern float volt1, volt2;
	float a ;
	unsigned char A[4],HalfHour[16*6*30],buf[16*6*30],cmd;
	int i=0,j=0,k=0,m=0,n=0,x=0;
	bsp_Init();		/* 硬件初始化 */
	PrintfLogo();	/* 打印例程信息到串口1 */
	while(1)
	{
		bsp_Idle();
		GetAD7705();
		GetBMP085();
		a =volt1;
		F2CA(A,a); /*float->charArray*/
		HalfHour[i++]=A[0];HalfHour[i++]=A[1];HalfHour[i++]=A[2];HalfHour[i++]=A[3];
		a =volt2;
		F2CA(A,a); /*float->charArray*/
		HalfHour[i++]=A[0];HalfHour[i++]=A[1];HalfHour[i++]=A[2];HalfHour[i++]=A[3];
		a =(float)g_tBMP085.Temp;
		F2CA(A,a); /*float->charArray*/
		HalfHour[i++]=A[0];HalfHour[i++]=A[1];HalfHour[i++]=A[2];HalfHour[i++]=A[3];
		a =(float)g_tBMP085.Press;
		F2CA(A,a); /*float->charArray*/
		HalfHour[i++]=A[0];HalfHour[i++]=A[1];HalfHour[i++]=A[2];HalfHour[i++]=A[3];
		
		
		if (comGetChar(COM1, &cmd))	/* 从串口读入一个字符(非阻塞方式) */
		{
			switch (cmd)
			{
				case '1':
					x=0;
					for(k=j-48;k<=j;k++)
					{
						if(k>=0)
							m=k;
						else
							m=2*24*30+k;
						sf_ReadBuffer(buf, m * g_tSF.PageSize, 16*6*30);	
						for(n=0;n<16*6*30;n++)
						{
							printf(" %02X",buf[x]);
						}
					}
					printf("%d\r\n",x);
					
					break;


				default:
					printf("Wrong");	/* 无效命令，重新打印命令提示 */
					break;

			}
		}
		
		if(i==16*6*30)
		{
			i=0;
			/* 在j页中写入半小时的数据 */
			bsp_InitSFlash();
			if (sf_WriteBuffer(HalfHour, j * g_tSF.PageSize, 16*6*30) == 0)
			{
				printf("写串行Flash出错！\r\n");
			}
			j++;
			if(j==2*24*30)
			{
				j=0;
			}
		
		}
		
	}
//	bsp_DelayMS(5000);
//	GetAD7705();
//	GetBMP085();
	
// Begin 采集数据转换为字符数组存储到B[16]中**************************************************************
//  a =volt1;
//	F2CA(A,a); /*float->charArray*/
//	B[0]=A[0];B[1]=A[1];B[2]=A[2];B[3]=A[3];
//	a =volt2;
//	F2CA(A,a); /*float->charArray*/
//	B[4]=A[0];B[5]=A[1];B[6]=A[2];B[7]=A[3];
//	a =(float)g_tBMP085.Temp;
//	F2CA(A,a); /*float->charArray*/
//	B[8]=A[0];B[9]=A[1];B[10]=A[2];B[11]=A[3];
//	a =(float)g_tBMP085.Press;
//	F2CA(A,a); /*float->charArray*/
//	B[12]=A[0];B[13]=A[1];B[14]=A[2];B[15]=A[3];
// End   采集数据转换为字符数组存储到B[16]中**************************************************************
	
//	DemoSpiFlash();		/* 串行Flash演示程序 */
	
//
}

/*
*********************************************************************************************************
*	函 数 名: PrintfLogo
*	功能说明: 打印例程名称和例程发布日期, 接上串口线后，打开PC机的超级终端软件可以观察结果
*	形    参：无
*	返 回 值: 无
*********************************************************************************************************
*/
static void PrintfLogo(void)
{
	printf("\r\n");
	printf("*************************************************************\r\n");
	printf("* 例程名称   : %s\r\n", EXAMPLE_NAME);	/* 打印例程名称 */
	printf("* 例程版本   : %s\r\n", DEMO_VER);		/* 打印例程版本 */
	printf("* 发布日期   : %s\r\n", EXAMPLE_DATE);	/* 打印例程日期 */

	/* 打印ST固件库版本，这3个定义宏在stm32f10x.h文件中 */
	printf("* 固件库版本 : V%d.%d.%d (STM32F4xx_StdPeriph_Driver)\r\n", __STM32F4XX_STDPERIPH_VERSION_MAIN,
			__STM32F4XX_STDPERIPH_VERSION_SUB1,__STM32F4XX_STDPERIPH_VERSION_SUB2);
	printf("* \n\r");	/* 打印一行空格 */
	printf("*************************************************************\r\n");
}

/***************************** 安富莱电子 www.armfly.com (END OF FILE) *********************************/
