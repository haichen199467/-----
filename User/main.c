/*
*********************************************************************************************************
*
*	ģ������ : ������ģ�顣
*	�ļ����� : main.c
*	��    �� : V1.2
*	˵    �� : ����Flash��д�������̡������ļ�Ϊ bsp_spi_flash.c
*	�޸ļ�¼ :
*		�汾��  ����       ����    ˵��
*		V1.0    2013-02-01 armfly  �׷�
*		V1.1    2013-06-20 armfly  ������ȡ���������д���������� getchar() ������ʽ��
*		V1.2    2014-02-28 armfly  �����̼��⵽V1.3.0
*
*	Copyright (C), 2013-2014, ���������� www.armfly.com
*
*********************************************************************************************************
*/

#include "bsp.h"				/* �ײ�Ӳ������ */
#include "demo_spi_flash.h"
#include "mems_test.h"
#include "ad7705_test.h"

/* ���������������̷������� */
#define EXAMPLE_NAME	"��ҵ���"
#define EXAMPLE_DATE	"2014-02-28"
#define DEMO_VER		"1.2"

/* �������ļ��ڵ��õĺ������� */
static void PrintfLogo(void);

/*
*********************************************************************************************************
*	�� �� ��: main
*	����˵��: c�������
*	��    �Σ���
*	�� �� ֵ: �������(���账��)
*********************************************************************************************************
*/
int main(void)
{
	/*
		ST�̼����е������ļ��Ѿ�ִ���� SystemInit() �������ú����� system_stm32f4xx.c �ļ�����Ҫ������
	����CPUϵͳ��ʱ�ӣ��ڲ�Flash����ʱ������FSMC�����ⲿSRAM
	*/
	extern uint16_t adc1, adc2;
	extern float volt1, volt2;
	extern uint8_t aShowTime[50];
	extern RTC_TimeTypeDef  RTC_TimeStructure;
	extern RTC_DateTypeDef  RTC_DateStructure;
	float a ;
	unsigned char A[4],HalfHour[16*6*30+6],buf[16*6*30+6],cmd;
	int i=0,j=0,k=0,m=0,n=0,x=0;
	bsp_Init();		/* Ӳ����ʼ�� */
	PrintfLogo();	/* ��ӡ������Ϣ������1 */
	while(1)
	{
		bsp_Idle();
		RTC_TimeShow();/*��ȡ��ǰʱ��*/
		while(RTC_TimeStructure.RTC_Seconds%10!=0)
		{
			bsp_Idle();
			RTC_TimeShow();
		}
		/*��ҳͷ��¼ʱ��*/
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
		/*�ɼ�����*/
		GetAD7705();
		GetBMP085();
		/*����ת��ΪFlash�״洢��ʽ*/
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
		
		
		
//		/*����״̬��¼*/
//		printf(" %d %d %d %d %2d %2d     ",
//					RTC_DateStructure.RTC_Year,
//					RTC_DateStructure.RTC_Month,
//					RTC_DateStructure.RTC_Date,
//					RTC_TimeStructure.RTC_Hours,
//					RTC_TimeStructure.RTC_Minutes,
//					RTC_TimeStructure.RTC_Seconds);
//		printf("%d\r\n",i);
		
		
		
		
		
		/*�ж��Ƿ���Ҫ��Ӧ��λ��*/
		if (comGetChar(COM1, &cmd))	/* �Ӵ��ڶ���һ���ַ�(��������ʽ) */
		{
			switch (cmd)
			{
				case '1':
					/*���SPI Flash�е�����*/
					for(k=j-47;k<j;k++)
					{
						/*���kС��0������*/
						if(k>=0)
							m=k;
						else
							m=2*24*30+k;
						/*��� n*��Сʱ ǰ ��������*/
						bsp_InitSFlash();	/* ��ʼ��SPI ����Flash */
						sf_ReadBuffer(buf, m * g_tSF.PageSize+i-16, 16);	
						for(n=0;n<16;n++)
						{
							printf(" %02X",buf[n]);
						}
						printf("\r\n");
						
					}
					/*��ӡ���²ɼ���������*/
					for(n=i-16;n<i;n++)
						{
							printf(" %02X",HalfHour[n]);
							x++;
						}
					printf("\r\n");
					/*��ӡ��ǰʱ��*/
					printf(" %d %d %d %d %d %d\r\n",
					RTC_DateStructure.RTC_Year,
					RTC_DateStructure.RTC_Month,
					RTC_DateStructure.RTC_Date,
					RTC_TimeStructure.RTC_Hours,
					RTC_TimeStructure.RTC_Minutes,
					RTC_TimeStructure.RTC_Seconds);
					break;
						
				case '2':
					/*���SPI Flash�е�����*/
					for(k=j-335;k<j;k++)
					{
						/*���kС��0������*/
						if(k>=0)
							m=k;
						else
							m=2*24*30+k;
						/*��� n*��Сʱ ǰ ��������*/
						bsp_InitSFlash();	/* ��ʼ��SPI ����Flash */
						sf_ReadBuffer(buf, m * g_tSF.PageSize+i-16, 16);	
						for(n=0;n<16;n++)
						{
							printf(" %02X",buf[n]);
						}
						printf("\r\n");
						
					}
					/*��ӡ���²ɼ���������*/
					for(n=i-16;n<i;n++)
						{
							printf(" %02X",HalfHour[n]);
							x++;
						}
					printf("\r\n");
					/*��ӡ��ǰʱ��*/
					printf(" %d %d %d %d %d %d\r\n",
					RTC_DateStructure.RTC_Year,
					RTC_DateStructure.RTC_Month,
					RTC_DateStructure.RTC_Date,
					RTC_TimeStructure.RTC_Hours,
					RTC_TimeStructure.RTC_Minutes,
					RTC_TimeStructure.RTC_Seconds);
					break;
						
			case '3':
					/*���SPI Flash�е�����*/
					for(k=j-1439;k<j;k++)
					{
						/*���kС��0������*/
						if(k>=0)
							m=k;
						else
							m=2*24*30+k;
						/*��� n*��Сʱ ǰ ��������*/
						bsp_InitSFlash();	/* ��ʼ��SPI ����Flash */
						sf_ReadBuffer(buf, m * g_tSF.PageSize+i-16, 16);	
						for(n=0;n<16;n++)
						{
							printf(" %02X",buf[n]);
						}
						printf("\r\n");
						
					}
					/*��ӡ���²ɼ���������*/
					for(n=i-16;n<i;n++)
						{
							printf(" %02X",HalfHour[n]);
							x++;
						}
					printf("\r\n");
					/*��ӡ��ǰʱ��*/
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
					printf("Wrong");	/* ��Ч������´�ӡ������ʾ */
					break;

			}
		}
		/*��Сʱ���ݲɼ����*/
		/* ��jҳ��д���Сʱ������ */
		if(i==16*6*30+6)
		{
			i=0;
			bsp_InitSFlash();
			if (sf_WriteBuffer(HalfHour, j * g_tSF.PageSize, 16*6*30+6) == 0)
			{
				printf("д����Flash����\r\n");
			}
			j++;
			/*�ж��Ƿ񳬳��洢�ռ�,������λ0*/
			if(j==2*24*30)
			{
				j=0;
			}
		
		}
	}
}

/*
*********************************************************************************************************
*	�� �� ��: PrintfLogo
*	����˵��: ��ӡ�������ƺ����̷�������, ���ϴ����ߺ󣬴�PC���ĳ����ն�������Թ۲���
*	��    �Σ���
*	�� �� ֵ: ��
*********************************************************************************************************
*/
static void PrintfLogo(void)
{
	printf("\r\n");
	printf("*************************************************************\r\n");
	printf("* ��������   : %s\r\n", EXAMPLE_NAME);	/* ��ӡ�������� */
	printf("* ���̰汾   : %s\r\n", DEMO_VER);		/* ��ӡ���̰汾 */
	printf("* ��������   : %s\r\n", EXAMPLE_DATE);	/* ��ӡ�������� */

	/* ��ӡST�̼���汾����3���������stm32f10x.h�ļ��� */
	printf("* �̼���汾 : V%d.%d.%d (STM32F4xx_StdPeriph_Driver)\r\n", __STM32F4XX_STDPERIPH_VERSION_MAIN,
			__STM32F4XX_STDPERIPH_VERSION_SUB1,__STM32F4XX_STDPERIPH_VERSION_SUB2);
	/*��������*/
	printf("* ����1����ʾ���24Сʱ����\r\n");	
	printf("* ����2����ʾ���һ�ܵ�����\r\n");	
	printf("* ����3����ʾ���һ��������\r\n");	
	printf("* ����4����ʾ���²ɼ�������\r\n");
	printf("* \n\r");	/* ��ӡһ�пո� */
	printf("*************************************************************\r\n");
}

/***************************** ���������� www.armfly.com (END OF FILE) *********************************/
