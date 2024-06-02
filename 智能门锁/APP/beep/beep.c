#include "beep.h"
#include "SysTick.h"

void BEEP_Init(void)
{
	//定义结构体变量
	GPIO_InitTypeDef GPIO_InitStructure;
	//对LED端口进行时钟使能
	RCC_APB2PeriphClockCmd(BEEP_PORT_RCC,ENABLE);
	//配置GPIO的模式和IO口
	//模式为：推挽输出  低电平输出
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
	GPIO_InitStructure.GPIO_Pin = BEEP_PIN;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	//初始化GPIO
	GPIO_Init(BEEP_PORT,&GPIO_InitStructure);
	//给低电平，对应输出高阻态 蜂鸣器不会得电
	GPIO_ResetBits(BEEP_PORT,BEEP_PIN);
}
//蜂鸣器报警函数实现
/*实现思路：通过传递不同的us值从而改变不同频率输出高低电平*/
//times:报警次数即高低电平次数
//us:延时微秒时间
void BEEP_Alarm(u8 times,u8 us)
{
	while(times--)
	{
		BEEP=!BEEP;//翻转电平
		delay_us(us);
	}
}
