#include "stm32f10x.h"                  // Device header
#include "PWM.h"

void Servo_Init(void)
{
	PWM_Init();	
}


void Servo_SetAngle_X(float Angle)
{
	PWM_SetCompare2_TIM2(Angle/180*2000+500);
}

void Servo_SetAngle_Y(float Angle)
{
	PWM_SetCompare2_TIM3(Angle/180*2000+500);
}
