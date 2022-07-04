#include "PID.h"
#include "pwm.h"
#include "EasyTracered.h"

extern RESULT Resured;
extern unsigned int LastPwm;
PID pid; //storage PID data

/************************************************/
/*		X				 0 			63		 128	*/
/*	PWM			 500		1500		2500	*/
/************************************************/

void PID_Init(void)    
{
	pid.Sv = (Element_Type)(Resured.x * 15.873) + 500;  // x坐标转换成PWM占空比（目标值）
	pid.Pv = (Element_Type)LastPwm;											//当前值
	pid.T  = 2;			//PID计算周期40ms
	
	pid.Tp = 0.05;		//比例系数
	pid.Ti = 0;				//积分系数
	pid.Td = 0.01;		//微分系数

	pid.OUT0 = 0;
}

void OUT_Position_Calculate(void)
{
	if (pid.time < pid.T)
		return;
	else
	{
		pid.time = 0;		//time count reset
		
		Element_Type Delta_Ek; 
		Element_Type Pout, Dout;
		
		pid.Ek	 =  pid.Sv - pid.Pv;		//当前误差
		Delta_Ek =  pid.Ek - pid.Ek_1;	//上次误差和当前误差之差
						

		Pout = pid.Tp * pid.Ek;
		Dout = pid.Td * Delta_Ek;
		
		pid.OUT = Pout - Dout;
		
		pid.Ek_1 = pid.Ek;					//更新误差值 
	}	
}

