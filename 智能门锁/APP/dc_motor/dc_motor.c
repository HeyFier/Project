#include "dc_motor.h"

void DC_Motor_Init(void)
{
	//定义结构体变量
	GPIO_InitTypeDef GPIO_InitStructure;
	
	//对端口进行时钟使能
	RCC_APB2PeriphClockCmd(DC_MOTOR_PORT_RCC,ENABLE);

	GPIO_InitStructure.GPIO_Pin = DC_MOTOR_PIN;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	//初始化GPIO
	GPIO_Init(DC_MOTOR_PORT,&GPIO_InitStructure);
	//设置管脚将其拉高使得led处于熄灭的状态
	GPIO_ResetBits(DC_MOTOR_PORT,DC_MOTOR_PIN);
}
