#ifndef _key_H
#define _key_H

#include "system.h"

//时钟端口定义
#define KEY1_PORT					GPIOA
#define KEY1_PIN					GPIO_Pin_15
#define KEY1_PORT_RCC			RCC_APB2Periph_GPIOA
/*
#define KEY2_PORT					GPIOA
#define KEY2_PIN					GPIO_Pin_14
#define KEY2_PORT_RCC			RCC_APB2Periph_GPIOA

#define KEY3_PORT					GPIOA
#define KEY3_PIN					GPIO_Pin_13
#define KEY3_PORT_RCC			RCC_APB2Periph_GPIOA
*/
#define KEY4_PORT					GPIOA
#define KEY4_PIN					GPIO_Pin_12
#define KEY4_PORT_RCC			RCC_APB2Periph_GPIOA

//位带定义
#define KEY1 PAin(15)
#define KEY2 PAin(14)
#define KEY3 PAin(13)
#define KEY4 PAin(12)

//定义各个按键值
#define KEY1_PRESS 1
#define KEY2_PRESS 2
#define KEY3_PRESS 3
#define KEY4_PRESS 4



/*矩阵按键相关定义*/
#define KEY_MATRIX_H1_PORT					GPIOB
#define KEY_MATRIX_H1_PIN						GPIO_Pin_8
#define KEY_MATRIX_H1_PORT_RCC			RCC_APB2Periph_GPIOB

#define KEY_MATRIX_H2_PORT					GPIOB
#define KEY_MATRIX_H2_PIN						GPIO_Pin_9
#define KEY_MATRIX_H2_PORT_RCC			RCC_APB2Periph_GPIOB

#define KEY_MATRIX_H3_PORT					GPIOB
#define KEY_MATRIX_H3_PIN						GPIO_Pin_10
#define KEY_MATRIX_H3_PORT_RCC			RCC_APB2Periph_GPIOB

#define KEY_MATRIX_H4_PORT					GPIOB
#define KEY_MATRIX_H4_PIN						GPIO_Pin_11
#define KEY_MATRIX_H4_PORT_RCC			RCC_APB2Periph_GPIOB

#define KEY_MATRIX_L1_PORT					GPIOB
#define KEY_MATRIX_L1_PIN						GPIO_Pin_12
#define KEY_MATRIX_L1_PORT_RCC			RCC_APB2Periph_GPIOB

#define KEY_MATRIX_L2_PORT					GPIOB
#define KEY_MATRIX_L2_PIN						GPIO_Pin_13
#define KEY_MATRIX_L2_PORT_RCC			RCC_APB2Periph_GPIOB

#define KEY_MATRIX_L3_PORT					GPIOB
#define KEY_MATRIX_L3_PIN						GPIO_Pin_14
#define KEY_MATRIX_L3_PORT_RCC			RCC_APB2Periph_GPIOB

#define KEY_MATRIX_L4_PORT					GPIOB
#define KEY_MATRIX_L4_PIN						GPIO_Pin_15
#define KEY_MATRIX_L4_PORT_RCC			RCC_APB2Periph_GPIOB

void KEY_Init(void);
u8 KEY_Scan(u8 mode);
void KEY_Matrix_Init(void);
u8 KEY_Matrix_Scan(void);

#endif
