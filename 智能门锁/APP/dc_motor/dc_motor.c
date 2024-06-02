#include "dc_motor.h"

void DC_Motor_Init(void)
{
	//����ṹ�����
	GPIO_InitTypeDef GPIO_InitStructure;
	
	//�Զ˿ڽ���ʱ��ʹ��
	RCC_APB2PeriphClockCmd(DC_MOTOR_PORT_RCC,ENABLE);

	GPIO_InitStructure.GPIO_Pin = DC_MOTOR_PIN;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	//��ʼ��GPIO
	GPIO_Init(DC_MOTOR_PORT,&GPIO_InitStructure);
	//���ùܽŽ�������ʹ��led����Ϩ���״̬
	GPIO_ResetBits(DC_MOTOR_PORT,DC_MOTOR_PIN);
}
