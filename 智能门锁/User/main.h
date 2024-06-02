#include "system.h"
#include "oled.h"
#include "key.h"
#include "led.h"
#include "stmflash.h"
#include "SysTick.h"
#include "system.h"
#include "usart.h"
#include "rtc.h"
#include "oled.h"
#include "RFID.h"
#include <stdio.h>

#define SYS_SAVEADDR 0x0800f000
typedef struct 
{
	u32 HZCFlag;
	u8 passwd1[7];
	u8 passwd2[7];
	u8 cardid[10][6];
	u8 errCnt;//�������
	u8 errTime;//�ȴ�����ʱ��
}SysTemPat;

extern SysTemPat sys;