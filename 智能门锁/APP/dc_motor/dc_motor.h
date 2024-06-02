#ifndef _dc_motor_H
#define _dc_motor_H
#include "system.h"

//时钟端口定义
#define DC_MOTOR_PORT				GPIOB
#define DC_MOTOR_PIN				GPIO_Pin_8 
#define DC_MOTOR_PORT_RCC		RCC_APB2Periph_GPIOB //时钟端口定义

#define DC_MOTOR PBout(8)

void DC_Motor_Init(void);

#endif
