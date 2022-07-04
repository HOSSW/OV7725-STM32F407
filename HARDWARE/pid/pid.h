#ifndef __PID_H
#define __PID_H
#include "stm32f4xx.h"        

typedef float Element_Type;
typedef struct
{        
	Element_Type Sv; 	//设置值
	Element_Type Pv; 	//当前值
	Element_Type T;		//pid计算周期	
	
	Element_Type Tp; 	//p算法系数 
	Element_Type Ti;	//i算法系数
	Element_Type Td;	//d算法系数
	
	Element_Type Ek;	//这次误差
	Element_Type Ek_1;  //上次误差
	Element_Type Ek_2;	//上上次误差
	Element_Type S_Ek;  //历史误差之和
	
	Element_Type OUT0;	//输出补偿常数 
	Element_Type OUT;	//输出结果 
	unsigned char time;

} PID;


void PID_Init(void);
void OUT_Position_Calculate(void);

#endif // __PID_H
