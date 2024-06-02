#include "beep.h"
#include "SysTick.h"

void BEEP_Init(void)
{
	//����ṹ�����
	GPIO_InitTypeDef GPIO_InitStructure;
	//��LED�˿ڽ���ʱ��ʹ��
	RCC_APB2PeriphClockCmd(BEEP_PORT_RCC,ENABLE);
	//����GPIO��ģʽ��IO��
	//ģʽΪ���������  �͵�ƽ���
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
	GPIO_InitStructure.GPIO_Pin = BEEP_PIN;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	//��ʼ��GPIO
	GPIO_Init(BEEP_PORT,&GPIO_InitStructure);
	//���͵�ƽ����Ӧ�������̬ ����������õ�
	GPIO_ResetBits(BEEP_PORT,BEEP_PIN);
}
//��������������ʵ��
/*ʵ��˼·��ͨ�����ݲ�ͬ��usֵ�Ӷ��ı䲻ͬƵ������ߵ͵�ƽ*/
//times:�����������ߵ͵�ƽ����
//us:��ʱ΢��ʱ��
void BEEP_Alarm(u8 times,u8 us)
{
	while(times--)
	{
		BEEP=!BEEP;//��ת��ƽ
		delay_us(us);
	}
}
