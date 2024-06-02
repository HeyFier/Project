#include "key.h"
#include "SysTick.h"

//独立按键初始化
void KEY_Init(void)
{
	//定义结构体变量
	GPIO_InitTypeDef GPIO_InitStructure;
	
	//对端口进行时钟使能
	RCC_APB2PeriphClockCmd(KEY1_PORT_RCC,ENABLE);
	//RCC_APB2PeriphClockCmd(KEY2_PORT_RCC,ENABLE);
	//RCC_APB2PeriphClockCmd(KEY3_PORT_RCC,ENABLE);
	RCC_APB2PeriphClockCmd(KEY4_PORT_RCC,ENABLE);
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_AFIO,ENABLE);
	GPIO_PinRemapConfig(GPIO_Remap_SWJ_JTAGDisable,ENABLE);
	GPIO_PinRemapConfig(GPIO_Remap_SWJ_Disable,ENABLE);
	
//配置Led管脚的模式 引脚 速度
	//模式为：推挽输出  低电平输出
	GPIO_InitStructure.GPIO_Pin = KEY1_PIN;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPU;
	//初始化GPIO
	GPIO_Init(KEY1_PORT,&GPIO_InitStructure);
	
	/*GPIO_InitStructure.GPIO_Pin = KEY2_PIN;
	GPIO_Init(KEY2_PORT,&GPIO_InitStructure);
	
	GPIO_InitStructure.GPIO_Pin = KEY3_PIN;
	GPIO_Init(KEY3_PORT,&GPIO_InitStructure);*/
	
	GPIO_InitStructure.GPIO_Pin = KEY4_PIN;
	GPIO_Init(KEY4_PORT,&GPIO_InitStructure);
}

//独立按键检测
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

//矩阵按键初始化
void KEY_Matrix_Init(void)
{
	//结构体变量定义
	GPIO_InitTypeDef GPIO_InitStructure;
	//时钟端口使能
	RCC_APB2PeriphClockCmd(KEY_MATRIX_H1_PORT_RCC,ENABLE);
	RCC_APB2PeriphClockCmd(KEY_MATRIX_H2_PORT_RCC,ENABLE);
	RCC_APB2PeriphClockCmd(KEY_MATRIX_H3_PORT_RCC,ENABLE);
	RCC_APB2PeriphClockCmd(KEY_MATRIX_H4_PORT_RCC,ENABLE);
	RCC_APB2PeriphClockCmd(KEY_MATRIX_L1_PORT_RCC,ENABLE);
	RCC_APB2PeriphClockCmd(KEY_MATRIX_L2_PORT_RCC,ENABLE);
	RCC_APB2PeriphClockCmd(KEY_MATRIX_L3_PORT_RCC,ENABLE);
	RCC_APB2PeriphClockCmd(KEY_MATRIX_L4_PORT_RCC,ENABLE);
	
	//列管脚输入配置 下拉输出
	GPIO_InitStructure.GPIO_Pin=KEY_MATRIX_L1_PIN;
	GPIO_InitStructure.GPIO_Mode=GPIO_Mode_IPD;
	GPIO_Init(KEY_MATRIX_L1_PORT,&GPIO_InitStructure);
	
	GPIO_InitStructure.GPIO_Pin=KEY_MATRIX_L2_PIN;
	GPIO_Init(KEY_MATRIX_L2_PORT,&GPIO_InitStructure);
	
	GPIO_InitStructure.GPIO_Pin=KEY_MATRIX_L3_PIN;
	GPIO_Init(KEY_MATRIX_L3_PORT,&GPIO_InitStructure);
	
	GPIO_InitStructure.GPIO_Pin=KEY_MATRIX_L4_PIN;
	GPIO_Init(KEY_MATRIX_L4_PORT,&GPIO_InitStructure);
	
	//行管脚输出配置 推挽输出
	GPIO_InitStructure.GPIO_Pin=KEY_MATRIX_H1_PIN;//选择你要设置的 IO 口
	GPIO_InitStructure.GPIO_Mode=GPIO_Mode_Out_PP;//设置推挽输出模式
	GPIO_InitStructure.GPIO_Speed=GPIO_Speed_50MHz;//设置传输速率
	GPIO_Init(KEY_MATRIX_H1_PORT,&GPIO_InitStructure); /* 初始化 GPIO */
	
	GPIO_InitStructure.GPIO_Pin=KEY_MATRIX_H2_PIN;
	GPIO_Init(KEY_MATRIX_H2_PORT,&GPIO_InitStructure);
	
	GPIO_InitStructure.GPIO_Pin=KEY_MATRIX_H3_PIN;
	GPIO_Init(KEY_MATRIX_H3_PORT,&GPIO_InitStructure);
	
	GPIO_InitStructure.GPIO_Pin=KEY_MATRIX_H4_PIN;
	GPIO_Init(KEY_MATRIX_H4_PORT,&GPIO_InitStructure);
	
}


//矩阵按键检测
u8 KEY_Matrix_Scan(void)
{
	u8 col1,col2,col3,col4;
	u8 key_value=0;
	
	//先让L1~L4输出高电平
	GPIO_SetBits(KEY_MATRIX_H1_PORT,KEY_MATRIX_H1_PIN);
	GPIO_SetBits(KEY_MATRIX_H2_PORT,KEY_MATRIX_H2_PIN);
	GPIO_SetBits(KEY_MATRIX_H3_PORT,KEY_MATRIX_H3_PIN);
	GPIO_SetBits(KEY_MATRIX_H4_PORT,KEY_MATRIX_H4_PIN);
	
	//检测L1~L4是否有输出高电平
	if((GPIO_ReadInputDataBit(KEY_MATRIX_L1_PORT,KEY_MATRIX_L1_PIN) |
			GPIO_ReadInputDataBit(KEY_MATRIX_L2_PORT,KEY_MATRIX_L2_PIN) |
			GPIO_ReadInputDataBit(KEY_MATRIX_L3_PORT,KEY_MATRIX_L3_PIN) |
			GPIO_ReadInputDataBit(KEY_MATRIX_L4_PORT,KEY_MATRIX_L4_PIN))==0)
	{
				return 0;//如果全为0表示没有按键按下
	}
	else
	{
		delay_ms(5);//延时去抖动
		if((GPIO_ReadInputDataBit(KEY_MATRIX_L1_PORT,KEY_MATRIX_L1_PIN)|
			GPIO_ReadInputDataBit(KEY_MATRIX_L2_PORT,KEY_MATRIX_L2_PIN)|
			GPIO_ReadInputDataBit(KEY_MATRIX_L3_PORT,KEY_MATRIX_L3_PIN)|
			GPIO_ReadInputDataBit(KEY_MATRIX_L4_PORT,KEY_MATRIX_L4_PIN))==0)
			return 0;//再次检测防止5ms内断开
	}
	//第一行检测
	GPIO_SetBits(KEY_MATRIX_H1_PORT,KEY_MATRIX_H1_PIN);
	GPIO_ResetBits(KEY_MATRIX_H2_PORT,KEY_MATRIX_H2_PIN);
	GPIO_ResetBits(KEY_MATRIX_H3_PORT,KEY_MATRIX_H3_PIN);	
	GPIO_ResetBits(KEY_MATRIX_H4_PORT,KEY_MATRIX_H4_PIN);
	
	col1=GPIO_ReadInputDataBit(KEY_MATRIX_L1_PORT,KEY_MATRIX_L1_PIN);
	col2=GPIO_ReadInputDataBit(KEY_MATRIX_L2_PORT,KEY_MATRIX_L2_PIN);
	col3=GPIO_ReadInputDataBit(KEY_MATRIX_L3_PORT,KEY_MATRIX_L3_PIN);
	col4=GPIO_ReadInputDataBit(KEY_MATRIX_L4_PORT,KEY_MATRIX_L4_PIN);
	
	if(col1==1&&col2==0&&col3==0&&col4==0)
		key_value=1;
	if(col1==0&&col2==1&&col3==0&&col4==0)
		key_value=2;
	if(col1==0&&col2==0&&col3==1&&col4==0)
		key_value=3;
	if(col1==0&&col2==0&&col3==0&&col4==1)
		key_value=4;
		
 //等待按键松开，不然就一直在这个循环中
	while(((GPIO_ReadInputDataBit(KEY_MATRIX_L1_PORT,KEY_MATRIX_L1_PIN))|
				(GPIO_ReadInputDataBit(KEY_MATRIX_L2_PORT,KEY_MATRIX_L2_PIN))|
				(GPIO_ReadInputDataBit(KEY_MATRIX_L3_PORT,KEY_MATRIX_L3_PIN))|
				(GPIO_ReadInputDataBit(KEY_MATRIX_L4_PORT,KEY_MATRIX_L4_PIN))) > 0);
	
	//第二行检测
	GPIO_ResetBits(KEY_MATRIX_H1_PORT,KEY_MATRIX_H1_PIN);
	GPIO_SetBits(KEY_MATRIX_H2_PORT,KEY_MATRIX_H2_PIN);
	GPIO_ResetBits(KEY_MATRIX_H3_PORT,KEY_MATRIX_H3_PIN);	
	GPIO_ResetBits(KEY_MATRIX_H4_PORT,KEY_MATRIX_H4_PIN);
	
	col1=GPIO_ReadInputDataBit(KEY_MATRIX_L1_PORT,KEY_MATRIX_L1_PIN);
	col2=GPIO_ReadInputDataBit(KEY_MATRIX_L2_PORT,KEY_MATRIX_L2_PIN);
	col3=GPIO_ReadInputDataBit(KEY_MATRIX_L3_PORT,KEY_MATRIX_L3_PIN);
	col4=GPIO_ReadInputDataBit(KEY_MATRIX_L4_PORT,KEY_MATRIX_L4_PIN);
	
	if(col1==1&&col2==0&&col3==0&&col4==0)
		key_value=5;
	if(col1==0&&col2==1&&col3==0&&col4==0)
		key_value=6;
	if(col1==0&&col2==0&&col3==1&&col4==0)
		key_value=7;
	if(col1==0&&col2==0&&col3==0&&col4==1)
		key_value=8;
		
 //等待按键松开，不然就一直在这个循环中
	while(((GPIO_ReadInputDataBit(KEY_MATRIX_L1_PORT,KEY_MATRIX_L1_PIN))|
				(GPIO_ReadInputDataBit(KEY_MATRIX_L2_PORT,KEY_MATRIX_L2_PIN))|
				(GPIO_ReadInputDataBit(KEY_MATRIX_L3_PORT,KEY_MATRIX_L3_PIN))|
				(GPIO_ReadInputDataBit(KEY_MATRIX_L4_PORT,KEY_MATRIX_L4_PIN))) > 0);
	
	//第三行检测
	GPIO_ResetBits(KEY_MATRIX_H1_PORT,KEY_MATRIX_H1_PIN);	
	GPIO_ResetBits(KEY_MATRIX_H2_PORT,KEY_MATRIX_H2_PIN);
	GPIO_SetBits(KEY_MATRIX_H3_PORT,KEY_MATRIX_H3_PIN);
	GPIO_ResetBits(KEY_MATRIX_H4_PORT,KEY_MATRIX_H4_PIN);
	
	
	col1=GPIO_ReadInputDataBit(KEY_MATRIX_L1_PORT,KEY_MATRIX_L1_PIN);
	col2=GPIO_ReadInputDataBit(KEY_MATRIX_L2_PORT,KEY_MATRIX_L2_PIN);
	col3=GPIO_ReadInputDataBit(KEY_MATRIX_L3_PORT,KEY_MATRIX_L3_PIN);
	col4=GPIO_ReadInputDataBit(KEY_MATRIX_L4_PORT,KEY_MATRIX_L4_PIN);
	
	if(col1==1&&col2==0&&col3==0&&col4==0)
		key_value=9;
	if(col1==0&&col2==1&&col3==0&&col4==0)
		key_value=10;
	if(col1==0&&col2==0&&col3==1&&col4==0)
		key_value=11;
	if(col1==0&&col2==0&&col3==0&&col4==1)
		key_value=12;
 //等待按键松开，不然就一直在这个循环中
	while(((GPIO_ReadInputDataBit(KEY_MATRIX_L1_PORT,KEY_MATRIX_L1_PIN))|
				(GPIO_ReadInputDataBit(KEY_MATRIX_L2_PORT,KEY_MATRIX_L2_PIN))|
				(GPIO_ReadInputDataBit(KEY_MATRIX_L3_PORT,KEY_MATRIX_L3_PIN))|
				(GPIO_ReadInputDataBit(KEY_MATRIX_L4_PORT,KEY_MATRIX_L4_PIN))) > 0);
	
	//第四行检测
	GPIO_ResetBits(KEY_MATRIX_H1_PORT,KEY_MATRIX_H1_PIN);
	GPIO_ResetBits(KEY_MATRIX_H2_PORT,KEY_MATRIX_H2_PIN);
	GPIO_ResetBits(KEY_MATRIX_H3_PORT,KEY_MATRIX_H3_PIN);	
	GPIO_SetBits(KEY_MATRIX_H4_PORT,KEY_MATRIX_H4_PIN);
	
	col1=GPIO_ReadInputDataBit(KEY_MATRIX_L1_PORT,KEY_MATRIX_L1_PIN);
	col2=GPIO_ReadInputDataBit(KEY_MATRIX_L2_PORT,KEY_MATRIX_L2_PIN);
	col3=GPIO_ReadInputDataBit(KEY_MATRIX_L3_PORT,KEY_MATRIX_L3_PIN);
	col4=GPIO_ReadInputDataBit(KEY_MATRIX_L4_PORT,KEY_MATRIX_L4_PIN);
	
	if(col1==1&&col2==0&&col3==0&&col4==0)
		key_value=13;
	if(col1==0&&col2==1&&col3==0&&col4==0)
		key_value=14;
	if(col1==0&&col2==0&&col3==1&&col4==0)
		key_value=15;
	if(col1==0&&col2==0&&col3==0&&col4==1)
		key_value=16;
 //等待按键松开，不然就一直在这个循环中
	while(((GPIO_ReadInputDataBit(KEY_MATRIX_L1_PORT,KEY_MATRIX_L1_PIN))|
				(GPIO_ReadInputDataBit(KEY_MATRIX_L2_PORT,KEY_MATRIX_L2_PIN))|
				(GPIO_ReadInputDataBit(KEY_MATRIX_L3_PORT,KEY_MATRIX_L3_PIN))|
				(GPIO_ReadInputDataBit(KEY_MATRIX_L4_PORT,KEY_MATRIX_L4_PIN))) > 0);
	
	return key_value;
}	

