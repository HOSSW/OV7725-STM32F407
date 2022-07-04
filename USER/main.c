//=========================================电源接线================================================//
//     LCD模块                STM32单片机
//      VCC          接        DC5V/3.3V      //电源
//      GND          接          GND          //电源地
//=======================================液晶屏数据线接线==========================================//
//本模块默认数据总线类型为SPI总线
//     LCD模块                STM32单片机    
//       SDA         接          PE9          //液晶屏SPI总线数据写信号
//       LED         接          PE13         //液晶屏背光控制信号，如果不需要控制，接5V或3.3V
//       SCK         接          PE10          //液晶屏SPI总线时钟信号
//       DC          接          PE14         //液晶屏数据/命令控制信号
//       RESET       接          PE12         //液晶屏复位控制信号
//       CS          接          PE15         //液晶屏片选控制信号
//===================================ov7725摄像头数据线接线========================================//
//     ov7725模块             STM32单片机    
//       VCC         接          DC3.3V       //电源
//       OE          接          PB15         //片选CS
//       RRST        接          PA4          //FIFO 读复位
//       RCLK        接          PA5          //FIFO 读时钟
//       NC          接           -           //不用接 悬空即可
//       VSYNC       接          PA8          //场中断
//       WRST        接          PB7          //FIFO 写复位
//       WEN         接          PB9          //FIFO 写使能
//       HR          接           -           //不用接 悬空即可
//       SDA         接          PD7          //SCCB总线数据信号
//       SCL         接          PD6          //SCCB总线时钟信号
//       D0          接          PC6          //数据0
//       D1          接          PC7          //数据1
//       D2          接          PC8          //数据2
//       D3          接          PC9          //数据3
//       D4          接          PC11         //数据4
//       D5          接          PB6          //数据5
//       D6          接          PE5          //数据6
//       D7          接          PE6          //数据7
//       GND         接          GND	        //电源地
//=======================================PWM输出引脚接线=========================================//
//     舵机模块               STM32单片机  
//       PWM         接         PB14	        //舵机PWM引脚
	
#include "sys.h"
#include "delay.h"
#include "usart.h"
#include "led.h"
#include "lcd.h"    
#include "ov7670.h" 
#include "GUI.h"
#include "EasyTracered.h"
#include "pwm.h"
#include "key.h"
#include "timer.h"
#include "pid.h"


extern u8 ov_sta;
static int i=0;
extern unsigned short sram[128*128];
TARGET_CONDI Conditionred={0,240,0,240,210,240,15,15,128,128};	//白色   API参数  hsl的阈值，识别时用的	
//TARGET_CONDI Conditionred={0,240,0,240,0,10,15,15,128,128};	//黑色   API参数  hsl的阈值，识别时用的	
RESULT Resured;
u16 x,y;
unsigned int LastPwm = 1500;
u8 key,choice,flag;


void camera_refresh(void)
{
	int j; 
	unsigned char picH,picL;
	 
	if(ov_sta)//有帧中断更新？
	{
		LCD_SetWindows(0,0,lcddev.width-1,lcddev.height-1);//显示窗口为全屏	
		LCD_WriteRAM_Prepare();   //开始写入GRAM	
		OV7670_RRST=0;				//开始复位读指针 
		OV7670_RCK_L;
		OV7670_RCK_H;
		OV7670_RCK_L;
		OV7670_RRST=1;				//复位读指针结束 
		OV7670_RCK_H;
			
		for(j=0;j<128*128 - 1;j++)
		{
			OV7670_RCK_L;
			picH=OV7670_DATA;	//读数据
			OV7670_RCK_H;
			
			OV7670_RCK_L;
			picL=OV7670_DATA;	//读数据
			OV7670_RCK_H; 
					
			sram[j] = picH<<8|picL;
			//Gray(&sram[j]);
			Lcd_WriteData_16Bit(sram[j]); 

		}
	 if(Trace(&Conditionred,&Resured))                      //API
		{				
			LCD_Fill(Resured.x-Resured.w/2,Resured.y-Resured.h/2,Resured.x+Resured.w/2,Resured.y-Resured.h/2+1,0xf800);//u16 x,u16 y,u16 width,u16 hight,u16 Color
			LCD_Fill(Resured.x-Resured.w/2,Resured.y-Resured.h/2,Resured.x-Resured.w/2+1,Resured.y+Resured.h/2,0xf800);
			LCD_Fill(Resured.x-Resured.w/2,Resured.y+Resured.h/2,Resured.x+Resured.w/2,Resured.y+Resured.h/2+1,0xf800);
			LCD_Fill(Resured.x+Resured.w/2,Resured.y-Resured.h/2,Resured.x+Resured.w/2+1,Resured.y+Resured.h/2,0xf800);
			LCD_Fill(Resured.x-2,Resured.y-2,Resured.x+2,Resured.y+2,0xf800);

			x=Resured.x;
			y=Resured.y;
		}		
 		ov_sta=0;					//清零帧中断标
		LED1=!LED1;
	}

}
void Steering_Follow() 			//入口参数用于判断模式 0为摄像头跟随，1为搜索
{
	camera_refresh();
	if(x <= 53)
	{
		while(x < 53)
		{
			LastPwm-=15;
			if(LastPwm <= 500)
				LastPwm = 500;
			TIM_SetCompare1(TIM12,LastPwm);
			x=63;
			camera_refresh();
		}
		return;
	}
	else if(x >= 73)
	{
		while(x > 73)
		{			
			LastPwm+=15;
			if(LastPwm >= 2500)
					LastPwm = 2500;
			TIM_SetCompare1(TIM12,LastPwm);
			x=63;
			camera_refresh();
		}
		return;
	}
}
void Steering_Find()
{
	camera_refresh();		//先更新一次	
	TIM_SetCompare1(TIM12,1500);		//先回正
	delay_ms(10);		
	while(1)
	{
		if(flag)	//flag为正
		{
			LastPwm-=10;		//向左转
			if(LastPwm <= 500) //到头了
				flag = 0;	//取反
		}
		else
		{
			LastPwm+=10;		//向右转
			if(LastPwm >= 2500) //到头了
				flag = 1;	//取反
		}
		TIM_SetCompare1(TIM12,LastPwm);
		x=0;
		camera_refresh();
		delay_ms(1);
		if(x != 0)						//如果检测到了物体
		{
			choice = KEY0_PRES;//找到了物体，进入跟随模式修正
			return;							//跳出循环
		}

	}
	
}


int main(void)
{ 
	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2);//设置系统中断优先级分组2
	delay_init(168);  	//初始化延时函数
	uart_init(115200);	//初始化串口波特率为115200
	LED_Init();					//初始化LED 
 	LCD_Init();					//LCD初始化
 	TIM12_PWM_Init(20000-1,84-1);	//84M/84=1000khz的计数频率,重装载值20000，所以PWM频率为1 000 000/20 000=50hz. 

	while(OV7670_Init())					//初始化OV7670
	{
		delay_ms(200);
		LCD_Fill(0,0,lcddev.width,lcddev.height,RED);			//测试屏幕是否好使
		delay_ms(200);
		LED0=!LED0;
	}	
	delay_ms(1000);	 	   
	LCD_Fill(0,0,lcddev.width,lcddev.height,WHITE);			//这测试屏幕是否好使			  
	EXTI8_Init();									//使能定时器捕获
	TIM3_Int_Init(2000-1,8400-1);
	KEY_Init();
	OV7725_Window_Set(128,128,0);	//设置窗口
  OV7670_CS=0;
	choice=KEY0_PRES;
 	while(1)
	{	
		key=KEY_Scan(1);
		if(key>0)
		{
			choice = key;
		}
		camera_refresh();//更新显示
		i++;
		switch(choice)
		{
			case KEY1_PRES://按键2
				Steering_Find();	
				break;
			default:
				 Steering_Follow();		
				break;
		
		}
		if(i==100)//DS0闪烁.
		{
			i=0;
			LED0=!LED0;		
	
 		}
		
	}	   

}
