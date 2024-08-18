#include "stm32f10x.h"                  // Device header
#include "PID.h"

PID_typedef pid_x; // x轴舵机
PID_typedef pid_y; // y轴舵机

void PID_Init(void)
{
	pid_x.Err = 0.0;
	pid_x.Err_difference = 0.0;
	pid_x.Err_sum = 0.0 ;
	pid_x.Last_err = 0.0;
	pid_x.Kd = 0.05;
	pid_x.Ki = 0.0005;
	pid_x.Kp = 0.05;

	pid_y.Err = 0.0;
	pid_y.Err_difference = 0.0;
	pid_y.Err_sum = 0.0 ;
	pid_y.Last_err = 0.0;
	pid_y.Kd = 0.05;
	pid_y.Ki = 0.0005;
	pid_y.Kp = 0.05;

}

float PID_value_x(float measure, float calcu)
{
	pid_x.Err = measure - calcu; // 误差
	pid_x.Err_sum += pid_x.Err; // 误差和
	pid_x.Err_difference = pid_x.Err -pid_x.Last_err; // 误差差值
	pid_x.Last_err = pid_x.Err;
	
	return pid_x.Kp*pid_x.Err + pid_x.Ki*pid_x.Err_sum +pid_x.Kd*pid_x.Err_difference;
}

float PID_value_y(float measure, float calcu)
{
	pid_y.Err = measure - calcu; // 误差
	pid_y.Err_sum += pid_y.Err; // 误差和
	pid_y.Err_difference = pid_y.Err -pid_y.Last_err; // 误差差值
	pid_y.Last_err = pid_y.Err;
	
	return pid_y.Kp*pid_y.Err + pid_y.Ki*pid_y.Err_sum +pid_y.Kd*pid_y.Err_difference;
}
