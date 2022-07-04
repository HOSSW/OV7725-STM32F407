#ifndef __PID_H
#define __PID_H
#include "stm32f4xx.h"        

typedef float Element_Type;
typedef struct
{        
	Element_Type Sv; 	//����ֵ
	Element_Type Pv; 	//��ǰֵ
	Element_Type T;		//pid��������	
	
	Element_Type Tp; 	//p�㷨ϵ�� 
	Element_Type Ti;	//i�㷨ϵ��
	Element_Type Td;	//d�㷨ϵ��
	
	Element_Type Ek;	//������
	Element_Type Ek_1;  //�ϴ����
	Element_Type Ek_2;	//���ϴ����
	Element_Type S_Ek;  //��ʷ���֮��
	
	Element_Type OUT0;	//����������� 
	Element_Type OUT;	//������ 
	unsigned char time;

} PID;


void PID_Init(void);
void OUT_Position_Calculate(void);

#endif // __PID_H
