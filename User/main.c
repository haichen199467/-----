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
	extern uint8_t aShowTime[50];
	extern RTC_TimeTypeDef  RTC_TimeStructure;
	extern RTC_DateTypeDef  RTC_DateStructure;
	float a ;
	unsigned char A[4],HalfHour[16*6*30+6],buf[16*6*30+6],cmd;
	int i=0,j=0,k=0,m=0,n=0,x=0;
	bsp_Init();		/* 硬件初始化 */
	PrintfLogo();	/* 打印例程信息到串口1 */
	while(1)
	{
		bsp_Idle();
		RTC_TimeShow();/*获取当前时间*/
		while(RTC_TimeStructure.RTC_Seconds%10!=0)
		{
			bsp_Idle();
			RTC_TimeShow();
		}
		/*在页头记录时间*/
		if(i==0)
		{ RTC_DateShow();
			HalfHour[i++]=RTC_DateStructure.RTC_Year;
			HalfHour[i++]=RTC_DateStructure.RTC_Month;
			HalfHour[i++]=RTC_DateStructure.RTC_Date;
			HalfHour[i++]=RTC_TimeStructure.RTC_Hours;
			HalfHour[i++]=RTC_TimeStructure.RTC_Minutes;
			HalfHour[i++]=RTC_TimeStructure.RTC_Seconds;
		}
		RTC_TimeShow();
		bsp_DelayMS(1000);
		/*采集数据*/
		GetAD7705();
		GetBMP085();
		/*数据转换为Flash易存储形式*/
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
		
		
		
//		/*运行状态记录*/
//		printf(" %d %d %d %d %2d %2d     ",
//					RTC_DateStructure.RTC_Year,
//					RTC_DateStructure.RTC_Month,
//					RTC_DateStructure.RTC_Date,
//					RTC_TimeStructure.RTC_Hours,
//					RTC_TimeStructure.RTC_Minutes,
//					RTC_TimeStructure.RTC_Seconds);
//		printf("%d\r\n",i);
		
		
		
		
		
		/*判断是否需要相应上位机*/
		if (comGetChar(COM1, &cmd))	/* 从串口读入一个字符(非阻塞方式) */
		{
			switch (cmd)
			{
				case '1':
					/*输出SPI Flash中的数据*/
					for(k=j-47;k<j;k++)
					{
						/*解决k小于0的问题*/
						if(k>=0)
							m=k;
						else
							m=2*24*30+k;
						/*输出 n*半小时 前 测量数据*/
						bsp_InitSFlash();	/* 初始化SPI 串行Flash */
						sf_ReadBuffer(buf, m * g_tSF.PageSize+i-16, 16);	
						for(n=0;n<16;n++)
						{
							printf(" %02X",buf[n]);
						}
						printf("\r\n");
						
					}
					/*打印最新采集到的数据*/
					for(n=i-16;n<i;n++)
						{
							printf(" %02X",HalfHour[n]);
							x++;
						}
					printf("\r\n");
					/*打印当前时间*/
					printf(" %d %d %d %d %d %d\r\n",
					RTC_DateStructure.RTC_Year,
					RTC_DateStructure.RTC_Month,
					RTC_DateStructure.RTC_Date,
					RTC_TimeStructure.RTC_Hours,
					RTC_TimeStructure.RTC_Minutes,
					RTC_TimeStructure.RTC_Seconds);
					break;
						
				case '2':
					/*输出SPI Flash中的数据*/
					for(k=j-335;k<j;k++)
					{
						/*解决k小于0的问题*/
						if(k>=0)
							m=k;
						else
							m=2*24*30+k;
						/*输出 n*半小时 前 测量数据*/
						bsp_InitSFlash();	/* 初始化SPI 串行Flash */
						sf_ReadBuffer(buf, m * g_tSF.PageSize+i-16, 16);	
						for(n=0;n<16;n++)
						{
							printf(" %02X",buf[n]);
						}
						printf("\r\n");
						
					}
					/*打印最新采集到的数据*/
					for(n=i-16;n<i;n++)
						{
							printf(" %02X",HalfHour[n]);
							x++;
						}
					printf("\r\n");
					/*打印当前时间*/
					printf(" %d %d %d %d %d %d\r\n",
					RTC_DateStructure.RTC_Year,
					RTC_DateStructure.RTC_Month,
					RTC_DateStructure.RTC_Date,
					RTC_TimeStructure.RTC_Hours,
					RTC_TimeStructure.RTC_Minutes,
					RTC_TimeStructure.RTC_Seconds);
					break;
						
			case '3':
					/*输出SPI Flash中的数据*/
					for(k=j-1439;k<j;k++)
					{
						/*解决k小于0的问题*/
						if(k>=0)
							m=k;
						else
							m=2*24*30+k;
						/*输出 n*半小时 前 测量数据*/
						bsp_InitSFlash();	/* 初始化SPI 串行Flash */
						sf_ReadBuffer(buf, m * g_tSF.PageSize+i-16, 16);	
						for(n=0;n<16;n++)
						{
							printf(" %02X",buf[n]);
						}
						printf("\r\n");
						
					}
					/*打印最新采集到的数据*/
					for(n=i-16;n<i;n++)
						{
							printf(" %02X",HalfHour[n]);
							x++;
						}
					printf("\r\n");
					/*打印当前时间*/
					printf(" %d %d %d %d %d %d\r\n",
					RTC_DateStructure.RTC_Year,
					RTC_DateStructure.RTC_Month,
					RTC_DateStructure.RTC_Date,
					RTC_TimeStructure.RTC_Hours,
					RTC_TimeStructure.RTC_Minutes,
					RTC_TimeStructure.RTC_Seconds);
					break;

			case '4':
					printf("%d,%d,%d,%d,%d,%2d,%10f,%10f,%10d,%10d \r\n",
					RTC_DateStructure.RTC_Year,
					RTC_DateStructure.RTC_Month,
					RTC_DateStructure.RTC_Date,
					RTC_TimeStructure.RTC_Hours,
					RTC_TimeStructure.RTC_Minutes,
					RTC_TimeStructure.RTC_Seconds,volt1,volt2,g_tBMP085.Temp,g_tBMP085.Press);
					break;
						
				default:
					printf("Wrong");	/* 无效命令，重新打印命令提示 */
					break;

			}
		}
		/*半小时数据采集完成*/
		/* 在j页中写入半小时的数据 */
		if(i==16*6*30+6)
		{
			i=0;
			bsp_InitSFlash();
			if (sf_WriteBuffer(HalfHour, j * g_tSF.PageSize, 16*6*30+6) == 0)
			{
				printf("写串行Flash出错！\r\n");
			}
			j++;
			/*判断是否超出存储空间,超出复位0*/
			if(j==2*24*30)
			{
				j=0;
			}
		
		}
	}
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
	/*功能描述*/
	printf("* 按“1”显示最近24小时数据\r\n");	
	printf("* 按“2”显示最近一周的数据\r\n");	
	printf("* 按“3”显示最近一个月数据\r\n");	
	printf("* 按“4”显示最新采集的数据\r\n");
	printf("* \n\r");	/* 打印一行空格 */
	printf("*************************************************************\r\n");
}

/***************************** 安富莱电子 www.armfly.com (END OF FILE) *********************************/
