#include "stm32f10x.h"                  // Device header
#include "public.h"

extern u8 flag_radar;
extern u8 flag;
extern status speed;

void USART1_init(uint32_t bound)
{
	GPIO_InitTypeDef GPIO_InitStruct;   //定义GPIO结构体变量
	USART_InitTypeDef USART_InitStruct;   //定义串口结构体变量
	NVIC_InitTypeDef NVIC_InitStructure;
	
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA|RCC_APB2Periph_USART1,ENABLE);   //使能GPIOC的时钟
	
	GPIO_InitStruct.GPIO_Pin=GPIO_Pin_9;   //配置TX引脚
	GPIO_InitStruct.GPIO_Mode=GPIO_Mode_AF_PP;   //配置PA9为复用推挽输出
	GPIO_InitStruct.GPIO_Speed=GPIO_Speed_50MHz;   //配置PA9速率
	GPIO_Init(GPIOA,&GPIO_InitStruct);   //GPIO初始化函数
	
	GPIO_InitStruct.GPIO_Pin=GPIO_Pin_10;   //配置RX引脚
	GPIO_InitStruct.GPIO_Mode=GPIO_Mode_IN_FLOATING;   //配置PA10为浮空输入
	GPIO_InitStruct.GPIO_Speed=GPIO_Speed_50MHz;   //配置PA10速率
	GPIO_Init(GPIOA,&GPIO_InitStruct);   //GPIO初始化函数

	USART_InitStruct.USART_Mode=USART_Mode_Tx|USART_Mode_Rx;   //发送接收模式
	USART_InitStruct.USART_Parity=USART_Parity_No;   //无奇偶校验
	USART_InitStruct.USART_BaudRate=bound;   //波特率
	USART_InitStruct.USART_StopBits=USART_StopBits_1;   //停止位1位
	USART_InitStruct.USART_WordLength=USART_WordLength_8b;   //字长8位
	USART_InitStruct.USART_HardwareFlowControl=USART_HardwareFlowControl_None;   //无硬件数据流控制
	
	USART_Init(USART1,&USART_InitStruct);   //串口初始化函数
	USART_Cmd(USART1,ENABLE);   //使能USART2
	
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
	GPIO_InitTypeDef GPIO_InitStruct;   //定义GPIO结构体变量
	USART_InitTypeDef USART_InitStruct;   //定义串口结构体变量
	NVIC_InitTypeDef NVIC_InitStructure;
	
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA, ENABLE); //使能GPIOA时钟
  RCC_APB1PeriphClockCmd(RCC_APB1Periph_USART2, ENABLE); //使能USART2

	GPIO_InitStruct.GPIO_Pin=GPIO_Pin_2;   //配置TX引脚
	GPIO_InitStruct.GPIO_Mode=GPIO_Mode_AF_PP;   //配置PA2为复用推挽输出
	GPIO_InitStruct.GPIO_Speed=GPIO_Speed_50MHz;   //配置PA2速率
	GPIO_Init(GPIOA,&GPIO_InitStruct);   //GPIO初始化函数
	
  GPIO_InitStruct.GPIO_Pin=GPIO_Pin_3;   //配置TX引脚
	GPIO_InitStruct.GPIO_Mode=GPIO_Mode_IN_FLOATING;   //配置PA3为复用推挽输出
	GPIO_InitStruct.GPIO_Speed=GPIO_Speed_50MHz;   //配置PA3速率
	GPIO_Init(GPIOA,&GPIO_InitStruct);   //GPIO初始化函数
		
	USART_InitStruct.USART_Mode=USART_Mode_Tx|USART_Mode_Rx;   //发送接收模式
	USART_InitStruct.USART_Parity=USART_Parity_No;   //无奇偶校验
	USART_InitStruct.USART_BaudRate=bound;   //波特率
	USART_InitStruct.USART_StopBits=USART_StopBits_1;   //停止位1位
	USART_InitStruct.USART_WordLength=USART_WordLength_8b;   //字长8位
	USART_InitStruct.USART_HardwareFlowControl=USART_HardwareFlowControl_None;   //无硬件数据流控制
	USART_Init(USART2,&USART_InitStruct);   //串口初始化函数
	
	USART_Init(USART2,&USART_InitStruct);   //串口初始化函数	
	USART_Cmd(USART2,ENABLE);   //使能USART2
	

  NVIC_InitStructure.NVIC_IRQChannel = USART2_IRQn;                     /* USART2中断 */
  NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;                       /* 中断使能 */
  NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0;                    /* 响应优先级0 */   
  NVIC_Init(&NVIC_InitStructure);                                       /* 中断初始化 */
	USART_ITConfig(USART2,USART_IT_RXNE,ENABLE);                        /*空闲中断使能 */	
}


int fputc(int ch,FILE *f)   //printf重定向函数
{
	USART_SendData(USART1,(uint8_t)ch);   //发送一字节数据
	while(USART_GetFlagStatus(USART1,USART_FLAG_TXE) == RESET);   //等待发送完成
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


