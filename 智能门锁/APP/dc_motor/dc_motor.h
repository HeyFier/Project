#ifndef _dc_motor_H
#define _dc_motor_H
#include "system.h"

//ʱ�Ӷ˿ڶ���
#define DC_MOTOR_PORT				GPIOB
#define DC_MOTOR_PIN				GPIO_Pin_8 
#define DC_MOTOR_PORT_RCC		RCC_APB2Periph_GPIOB //ʱ�Ӷ˿ڶ���

#define DC_MOTOR PBout(8)

void DC_Motor_Init(void);

#endif
