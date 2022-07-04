//=========================================��Դ����================================================//
//     LCDģ��                STM32��Ƭ��
//      VCC          ��        DC5V/3.3V      //��Դ
//      GND          ��          GND          //��Դ��
//=======================================Һ���������߽���==========================================//
//��ģ��Ĭ��������������ΪSPI����
//     LCDģ��                STM32��Ƭ��    
//       SDA         ��          PE9          //Һ����SPI��������д�ź�
//       LED         ��          PE13         //Һ������������źţ��������Ҫ���ƣ���5V��3.3V
//       SCK         ��          PE10          //Һ����SPI����ʱ���ź�
//       DC          ��          PE14         //Һ��������/��������ź�
//       RESET       ��          PE12         //Һ������λ�����ź�
//       CS          ��          PE15         //Һ����Ƭѡ�����ź�
//===================================ov7725����ͷ�����߽���========================================//
//     ov7725ģ��             STM32��Ƭ��    
//       VCC         ��          DC3.3V       //��Դ
//       OE          ��          PB15         //ƬѡCS
//       RRST        ��          PA4          //FIFO ����λ
//       RCLK        ��          PA5          //FIFO ��ʱ��
//       NC          ��           -           //���ý� ���ռ���
//       VSYNC       ��          PA8          //���ж�
//       WRST        ��          PB7          //FIFO д��λ
//       WEN         ��          PB9          //FIFO дʹ��
//       HR          ��           -           //���ý� ���ռ���
//       SDA         ��          PD7          //SCCB���������ź�
//       SCL         ��          PD6          //SCCB����ʱ���ź�
//       D0          ��          PC6          //����0
//       D1          ��          PC7          //����1
//       D2          ��          PC8          //����2
//       D3          ��          PC9          //����3
//       D4          ��          PC11         //����4
//       D5          ��          PB6          //����5
//       D6          ��          PE5          //����6
//       D7          ��          PE6          //����7
//       GND         ��          GND	        //��Դ��
//=======================================PWM������Ž���=========================================//
//     ���ģ��               STM32��Ƭ��  
//       PWM         ��         PB14	        //���PWM����
	
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
TARGET_CONDI Conditionred={0,240,0,240,210,240,15,15,128,128};	//��ɫ   API����  hsl����ֵ��ʶ��ʱ�õ�	
//TARGET_CONDI Conditionred={0,240,0,240,0,10,15,15,128,128};	//��ɫ   API����  hsl����ֵ��ʶ��ʱ�õ�	
RESULT Resured;
u16 x,y;
unsigned int LastPwm = 1500;
u8 key,choice,flag;


void camera_refresh(void)
{
	int j; 
	unsigned char picH,picL;
	 
	if(ov_sta)//��֡�жϸ��£�
	{
		LCD_SetWindows(0,0,lcddev.width-1,lcddev.height-1);//��ʾ����Ϊȫ��	
		LCD_WriteRAM_Prepare();   //��ʼд��GRAM	
		OV7670_RRST=0;				//��ʼ��λ��ָ�� 
		OV7670_RCK_L;
		OV7670_RCK_H;
		OV7670_RCK_L;
		OV7670_RRST=1;				//��λ��ָ����� 
		OV7670_RCK_H;
			
		for(j=0;j<128*128 - 1;j++)
		{
			OV7670_RCK_L;
			picH=OV7670_DATA;	//������
			OV7670_RCK_H;
			
			OV7670_RCK_L;
			picL=OV7670_DATA;	//������
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
 		ov_sta=0;					//����֡�жϱ�
		LED1=!LED1;
	}

}
void Steering_Follow() 			//��ڲ��������ж�ģʽ 0Ϊ����ͷ���棬1Ϊ����
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
	camera_refresh();		//�ȸ���һ��	
	TIM_SetCompare1(TIM12,1500);		//�Ȼ���
	delay_ms(10);		
	while(1)
	{
		if(flag)	//flagΪ��
		{
			LastPwm-=10;		//����ת
			if(LastPwm <= 500) //��ͷ��
				flag = 0;	//ȡ��
		}
		else
		{
			LastPwm+=10;		//����ת
			if(LastPwm >= 2500) //��ͷ��
				flag = 1;	//ȡ��
		}
		TIM_SetCompare1(TIM12,LastPwm);
		x=0;
		camera_refresh();
		delay_ms(1);
		if(x != 0)						//�����⵽������
		{
			choice = KEY0_PRES;//�ҵ������壬�������ģʽ����
			return;							//����ѭ��
		}

	}
	
}


int main(void)
{ 
	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2);//����ϵͳ�ж����ȼ�����2
	delay_init(168);  	//��ʼ����ʱ����
	uart_init(115200);	//��ʼ�����ڲ�����Ϊ115200
	LED_Init();					//��ʼ��LED 
 	LCD_Init();					//LCD��ʼ��
 	TIM12_PWM_Init(20000-1,84-1);	//84M/84=1000khz�ļ���Ƶ��,��װ��ֵ20000������PWMƵ��Ϊ1 000 000/20 000=50hz. 

	while(OV7670_Init())					//��ʼ��OV7670
	{
		delay_ms(200);
		LCD_Fill(0,0,lcddev.width,lcddev.height,RED);			//������Ļ�Ƿ��ʹ
		delay_ms(200);
		LED0=!LED0;
	}	
	delay_ms(1000);	 	   
	LCD_Fill(0,0,lcddev.width,lcddev.height,WHITE);			//�������Ļ�Ƿ��ʹ			  
	EXTI8_Init();									//ʹ�ܶ�ʱ������
	TIM3_Int_Init(2000-1,8400-1);
	KEY_Init();
	OV7725_Window_Set(128,128,0);	//���ô���
  OV7670_CS=0;
	choice=KEY0_PRES;
 	while(1)
	{	
		key=KEY_Scan(1);
		if(key>0)
		{
			choice = key;
		}
		camera_refresh();//������ʾ
		i++;
		switch(choice)
		{
			case KEY1_PRES://����2
				Steering_Find();	
				break;
			default:
				 Steering_Follow();		
				break;
		
		}
		if(i==100)//DS0��˸.
		{
			i=0;
			LED0=!LED0;		
	
 		}
		
	}	   

}
