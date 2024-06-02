#include "button.h"
#include "SysTick.h"

void KEY_Init(void)
{
	//����ṹ�����
	GPIO_InitTypeDef GPIO_InitStructure;
	
	//��LED�˿ڽ���ʱ��ʹ��
	RCC_APB2PeriphClockCmd(KEY1_PORT_RCC,ENABLE);
	RCC_APB2PeriphClockCmd(KEY2_PORT_RCC,ENABLE);
	RCC_APB2PeriphClockCmd(KEY3_PORT_RCC,ENABLE);
	RCC_APB2PeriphClockCmd(KEY4_PORT_RCC,ENABLE);
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_AFIO,ENABLE );
	GPIO_PinRemapConfig(GPIO_Remap_SWJ_JTAGDisable,ENABLE );
	GPIO_PinRemapConfig(GPIO_Remap_SWJ_Disable,ENABLE );
	
//����Led�ܽŵ�ģʽ ���� �ٶ�
	//ģʽΪ���������  �͵�ƽ���
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPU;
	GPIO_InitStructure.GPIO_Pin = KEY1_PIN;
	//��ʼ��GPIO
	GPIO_Init(KEY1_PORT,&GPIO_InitStructure);
	
	GPIO_InitStructure.GPIO_Pin = KEY2_PIN;
	GPIO_Init(KEY2_PORT,&GPIO_InitStructure);
	
	GPIO_InitStructure.GPIO_Pin = KEY3_PIN;
	GPIO_Init(KEY3_PORT,&GPIO_InitStructure);
	
	GPIO_InitStructure.GPIO_Pin = KEY4_PIN;
	GPIO_Init(KEY4_PORT,&GPIO_InitStructure);
}

//�������µ�ƽΪ�͵�ƽ
u8 KEY_Scan(u8 mode)
{
	static u8 key=1;
	
	//���䰴�²�ɨ��
	if(mode==1)
		key=1;
	//�жϰ�������
	if(key==1&&(KEY1==0||KEY2==0||KEY3==0||KEY4==0))
	{
		delay_ms(10);//����
		key=0;//��ֹ�����ٴα������ֽ���if��⣬�ﵽ����ʶ��һ�ε�Ŀ��
		if(KEY1==0)
			return KEY1_PRESS;
		else if(KEY2==0)
			return KEY2_PRESS;
		else if(KEY3==0)
			return KEY3_PRESS;
		else if(KEY4==0)
			return KEY4_PRESS;
	}
	//�жϰ����ɿ�
	else if(KEY1==1&&KEY2==1&&KEY3==1&&KEY4==1)
	{
		key=1;//�ָ�key���ڿ����ٴΰ�����
	}
	return 0;
}
