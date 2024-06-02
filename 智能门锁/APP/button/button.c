#include "button.h"
#include "SysTick.h"

void KEY_Init(void)
{
	//定义结构体变量
	GPIO_InitTypeDef GPIO_InitStructure;
	
	//对LED端口进行时钟使能
	RCC_APB2PeriphClockCmd(KEY1_PORT_RCC,ENABLE);
	RCC_APB2PeriphClockCmd(KEY2_PORT_RCC,ENABLE);
	RCC_APB2PeriphClockCmd(KEY3_PORT_RCC,ENABLE);
	RCC_APB2PeriphClockCmd(KEY4_PORT_RCC,ENABLE);
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_AFIO,ENABLE );
	GPIO_PinRemapConfig(GPIO_Remap_SWJ_JTAGDisable,ENABLE );
	GPIO_PinRemapConfig(GPIO_Remap_SWJ_Disable,ENABLE );
	
//配置Led管脚的模式 引脚 速度
	//模式为：推挽输出  低电平输出
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPU;
	GPIO_InitStructure.GPIO_Pin = KEY1_PIN;
	//初始化GPIO
	GPIO_Init(KEY1_PORT,&GPIO_InitStructure);
	
	GPIO_InitStructure.GPIO_Pin = KEY2_PIN;
	GPIO_Init(KEY2_PORT,&GPIO_InitStructure);
	
	GPIO_InitStructure.GPIO_Pin = KEY3_PIN;
	GPIO_Init(KEY3_PORT,&GPIO_InitStructure);
	
	GPIO_InitStructure.GPIO_Pin = KEY4_PIN;
	GPIO_Init(KEY4_PORT,&GPIO_InitStructure);
}

//按键按下电平为低电平
u8 KEY_Scan(u8 mode)
{
	static u8 key=1;
	
	//让其按下并扫描
	if(mode==1)
		key=1;
	//判断按键按下
	if(key==1&&(KEY1==0||KEY2==0||KEY3==0||KEY4==0))
	{
		delay_ms(10);//消抖
		key=0;//防止按键再次被按下又进入if检测，达到按键识别一次的目的
		if(KEY1==0)
			return KEY1_PRESS;
		else if(KEY2==0)
			return KEY2_PRESS;
		else if(KEY3==0)
			return KEY3_PRESS;
		else if(KEY4==0)
			return KEY4_PRESS;
	}
	//判断按键松开
	else if(KEY1==1&&KEY2==1&&KEY3==1&&KEY4==1)
	{
		key=1;//恢复key现在可以再次按下了
	}
	return 0;
}
