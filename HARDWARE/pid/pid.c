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
	pid.Sv = (Element_Type)(Resured.x * 15.873) + 500;  // x����ת����PWMռ�ձȣ�Ŀ��ֵ��
	pid.Pv = (Element_Type)LastPwm;											//��ǰֵ
	pid.T  = 2;			//PID��������40ms
	
	pid.Tp = 0.05;		//����ϵ��
	pid.Ti = 0;				//����ϵ��
	pid.Td = 0.01;		//΢��ϵ��

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
		
		pid.Ek	 =  pid.Sv - pid.Pv;		//��ǰ���
		Delta_Ek =  pid.Ek - pid.Ek_1;	//�ϴ����͵�ǰ���֮��
						

		Pout = pid.Tp * pid.Ek;
		Dout = pid.Td * Delta_Ek;
		
		pid.OUT = Pout - Dout;
		
		pid.Ek_1 = pid.Ek;					//�������ֵ 
	}	
}

