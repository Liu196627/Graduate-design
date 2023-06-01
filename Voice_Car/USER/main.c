#include "stm32f10x.h"                
#include "public.h"        

u8 flag;
status speed=300;
u8 flag_radar=0;


/*超声波用计算数字的长度*/
uint64_t numlen(uint64_t num)
{
    uint64_t len = 1;        // 初始长度为1
    for(; num > 9; ++len)    // 判断num是否大于9，否则长度+1
        num /= 10;	         // 使用除法进行运算，直到num小于1
    return len;              // 返回长度的值
}


int main(void)
{
	LED_Init();   //LED初始化
	Delay_Init();   //延时初始化
	motor();
	USART1_init(9600);   //蓝牙串口初始化，入口参数为波特率9600		 
	USART2_init(9600);   //LD3320串口初始化，入口参数为波特率9600		
	TIM3_PWM_Init();     //输出pwm的定时器初始化
	OLED_Init();         //屏幕初始化
	OLED_ColorTurn(0);//0正常显示，1 反色显示
  OLED_DisplayTurn(0);//0正常显示 1 屏幕翻转显示
	
	Timer_Init();		//初始化超声波定时器
	HC_SR04_Init();		//初始化超声波测距模块
	
	while(1){

		OLED_Refresh();
		OLED_ShowNum(50,0,626,3,16,1);
		/*显示速度*/
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
							
			/*显示运动状态*/			
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
    /*显示雷达的数据*/
		if(flag_radar==1){
    OLED_Cls(48,64);
		OLED_ShowString(0,48,"Radar:",16,1);
    OLED_ShowString(70,48,"off",16,1);			
		}else{
		 int Distance_mm=sonar_mm();			//获取距离测量结果，单位毫米（mm）	
     if(Distance_mm<150){			
	   USART_SendData(USART1,'a');   //发送一字节数据
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

