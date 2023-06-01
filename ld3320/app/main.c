/*******************************************************
**	CPU: STC11L08XE
**	����22.1184MHZ
**	�����ʣ�9600 bit/S
**  ��ʱ����ģʽ�� ��ʶ��ʱ��˵����Ѻ����15���ڲ���������ʶ�����˳�����
/*********************************************************/


#include "config.h"
#include "string.h"
/************************************************************************************/
//	nAsrStatus ������main�������б�ʾ�������е�״̬������LD3320оƬ�ڲ���״̬�Ĵ���
//	LD_ASR_NONE:		��ʾû������ASRʶ��
//	LD_ASR_RUNING��		��ʾLD3320������ASRʶ����
//	LD_ASR_FOUNDOK:		��ʾһ��ʶ�����̽�������һ��ʶ����
//	LD_ASR_FOUNDZERO:	��ʾһ��ʶ�����̽�����û��ʶ����
//	LD_ASR_ERROR:		��ʾһ��ʶ��������LD3320оƬ�ڲ����ֲ���ȷ��״̬
/***********************************************************************************/
uint8 idata nAsrStatus = 0;
void MCU_init();
void ProcessInt0(); //ʶ��������
void delay(unsigned long uldata);
void User_handle(uint8 dat);//�û�ִ�в�������
void Delay200ms();
void Led_test(void);//��Ƭ������ָʾ
uint8_t G0_flag = DISABLE; //���б�־��ENABLE:���С�DISABLE:��ֹ����
sbit LED = P4 ^ 2; //�ź�ָʾ��
int over_time; 


/***********************************************************
* ��    �ƣ� void  main(void)
* ��    �ܣ� ������	�������
* ��ڲ�����
* ���ڲ�����
* ˵    ����
* ���÷�����
**********************************************************/
void  main(void)
{
	uint8 idata nAsrRes;
	uint8 i = 0;
	P1M0 = 0xFF;
	P1M1 = 0x00;
	Led_test();
	MCU_init();
	LD_Reset();
	UartIni(); /*���ڳ�ʼ��*/
	nAsrStatus = LD_ASR_NONE;		//	��ʼ״̬��û������ASR
	//PrintCom("��ӭʹ��");
	while(1)
	{
		if(over_time>=500)
		{
			over_time=0;
			G0_flag = DISABLE;
			LED = 1;
		//	PrintCom("����,����������");
		}
		switch(nAsrStatus)
		{
		case LD_ASR_RUNING:
		case LD_ASR_ERROR:
			break;
		case LD_ASR_NONE:
		{
			nAsrStatus = LD_ASR_RUNING;
			if (RunASR() == 0)	/*	����һ��ASRʶ�����̣�ASR��ʼ����ASR���ӹؼ��������ASR����*/
			{
				nAsrStatus = LD_ASR_ERROR;
			}
			break;
		}
		case LD_ASR_FOUNDOK: /*	һ��ASRʶ�����̽�����ȥȡASRʶ����*/
		{
			nAsrRes = LD_GetResult();		/*��ȡ���*/
			User_handle(nAsrRes);//�û�ִ�к���
			nAsrStatus = LD_ASR_NONE;
			break;
		}
		case LD_ASR_FOUNDZERO:
		default:
		{
			nAsrStatus = LD_ASR_NONE;
			break;
		}
		}
	}

}
/***********************************************************
* ��    �ƣ� 	 LED�Ʋ���
* ��    �ܣ� ��Ƭ���Ƿ���ָʾ
* ��ڲ����� ��
* ���ڲ�������
* ˵    ����
**********************************************************/
void Led_test(void)
{
	LED = ~ LED;
	Delay200ms();
	LED = ~ LED;
	Delay200ms();
	LED = ~ LED;
	Delay200ms();
	LED = ~ LED;
	Delay200ms();
	LED = ~ LED;
	Delay200ms();
	LED = ~ LED;
}
/***********************************************************
* ��    �ƣ� void MCU_init()
* ��    �ܣ� ��Ƭ����ʼ��
* ��ڲ�����
* ���ڲ�����
* ˵    ����
* ���÷�����
**********************************************************/
void MCU_init()
{
	P0 = 0xff;
	P1 = 0x00;
	P2 = 0xff;
	P3 = 0xff;
	P4 = 0xff;

	AUXR &= 0x7F;		//��ʱ��ʱ��12Tģʽ
	TMOD |= 0x01;		//���ö�ʱ��ģʽ
	TL0 = 0x00;		//���ö�ʱ��ֵ
	TH0 = 0x28;		//���ö�ʱ��ֵ
	TF0 = 0;		//���TF0��־
	TR0 = 1;		//��ʱ��0��ʼ��ʱ
	ET0=1;

	LD_MODE = 0;		//	����MD�ܽ�Ϊ�ͣ�����ģʽ��д
	IE0 = 1;
	EX0 = 1;
	EA = 1;
	WDT_CONTR=0x3D;
}
/***********************************************************
* ��    �ƣ�	��ʱ����
* ��    �ܣ�
* ��ڲ�����
* ���ڲ�����
* ˵    ����
* ���÷�����
**********************************************************/
void Delay200us()		//@22.1184MHz
{
	unsigned char i, j;
	_nop_();
	_nop_();
	i = 5;
	j = 73;
	do
	{
		while (--j);
	}
	while (--i);
}

void  delay(unsigned long uldata)
{
	unsigned int j  =  0;
	unsigned int g  =  0;
	while(uldata--)
		Delay200us();
}

void Delay200ms()		//@22.1184MHz
{
	unsigned char i, j, k;

	i = 17;
	j = 208;
	k = 27;
	do
	{
		do
		{
			while (--k);
		}
		while (--j);
	}
	while (--i);
}

/***********************************************************
* ��    �ƣ� �жϴ�������
* ��    �ܣ�
* ��ڲ�����
* ���ڲ�����
* ˵    ����
* ���÷�����
**********************************************************/
void ExtInt0Handler(void) interrupt 0
{
	ProcessInt0();
}
/***********************************************************
* ��    �ƣ��û�ִ�к���
* ��    �ܣ�ʶ��ɹ���ִ�ж������ڴ˽����޸�
* ��ڲ����� ��
* ���ڲ�������
* ˵    ����
**********************************************************/
void 	User_handle(uint8 dat)
{
	if(0 == dat)
	{
		G0_flag = ENABLE;
		over_time=0;
	//	PrintCom("���,����");
		LED = 0;
	}
	else if(ENABLE == G0_flag)
	{
//		G0_flag = DISABLE;
//		LED = 1;
		switch(dat)
		{
		case CODE_1:	 
			over_time=0;
		  //UARTSendByte("����ģʽ");
			PrintCom("0");
			break;
		case CODE_2:	 
			over_time=0;
		  //UARTSendByte("�ֶ�ģʽ");
			PrintCom("1");
			break;
		case CODE_3:		
			over_time=0;
			PrintCom("2");
			break;
		case CODE_4:		
			over_time=0;
		  //UARTSendByte("�ٶ���4");
			PrintCom("3");
			break;
		case CODE_5:		
			over_time=0;
		  //UARTSendByte("�ٶ�һ��");
			PrintCom("4");
			break;
		case CODE_6:		
			over_time=0;
			PrintCom("5");
			break;
		case CODE_7:		
			over_time=0;
			PrintCom("6");
			break;
		case CODE_8:		
			over_time=0;
			PrintCom("");
			break;
		case CODE_9:		/*���.....��*/
			over_time=0;
			PrintCom("");
			break;
		case CODE_10:		/*���.....��*/
			over_time=0;
			PrintCom("");
			break;
		case CODE_11:		/*���.....��*/
			over_time=0;
			PrintCom("");
			break;
		case CODE_12:		/*���.....��*/
			over_time=0;
			PrintCom("");
			break;
		case CODE_13:		/*���.....��*/
			over_time=0;
			PrintCom("");
			break;
		case CODE_14:		/*���.....��*/
			over_time=0;
			PrintCom("");
			break;
		case CODE_15:		/*���.....��*/
			over_time=0;
			PrintCom("");
			break;
		case CODE_16:		/*���.....��*/
			over_time=0;
			PrintCom("");
			break;
		case CODE_17:		/*���.....��*/
			over_time=0;
			PrintCom("");
			break;
		case CODE_18:		/*���.....��*/
			over_time=0;
			PrintCom("");
			break;
		case CODE_19:		/*���.....��*/
			over_time=0;
			PrintCom("");
			break;
		case CODE_20:		/*���.....��*/
			over_time=0;
			PrintCom("");
			break;
		case CODE_21:		/*���.....��*/
			over_time=0;
			PrintCom("");
			break;
		case CODE_22:		/*���.....��*/
			over_time=0;
			PrintCom("");
			break;
		case CODE_23:		/*���.....��*/
			over_time=0;
			PrintCom("");
			break;
		case CODE_24:		/*���.....��*/
			over_time=0;
			PrintCom("");
			break;
		case CODE_25:		/*���.....��*/
			over_time=0;
			PrintCom("");
			break;
		case CODE_26:		/*���.....��*/
			over_time=0;
			PrintCom("");
			break;
		case CODE_27:		/*���.....��*/
			over_time=0;
			PrintCom("");
			break;
		case CODE_28:		/*���.....��*/
			over_time=0;
			PrintCom("");
			break;
		case CODE_29:		/*���.....��*/
			over_time=0;
			PrintCom("");
			break;
		case CODE_30:		/*���.....��*/
			over_time=0;
			PrintCom("");
			break;
		case CODE_31:		/*���.....��*/
			over_time=0;
			PrintCom("");
			break;
		case CODE_32:		/*���.....��*/
			over_time=0;
			PrintCom("");
			break;
		case CODE_33:		/*���.....��*/
			over_time=0;
			PrintCom("");
			break;
		case CODE_34:		/*���.....��*/
			over_time=0;
			PrintCom("");
			break;
		case CODE_35:		/*���.....��*/
			over_time=0;
			PrintCom("");
			break;
		case CODE_36:		/*���.....��*/
			over_time=0;
			PrintCom("");
			break;
		case CODE_37:		/*���.....��*/
			over_time=0;
			PrintCom("");
			break;
		case CODE_38:		/*���.....��*/
			over_time=0;
			PrintCom("");
			break;
		case CODE_39:		/*���.....��*/
			over_time=0;
			PrintCom("");
			break;
		case CODE_40:		/*���.....��*/
			over_time=0;
			PrintCom("");
			break;
		case CODE_41:		/*���.....��*/
			over_time=0;
			PrintCom("");
			break;
		case CODE_42:		/*���.....��*/
			over_time=0;
			PrintCom("");
			break;
		case CODE_43:		/*���.....��*/
			over_time=0;
			PrintCom("");
			break;
		case CODE_44:		/*���.....��*/
			over_time=0;
			PrintCom("");
			break;
		case CODE_45:		/*���.....��*/
			over_time=0;
			PrintCom("");
			break;
		case CODE_46:		/*���.....��*/
			over_time=0;
			PrintCom("");
			break;
		case CODE_47:		/*���.....��*/
			over_time=0;
			PrintCom("");
			break;
		case CODE_48:		/*���.....��*/
			over_time=0;
			PrintCom("");
			break;
		case CODE_49:		/*���.....��*/
			over_time=0;
			PrintCom("");
			break;
		default:/*text.....*/
			break;
		}
	}
	else
	{
		//PrintCom("��˵��һ������\r\n"); /*text.....*/
	}
}



void tm0_isr() interrupt 1
{
    TL0 = 0x00;		//���ö�ʱ��ֵ
	TH0 = 0x28;		//���ö�ʱ��ֵ
	WDT_CONTR=0x3D;
    if(G0_flag == ENABLE)
	{
		over_time++;
	}
		
}