#include "stm32f10x.h"                  // Device header
#include "Delay.h"
#include "OLED.h"
#include "Serial.h"
#include "Servo.h"
#include "PID.h"

// 中心位置80, 75
#define angle_X_center 80.0 
#define angle_Y_center 75.0

int main(void)
{
	OLED_Init();
	Serial_Init();
	Servo_Init();
	PID_Init();
	
	float angle_X = 50.0;
	float PID_ResultX = 0.0;
	Servo_SetAngle_X(angle_X);
	
	// float angle_Y = 30.0;
	// float PID_ResultY = 0.0;
	// Servo_SetAngle_Y(angle_Y);
	
	OLED_ShowString(1,1,"ObjectData");
	OLED_ShowString(2,1,"X:");
	OLED_ShowString(2,7,"Y:");
	OLED_ShowString(3,1,"AngleX:");
	// OLED_ShowString(4,1,"AngleY:");
	
	// 查询串口数据
	while(1){
		if(Serial_K210DataFlag()==1)	
		{	
			// 数据显示
			OLED_ShowNum(2,3,Serial_RxPacket[0],3);
			OLED_ShowNum(2,9,Serial_RxPacket[1],3);
			
			// X轴PID处理
			PID_ResultX = PID_value_x(angle_X_center,(float)Serial_RxPacket[0]);
			angle_X+=PID_ResultX;
			// 防止角度过量
			if(angle_X>170){angle_X = 170;}
			if(angle_X<10){angle_X = 10;}
			// 舵机移动
			Servo_SetAngle_X(angle_X);
			OLED_ShowNum(3,8,angle_X,3);
			
			/*   
			Y轴PID处理
			PID_ResultY = PID_value_y(angle_Y_center,(float)Serial_RxPacket[1]);
			angle_Y+=PID_ResultY;
			if(angle_Y>170){angle_Y = 170;}
			if(angle_Y<10){angle_Y = 10;}
			Servo_SetAngle_Y(angle_Y);
			OLED_ShowNum(4,8,angle_Y,3);
			*/
		}
		
	}		

}	
