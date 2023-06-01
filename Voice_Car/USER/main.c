#include "stm32f10x.h"                
#include "public.h"        

u8 flag;
status speed=300;
u8 flag_radar=0;


/*�������ü������ֵĳ���*/
uint64_t numlen(uint64_t num)
{
    uint64_t len = 1;        // ��ʼ����Ϊ1
    for(; num > 9; ++len)    // �ж�num�Ƿ����9�����򳤶�+1
        num /= 10;	         // ʹ�ó����������㣬ֱ��numС��1
    return len;              // ���س��ȵ�ֵ
}


int main(void)
{
	LED_Init();   //LED��ʼ��
	Delay_Init();   //��ʱ��ʼ��
	motor();
	USART1_init(9600);   //�������ڳ�ʼ������ڲ���Ϊ������9600		 
	USART2_init(9600);   //LD3320���ڳ�ʼ������ڲ���Ϊ������9600		
	TIM3_PWM_Init();     //���pwm�Ķ�ʱ����ʼ��
	OLED_Init();         //��Ļ��ʼ��
	OLED_ColorTurn(0);//0������ʾ��1 ��ɫ��ʾ
  OLED_DisplayTurn(0);//0������ʾ 1 ��Ļ��ת��ʾ
	
	Timer_Init();		//��ʼ����������ʱ��
	HC_SR04_Init();		//��ʼ�����������ģ��
	
	while(1){

		OLED_Refresh();
		OLED_ShowNum(50,0,626,3,16,1);
		/*��ʾ�ٶ�*/
 					  switch(speed)
              {
					case 300:	
           			OLED_Cls(16,64);	
                OLED_ShowString(0,16,"Speed:",16,1);    					
		            OLED_ShowNum(70,16,15,2,16,1);
		            OLED_ShowString(102,16,"%",16,1); 					
   								 break;
					case 225:
           			OLED_Cls(16,64);	
                OLED_ShowString(0,16,"Speed:",16,1); 						
		            OLED_ShowNum(70,16,25,2,16,1);
		            OLED_ShowString(102,16,"%",16,1); 					
								   break;
					case 175:	
           			OLED_Cls(16,64);	
                OLED_ShowString(0,16,"Speed:",16,1); 						
		            OLED_ShowNum(70,16,50,2,16,1);
		            OLED_ShowString(102,16,"%",16,1); 					
								   break;
					case 100:
           			OLED_Cls(16,64);	
                OLED_ShowString(0,16,"Speed:",16,1); 						
		            OLED_ShowNum(70,16,75,2,16,1);
		            OLED_ShowString(102,16,"%",16,1); 					
								   break;
					case 50:
           			OLED_Cls(16,64);	
                OLED_ShowString(0,16,"Speed:",16,1); 						
		            OLED_ShowNum(70,16,100,3,16,1);	
		            OLED_ShowString(102,16,"%",16,1); 					
								   break;
           default:
           			OLED_Cls(16,64);	
                OLED_ShowString(0,16,"Speed:",16,1); 						 
		            OLED_ShowNum(70,16,15,3,16,1);	
		            OLED_ShowString(102,16,"%",16,1); 					 
								   break;                                                                                                
	          }	 
							
			/*��ʾ�˶�״̬*/			
				  switch(flag)
              {
					case 0:	
           			OLED_Cls(32,64);
                OLED_ShowString(0,32,"Stadus:",16,1);  					
						    OLED_ShowChinese(70,32,7,16,1);
								OLED_ShowChinese(86,32,8,16,1);		
   								 break;
					case 1:	
           			OLED_Cls(32,64);
                OLED_ShowString(0,32,"Stadus:",16,1); 						
						    OLED_ShowChinese(70,32,0,16,1);
								OLED_ShowChinese(86,32,1,16,1);		        								
								   break;
					case 2:
           			OLED_Cls(32,64);
                OLED_ShowString(0,32,"Stadus:",16,1); 						
						    OLED_ShowChinese(70,32,5,16,1);
								OLED_ShowChinese(86,32,6,16,1);									
								   break;
					case 3:	
           			OLED_Cls(32,64);
                OLED_ShowString(0,32,"Stadus:",16,1); 						
						    OLED_ShowChinese(70,32,3,16,1);
								OLED_ShowChinese(86,32,4,16,1);										
								   break;
					case 4:	
           			OLED_Cls(32,64);
                OLED_ShowString(0,32,"Stadus:",16,1); 						
						    OLED_ShowChinese(70,32,2,16,1);
								OLED_ShowChinese(86,32,4,16,1);					
								   break;
           default: 
           			OLED_Cls(32,64);
                OLED_ShowString(0,32,"Stadus:",16,1); 						 
								OLED_ShowChinese(70,32,2,16,1);
								OLED_ShowChinese(86,32,3,16,1);									
								   break;                                                                                                
	          }	 
    /*��ʾ�״������*/
		if(flag_radar==1){
    OLED_Cls(48,64);
		OLED_ShowString(0,48,"Radar:",16,1);
    OLED_ShowString(70,48,"off",16,1);			
		}else{
		 int Distance_mm=sonar_mm();			//��ȡ��������������λ���ף�mm��	
     if(Distance_mm<150){			
	   USART_SendData(USART1,'a');   //����һ�ֽ�����
     }else{
		   USART_SendData(USART1,'b');
		  }	
    OLED_Cls(48,64);
		OLED_ShowString(0,48,"Radar:",16,1);
		OLED_ShowNum(70,48,Distance_mm,numlen(Distance_mm),16,1);
		OLED_ShowString(102,48,"mm",16,1); 		
		
		}
 

  }
}

