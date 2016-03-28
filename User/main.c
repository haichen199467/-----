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
	float a ;
	unsigned char A[4],HalfHour[16*6*30],buf[16*6*30],cmd;
	int i=0,j=0,k=0,m=0,n=0,x=0;
	bsp_Init();		/* Ӳ����ʼ�� */
	PrintfLogo();	/* ��ӡ������Ϣ������1 */
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
		
		
		if (comGetChar(COM1, &cmd))	/* �Ӵ��ڶ���һ���ַ�(��������ʽ) */
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
					printf("Wrong");	/* ��Ч������´�ӡ������ʾ */
					break;

			}
		}
		
		if(i==16*6*30)
		{
			i=0;
			/* ��jҳ��д���Сʱ������ */
			bsp_InitSFlash();
			if (sf_WriteBuffer(HalfHour, j * g_tSF.PageSize, 16*6*30) == 0)
			{
				printf("д����Flash����\r\n");
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
	
// Begin �ɼ�����ת��Ϊ�ַ�����洢��B[16]��**************************************************************
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
// End   �ɼ�����ת��Ϊ�ַ�����洢��B[16]��**************************************************************
	
//	DemoSpiFlash();		/* ����Flash��ʾ���� */
	
//
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
	printf("* \n\r");	/* ��ӡһ�пո� */
	printf("*************************************************************\r\n");
}

/***************************** ���������� www.armfly.com (END OF FILE) *********************************/
