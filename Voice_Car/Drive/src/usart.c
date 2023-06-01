#include "stm32f10x.h"                  // Device header
#include "public.h"

extern u8 flag_radar;
extern u8 flag;
extern status speed;

void USART1_init(uint32_t bound)
{
	GPIO_InitTypeDef GPIO_InitStruct;   //����GPIO�ṹ�����
	USART_InitTypeDef USART_InitStruct;   //���崮�ڽṹ�����
	NVIC_InitTypeDef NVIC_InitStructure;
	
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA|RCC_APB2Periph_USART1,ENABLE);   //ʹ��GPIOC��ʱ��
	
	GPIO_InitStruct.GPIO_Pin=GPIO_Pin_9;   //����TX����
	GPIO_InitStruct.GPIO_Mode=GPIO_Mode_AF_PP;   //����PA9Ϊ�����������
	GPIO_InitStruct.GPIO_Speed=GPIO_Speed_50MHz;   //����PA9����
	GPIO_Init(GPIOA,&GPIO_InitStruct);   //GPIO��ʼ������
	
	GPIO_InitStruct.GPIO_Pin=GPIO_Pin_10;   //����RX����
	GPIO_InitStruct.GPIO_Mode=GPIO_Mode_IN_FLOATING;   //����PA10Ϊ��������
	GPIO_InitStruct.GPIO_Speed=GPIO_Speed_50MHz;   //����PA10����
	GPIO_Init(GPIOA,&GPIO_InitStruct);   //GPIO��ʼ������

	USART_InitStruct.USART_Mode=USART_Mode_Tx|USART_Mode_Rx;   //���ͽ���ģʽ
	USART_InitStruct.USART_Parity=USART_Parity_No;   //����żУ��
	USART_InitStruct.USART_BaudRate=bound;   //������
	USART_InitStruct.USART_StopBits=USART_StopBits_1;   //ֹͣλ1λ
	USART_InitStruct.USART_WordLength=USART_WordLength_8b;   //�ֳ�8λ
	USART_InitStruct.USART_HardwareFlowControl=USART_HardwareFlowControl_None;   //��Ӳ������������
	
	USART_Init(USART1,&USART_InitStruct);   //���ڳ�ʼ������
	USART_Cmd(USART1,ENABLE);   //ʹ��USART2
	
  NVIC_PriorityGroupConfig(NVIC_PriorityGroup_0);     
  NVIC_InitStructure.NVIC_IRQChannel = USART1_IRQn;     
  NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0;     
  NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0;     
  NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;     
  NVIC_Init(&NVIC_InitStructure);  
	USART_ITConfig(USART1,USART_IT_RXNE,ENABLE);

}

void USART2_init(uint32_t bound)
{
	GPIO_InitTypeDef GPIO_InitStruct;   //����GPIO�ṹ�����
	USART_InitTypeDef USART_InitStruct;   //���崮�ڽṹ�����
	NVIC_InitTypeDef NVIC_InitStructure;
	
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA, ENABLE); //ʹ��GPIOAʱ��
  RCC_APB1PeriphClockCmd(RCC_APB1Periph_USART2, ENABLE); //ʹ��USART2

	GPIO_InitStruct.GPIO_Pin=GPIO_Pin_2;   //����TX����
	GPIO_InitStruct.GPIO_Mode=GPIO_Mode_AF_PP;   //����PA2Ϊ�����������
	GPIO_InitStruct.GPIO_Speed=GPIO_Speed_50MHz;   //����PA2����
	GPIO_Init(GPIOA,&GPIO_InitStruct);   //GPIO��ʼ������
	
  GPIO_InitStruct.GPIO_Pin=GPIO_Pin_3;   //����TX����
	GPIO_InitStruct.GPIO_Mode=GPIO_Mode_IN_FLOATING;   //����PA3Ϊ�����������
	GPIO_InitStruct.GPIO_Speed=GPIO_Speed_50MHz;   //����PA3����
	GPIO_Init(GPIOA,&GPIO_InitStruct);   //GPIO��ʼ������
		
	USART_InitStruct.USART_Mode=USART_Mode_Tx|USART_Mode_Rx;   //���ͽ���ģʽ
	USART_InitStruct.USART_Parity=USART_Parity_No;   //����żУ��
	USART_InitStruct.USART_BaudRate=bound;   //������
	USART_InitStruct.USART_StopBits=USART_StopBits_1;   //ֹͣλ1λ
	USART_InitStruct.USART_WordLength=USART_WordLength_8b;   //�ֳ�8λ
	USART_InitStruct.USART_HardwareFlowControl=USART_HardwareFlowControl_None;   //��Ӳ������������
	USART_Init(USART2,&USART_InitStruct);   //���ڳ�ʼ������
	
	USART_Init(USART2,&USART_InitStruct);   //���ڳ�ʼ������	
	USART_Cmd(USART2,ENABLE);   //ʹ��USART2
	

  NVIC_InitStructure.NVIC_IRQChannel = USART2_IRQn;                     /* USART2�ж� */
  NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;                       /* �ж�ʹ�� */
  NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0;                    /* ��Ӧ���ȼ�0 */   
  NVIC_Init(&NVIC_InitStructure);                                       /* �жϳ�ʼ�� */
	USART_ITConfig(USART2,USART_IT_RXNE,ENABLE);                        /*�����ж�ʹ�� */	
}


int fputc(int ch,FILE *f)   //printf�ض�����
{
	USART_SendData(USART1,(uint8_t)ch);   //����һ�ֽ�����
	while(USART_GetFlagStatus(USART1,USART_FLAG_TXE) == RESET);   //�ȴ��������
	return ch;
}

void USART2_IRQHandler(void)
{    		
    if(USART_GetITStatus(USART2, USART_IT_RXNE) == SET)
    {
      char rx_data = USART_ReceiveData(USART2);
				  switch(rx_data)
              {
					case '0':  flag_radar=0;  break  ;
					case '1':  flag_radar=1;  break  ;
					case '2':  speed=50;      break  ;
					case '3':  speed=100;     break  ;
					case '4':  speed=175;     break  ;
					case '5':  speed=225;     break  ;
					case '6':  speed=300;     break  ;								
					default:   speed=300;     break  ;                                                                                                
	          }	
        USART_ClearITPendingBit(USART2, USART_IT_RXNE);       
    }
}

 
void USART1_IRQHandler(void)
{  
    if(USART_GetITStatus(USART1, USART_IT_RXNE) == SET)
    {
      char rx_data = USART_ReceiveData(USART1);
		  //USART_SendData(USART1,rx_data);
				  switch(rx_data)
              {
					case '0':Car_Stop();
                   flag=0;
   								 break;
					case '1':Car_Run(speed);
                   flag=1;          								
								   break;
					case '2':Car_Back(speed);
                   flag=2;								
								   break;
					case '3':Car_Right(speed);
                   flag=3;								
								   break;
					case '4':Car_Left(speed);
                   flag=4;								
								   break;
          default: Car_Stop();
                   flag=0;								
								   break;                                                                                                
	          }	
        USART_ClearITPendingBit(USART1, USART_IT_RXNE);
        
    }
}


