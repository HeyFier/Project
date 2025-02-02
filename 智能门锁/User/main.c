#include "main.h"
#include <string.h>
#define MAXERRTIMES 5

SysTemPat sys;

void Display_Data(void);//显示时间
void ShowErrMessage(u8 ensure);//显示确认码错误信息
int password(void);//密码锁
void SetPassworld(void);//修改密码
void starting(void);//开机界面信息

u8 MFRC522_lock(void);//刷卡解锁
u8 Add_Rfid(void);		//录入
u8 MFRC522_lock(void);
u16 GET_NUM(void);

void Set_Time(void);
void Massige(void);
void DisLock(void);
void DisUnLock(void);
u8 DisErrCnt(void);
void SysPartInit(void );   //系统参数初始化 


int Error;  //密码验证信息
u8 DisFlag = 1;
uc8 numberascii[]={'0','1','2','3','4','5','6','7','8','9','A','B','C','D','E','F'};
//MFRC522数据区
u8  mfrc552pidbuf[18];
u8  card_pydebuf[2];
u8  card_numberbuf[5];
u8  card_key0Abuf[6]={0xff,0xff,0xff,0xff,0xff,0xff};
u8  card_writebuf[16]={0,1,2,3,4,5,6,7,8,9,10,11,12,13,14,15};
u8  card_readbuf[18];

u8 * setup[7]={"1、录入卡片","2、查看信息","3、修改密码","4、修改时间","           ","           "};
u8 * week[7]={"Sun","Mon","Tue","Wed","Thu","Fri","Sat"};
int main()
{
	u16 set=0;
	 u8 err=0;
	int key_num;
	int time1;
	int time2;		//锁屏时间
	char arrow=0;  //箭头位子
	
	
	SysTick_Init(72);
	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2);  //中断优先级分组分2组
	uart_init(9600);
	LED_Init();
	OLED_Init();
	KEY_Matrix_Init();
	OLED_Clear();
	
	starting();
	err = RTC_Init();	  			//RTC初始化
	if(err)
	{
		OLED_Clear(); 
		Show_Str(12,13,128,20,"RTC CRY ERR!",12,0); 
		OLED_Refresh_Gram();//更新显示
		delay_ms(3000);
	}
	SysPartInit();   //系统参数初始化
	
	while(1)
	{
		
//锁屏界面
MAIN:
			OLED_Clear(); 
			OLED_Show_Font(56,48,0);//显示锁图标
		while(1)
			{
				time1++;  Display_Data();//时间显示：每1000ms更新一次显示数据
				
				if(DisFlag == 1)
				{
					DisFlag = 0;
					OLED_Fill(0,24,16,63,0);
					OLED_Refresh_Gram();//更新显示
				}	 
				//密码锁
				key_num=KEY_Matrix_Scan();	//按键扫描
				if(key_num!=-1)
				{
					Error=password();//密码解锁函数
					if(Error==0)
					{
						goto MENU;	//跳到解锁界面
					}
					else 
					{
						OLED_Show_Font(56,48,0);//锁
					}
				}
					delay_ms(1);
			
		}
			/********************主界面**************************/
MENU:
			OLED_Clear();
MENUNOCLR:
			OLED_Fill(0,0,20,48,0);
			//主页菜单显示
			if(arrow<3)
				{
				  Show_Str(5,arrow*16,128,16,"->",16,0);//显示箭头
				  set=0;
				}
			else 
				{
				  Show_Str(5,(arrow-3)*16,128,16,"->",16,0);
				  set=3;
				}
			Show_Str(25,0,128,16,setup[set],16,0);
			Show_Str(25,16,128,16,setup[set+1],16,0);
			Show_Str(25,32,128,16,setup[set+2],16,0);
			Show_Str(0,52,128,12,"上 返回 下     确定",12,0);
			OLED_Refresh_Gram();//更新显示
							time2=0;
			while(1)
			{
						//超时锁屏
						/*time2++;
						if(time2>100 | key_num==4){  
							
							  OLED_Clear();
								DisLock();
								//if(time2>100) beep_on_mode2();
								time2 =0;

								OLED_Clear();
								goto MAIN;
						}*/
						//手机蓝牙锁定
						if(memcmp(USART_RX_BUF,"lock",4)==0)	{

							DisLock();
							goto MAIN;
						}
						
						//功能选项选择
						key_num=KEY_Matrix_Scan();
						if(key_num)
						{
							if(key_num==13){
								if(arrow>0)arrow--;
								goto MENUNOCLR;
							}
							if(key_num==15){
								if(arrow<3)arrow++;
								goto MENUNOCLR;
							}
							if(key_num==14){
								goto MAIN;
							}
							if(key_num==16){
								switch(arrow)
								{
									case 0:Add_Rfid();		break;//录卡
									case 1:Massige();		break;//删卡
									case 2:SetPassworld();break;//修改密码
								  case 3:Set_Time(); break;  //设置时间

							
								}
								goto MENU;
							}		
						}delay_ms(1);
			}
		}			
}
/*********************************开机初始化及显示***********************************/

void SysPartInit(void )   //系统参数初始化 
{
		STMFLASH_Read(SYS_SAVEADDR,(u16*)&sys,sizeof(sys)); //读取
		if(sys.HZCFlag != 112233)
		{
			memset(&sys,0,sizeof(sys));
			sys.HZCFlag = 112233;
			strcpy((char *)sys.passwd1, "123456");//密码
			strcpy((char *)sys.passwd2, "112233");//密码
			STMFLASH_Write(SYS_SAVEADDR,(u16*)&sys,sizeof(sys));//保存到内部FLASH
			printf("初始化配置成功\r\n");
		}else
		{
			printf("欢迎使用智能密码锁\r\n");
		}
}

/*********************************开机初始化***********************************/

 //开机信息
void starting(void)
{
	u8 cnt = 0;
	u8 ensure;
	char str[64];			  
	u8 key;

	Show_Str(16,12,128,16,"智能密码锁系统",16,0);
	delay_ms(2000);
	OLED_Refresh_Gram();//更新显示
}

/*********************************按键解锁***********************************/

//密码解锁保密输入+密码验证
int password(void)
{
	int  key_num=0,i=0,satus=0;
	u16 num=0,num2=0,time3=0,time;
	u8 pwd[11]="          ";
	u8 hidepwd[11]="          ";
	u8 buf[64];
	OLED_Clear();//清屏

	if(DisErrCnt())  return -1;//错误次数超限
	
	OLED_Clear();//清屏
	Show_Str(5,0,128,16,"密码：",16,0);
	Show_Str(10,16,128,12," 1   2   3  Bck",12,0);
	Show_Str(10,28,128,12," 4   5   6  Del",12,0);
	Show_Str(10,40,128,12," 7   8   9  Dis",12,0);
	Show_Str(10,52,128,12,"Clr  0  Clr  OK",12,0);
	OLED_Refresh_Gram();//更新显示

	while(1)
	{
		//MFRC522解锁
			MFRC522_Initializtion();			
			Error=MFRC522_lock();
		if(Error==0)
			{
			sys.errCnt = 0;
			return 0;
	  	}
		
			//手机蓝牙解锁密码1
						Error=usart1_cherk((char*)sys.passwd1);         
						if(Error==0){ 
							OLED_Clear_NOupdate();
							Show_Str(12,13,128,20,"蓝牙密码1：正确",12,0); 
							OLED_Refresh_Gram();//更新显示
							delay_ms(800);
							DisUnLock();
							return 0;	
						}
						else {}
						
						//手机蓝牙解锁密码2
						Error=usart1_cherk((char*)sys.passwd2);         
						if(Error==0){
							sys.errCnt = 0;
							OLED_Clear_NOupdate();
							Show_Str(12,13,128,12,"蓝牙密码2：正确",12,0); 
							OLED_Refresh_Gram();//更新显示
							delay_ms(800);
							DisUnLock();
							return 0;
						}
						else {}
			
		key_num = KEY_Matrix_Scan();	
		if(key_num != -1)
		{		
			DisFlag = 1;
			time3=0;
			if(key_num != -1)
			{	
				DisFlag = 1;
				time3=0;
				switch(key_num)
				{
					case 1:
					case 2:
					case 3:
						pwd[i]=key_num+0x30; //1-3
						hidepwd[i]='*';
						i++;
					break;
					case 4://返回
						OLED_Clear();
						delay_ms(500);
					return -1;
						
					break;
					case 5:
					case 6:
					case 7:
						pwd[i]=key_num+0x30-1; //4-6
						hidepwd[i]='*';
						i++;
					break;
					case 8:
						if( i > 0){
							pwd[--i]=' ';  //‘del’键
							hidepwd[i]=' '; 
						}
					break;
					case 9:
					case 10:
					case 11:
						pwd[i]=key_num+0x30-2; //4-6
						hidepwd[i]='*';
						i++;
					break;
					case 12:satus=!satus; break;//DIS
					case 13:
					case 15:
						while(i--){
						pwd[i]=' ';  //‘清空’键
						hidepwd[i]=' '; 
						}
						i=0;
					break;
					case 14:
						pwd[i]=0x30; //4-6
						hidepwd[i]='*';
						i++;
					break;
					case 16:
						goto UNLOCK;
					break;
				}
		}
		if(DisFlag == 1)
		{
		if(satus==0)OLED_ShowString(53,0,hidepwd,12);
		else OLED_ShowString(53,0,pwd,12);
		OLED_Refresh_Gram();//更新显示
		}
		
		time3++;
		if(time3%10==0){
			OLED_Clear();//清屏
			return -1;
		}
	}
}
//密码验证
UNLOCK:	
		for(i=0; i<10; i++){   //验证虚伪密码
			if(pwd[i]==sys.passwd1[num])num++;
				else num=0;
			if(num==6)
				break;
		}
		for(i=0; i<10; i++){   //验证密码
			if(pwd[i]==sys.passwd2[num2])num2++;
				else num2=0;
			if(num2==6)
				break;
		}
		if(num==6 | num2==6){
			DisUnLock();
			OLED_Clear();//清屏
			sys.errCnt = 0;
			return 0;
		}
		else {
			sys.errCnt++;//错误次数计数
			if(sys.errCnt>MAXERRTIMES)
				sys.errTime = 30; //30秒不能再解锁
			OLED_Clear();//清屏
			Show_Str(45,48,128,16,"密码错误！",16,0);
			OLED_Refresh_Gram();//更新显示
			//beep_on_mode1();
			LED2=0;
			LED1=1;                     
			delay_ms(1500);
			OLED_Clear();//清屏
			return -1;
		}
}
//密码错误次数超过5次
u8 DisErrCnt(void)
{
	 int time=0;
	 u8 buf[64];
  if(sys.errTime>0)//错误次数计数
	{
		
		OLED_Clear();

		while(1)
		{
			if(time++ == 10)
			{
				time = 0;

				if(sys.errTime==0)
				{
					OLED_Clear();
					break;
				}
				Show_Str(0,16,128,16,"密码错误次数过多",16,0);
				sprintf(buf,"请%02d秒后重试", sys.errTime);
				Show_Str(20,32,128,16,buf,16,0);
				OLED_Refresh_Gram();//更新显示
			}
			delay_ms(1);
			if(4 == KEY_Matrix_Scan())//返回
			{
				OLED_Clear();
				return 1;
			}
		}
	}
}
//修改密码
void SetPassworld(void)
{
	int pwd_ch=0;
	int  key_num=0,i=0,satus=0;
	u16 time4=0;
	u8 pwd[6]="      ";
	u8 hidepwd[6]="      ";
	u8 buf[10];
	OLED_Clear();//清屏
	Show_Str(10,16,128,12," 1   2   3  Bck",12,0);
	Show_Str(10,28,128,12," 4   5   6  Del",12,0);
	Show_Str(10,40,128,12," 7   8   9  Dis",12,0);
	Show_Str(10,52,128,12,"Clr  0  Chg  OK",12,0);
	
	
	Show_Str(5,0,128,16,"新密码",16,0);
	sprintf((char*)buf,"%d:",pwd_ch+1);
	Show_Str(5,48,128,16,buf,16,0);
	OLED_Refresh_Gram();//更新显示
	while(1)
	{
		key_num=KEY_Matrix_Scan();	
		if(key_num != -1)
		{	
			DisFlag = 1;
			time4=0;
			switch(key_num)
			{
				case 1:
				case 2:
				case 3:
					pwd[i]=key_num+0x30; //1-3
					hidepwd[i]='*';
					i++;
				break;
				case 4://返回
					OLED_Clear();
					delay_ms(500);
				return ;
					
				break;
				case 5:
				case 6:
				case 7:
					pwd[i]=key_num+0x30-1; //4-6
					hidepwd[i]='*';
					i++;
				break;
				case 8:
					if( i > 0){
						pwd[--i]=' ';  //‘del’键
						hidepwd[i]=' '; 
					}
				break;
				case 9:
				case 10:
				case 11:
					pwd[i]=key_num+0x30-2; //4-6
					hidepwd[i]='*';
					i++;
				break;
				case 12:satus=!satus; break;//DIS
				case 13:
					sprintf((char*)buf,"%d:",pwd_ch+1);
					Show_Str(5,48,128,16,buf,16,0);
					pwd_ch = !pwd_ch;
				case 15:
					while(i--){
					pwd[i]=' ';  //‘清空’键
					hidepwd[i]=' '; 
					}
					i=0;
				break;
				case 14:
					pwd[i]=0x30; //4-6
					hidepwd[i]='*';
					i++;
				break;
				case 16:
					goto MODIF;
				break;
			}
		}
		if(DisFlag == 1)
		if(satus==0)
		{
			OLED_ShowString(70,0,hidepwd,12);
			OLED_Refresh_Gram();//更新显示
		}
		else 
		{
			OLED_ShowString(70,0,pwd,12);
			OLED_Refresh_Gram();//更新显示
		}
		
		time4++;
		if(time4%50==0){
			OLED_Clear();//清屏
			DisFlag = 1;
			return ;
		}
	}	
	
	
MODIF:
	if(pwd_ch==0)
	{
		memcpy(sys.passwd1,pwd,7);
		STMFLASH_Write(SYS_SAVEADDR,(u16*)&sys,sizeof(sys));//保存到内部FLASH
		
	
		printf("pwd=%s",sys.passwd1);
	}
	else
	{		
		memcpy(sys.passwd2,pwd,7);
		STMFLASH_Write(SYS_SAVEADDR,(u16*)&sys,sizeof(sys));//保存密码到内部FLASH

		printf("pwd2=%s",sys.passwd1);
	}
	OLED_Clear();//清屏
	Show_Str(0,48,128,16,"密码修改成功 ！",16,0);
	OLED_Refresh_Gram();//更新显示
	delay_ms(1000);
}



//解锁过程中界面显示
void DisUnLock(void)
{
	OLED_Clear();
	Show_Str(20,10,128,24,"解锁中...",24,0);	
	OLED_Refresh_Gram();//更新显示
//	GPIO_SetBits(GPIOB,GPIO_Pin_9);
		LED1=0;
		LED2=1;
	Show_Str(20,10,128,24,"已解锁！",24,0);
	OLED_Refresh_Gram();//更新显示
	delay_ms(500);
}

void DisLock(void)
{
	OLED_Clear();
	Show_Str(30,20,128,16,"锁定中！",16,0);
	OLED_Refresh_Gram();//更新显示
//GPIO_ResetBits(GPIOB,GPIO_Pin_9);//BEEP OFF

	Show_Str(30,20,128,16,"已锁定！",16,0);
	OLED_Show_Font(56,48,0);//锁
	OLED_Refresh_Gram();//更新显示
	delay_ms(500);
}
/*********************************按键解锁***********************************/

/*********************************RFID解锁***********************************/
//录入新卡
u8 Add_Rfid(void)
{
	u8 ID;
	u16 time6=0;
	u8 i,key_num,status=1,card_size;
	OLED_Clear();
	Show_Str(0,0,128,16,"=== 录入卡片 ===",16,0);	
	Show_Str(0,20,128,12,"请放入新卡片：",12,0);	
	Show_Str(0,52,128,12,"返回",12,0);
			OLED_Refresh_Gram();//更新显示
	MFRC522_Initializtion();			//初始化MFRC522
	while(1)
	{
		AntennaOn();
		status=MFRC522_Request(0x52, card_pydebuf);			//寻卡
		if(status==0)		//如果读到卡
		{
			printf("rc522 ok\r\n");
			Show_Str(0,38,128,12,"读卡成功！",12,0);
			OLED_Refresh_Gram();//更新显示
			status=MFRC522_Anticoll(card_numberbuf);			//防撞处理			
			card_size=MFRC522_SelectTag(card_numberbuf);	//选卡
			status=MFRC522_Auth(0x60, 4, card_key0Abuf, card_numberbuf);	//验卡
			status=MFRC522_Write(4, card_writebuf);				//写卡（写卡要小心，特别是各区的块3）
			status=MFRC522_Read(4, card_readbuf);					//读卡

			printf("卡的序列号：");
			for(i=0;i<5;i++)
			{
				printf("%#x ",card_numberbuf[i]);
			}
			printf("\r\n");

			AntennaOff();
			
			OLED_Clear_NOupdate();
			Show_Str(0,12,128,12,"请输入储存ID(0-9):  ",12,0);
			Show_Str(122,52,128,12," ",12,0);
			Show_Str(0,52,128,12,"删除 清空      确认",12,0);
			OLED_Refresh_Gram();//更新显示
				do
					ID=GET_NUM();
				while(!(ID<10));//输入ID必须小于最大容量
			printf("正在录入卡片：%d\r\n",ID);
			OLED_Clear_NOupdate();
			Show_Str(0,38,128,12,"正在录入.",12,0);
			OLED_Refresh_Gram();//更新显示

			for(i=0;i<5;i++)
			{
				sys.cardid[ID][i] = card_numberbuf[i];
			}
				
			STMFLASH_Write(SYS_SAVEADDR,(u16*)&sys,sizeof(sys));//保存到内部FLASH

			for(i=0;i<10;i++)
			printf("cardid={%X,%X,%X,%X}\r\n",sys.cardid[i][0],sys.cardid[i][1],sys.cardid[i][2],sys.cardid[i][3]);
			Show_Str(0,38,128,12,"录入成功！",12,0);
			OLED_Refresh_Gram();//更新显示
			delay_ms(1000);
			OLED_Clear();
			return 0;
		}
		key_num=KEY_Matrix_Scan();
		if(key_num==13)
		{
			break;
		}		
	/*	time6++;
		if(time6%50==0 | key_num==13)
		{
			OLED_Clear();
			return 1;
		}*/
	}
}
//rfid卡锁
u8 MFRC522_lock(void)
{
	u8 i,j,status=1,card_size;
	u8 count;
	u8 prtfbuf[64];
	
	AntennaOn();
  status=MFRC522_Request(0x52, card_pydebuf);			//寻卡
	if(status==0)		//如果读到卡
	{
		if(DisErrCnt())return -1;//错误次数超限
		printf("rc522 ok\r\n");
		status=MFRC522_Anticoll(card_numberbuf);			//防撞处理			
		card_size=MFRC522_SelectTag(card_numberbuf);	//选卡
		status=MFRC522_Auth(0x60, 4, card_key0Abuf, card_numberbuf);	//验卡
		status=MFRC522_Write(4, card_writebuf);				//写卡（写卡要小心，特别是各区的块3）
		status=MFRC522_Read(4, card_readbuf);					//读卡

		
		count=0;
		
		for(j=0;j<10;j++){
		printf("\r\n卡%d 的序列号：",j);
			for(i=0;i<5;i++)
			{
				printf("%x=%x    ",card_numberbuf[i],sys.cardid[j][i]);
				if(card_numberbuf[i]==sys.cardid[j][i])count++;
			}
		printf("\r\n");
			if(count>=4)
			{
				sys.errCnt = 0;
				OLED_Clear_NOupdate();
				sprintf(prtfbuf,"RFID:%d匹配成功",j);
				Show_Str(12,13,128,20,prtfbuf,12,0); 
				OLED_Refresh_Gram();//更新显示
				delay_ms(500);
				DisUnLock();
				return 0;
			}else count=0;
		}
		{
			sys.errCnt++;//错误次数计数
			
			if(sys.errCnt>MAXERRTIMES)
				sys.errTime = 30; //30秒不能再解锁
			OLED_Clear(); 
			Show_Str(12,13,128,20,"卡片错误",12,0); 
			OLED_Refresh_Gram();//更新显示
			LED0=0;
			OLED_Clear(); 
			OLED_Show_Font(56,48,0);//锁
			DisFlag = 1;
		}
		
		printf("\n");


	}	
	
	AntennaOff();
	return 1;
}

//获取键盘数值
u16 GET_NUM(void)
{
	u8  key_num=0;
	u16 num=0;
		OLED_ShowNum(78,32,num,3,12);
	OLED_Refresh_Gram();//更新显示
	while(1)
	{
		key_num=KEY_Matrix_Scan();
		if(key_num != -1)
		{

			
			switch(key_num)
				{
					case 1:
					case 2:
					case 3:
						if(key_num>0&&key_num<10&&num<99)//‘1-9’键(限制输入3位数)
						num =num*10+key_num;		
					break;
					case 4://返回
						return 0xFFFF;
					return -1;
						
					break;
					case 5:
					case 6:
					case 7:
						if(key_num>0&&key_num<10&&num<99)//‘1-9’键(限制输入3位数)
						num =num*10+key_num-1;
					break;
					case 8:num =num/10;//‘del’键
					break;
					case 9:
					case 10:
					case 11:
						if(key_num>0&&key_num<10&&num<99)//‘1-9’键(限制输入3位数)
						num =num*10+key_num-2;
					break;
					case 12: break;//DIS
					case 13:
					case 15:
						return 0xFF00;
					break;
					case 14:num =num*10;
					break;
					case 16:return num;
					break;
				}
		OLED_ShowNum(78,32,num,3,12);
	OLED_Refresh_Gram();//更新显示
		}
	}	
}
//显示信息
void Massige(void)
{
	OLED_Clear();
	Show_Str(0,0,128,12,"智能密码锁系统",12,0); 

	Show_Str(0,52,128,12,"2023-10-16 ",12,0); 
	
	OLED_Refresh_Gram();//更新显示
	delay_ms(3000);
}

/*********************************RFID解锁***********************************/

/*********************************时间显示***********************************/
 //显示时间
void Display_Data(void)
{
	static u8 t=1;	
	u8 tbuf[40];
	if(t!=calendar.sec)
	{
		t=calendar.sec;
		//sprintf((char*)tbuf,"%02d:%02d:%02d",calendar.hour,calendar.min,calendar.sec); 
		OLED_ShowString(0,0,tbuf,24);	

		//sprintf((char*)tbuf,"%04d-%02d-%02d",calendar.w_year,calendar.w_month,calendar.w_date); 
		OLED_ShowString(68,26,tbuf,12);		

		//sprintf((char*)tbuf,"%s",week[calendar.week]); 
	
		OLED_ShowString(110,12,tbuf,12);
		DisFlag = 1;//更新显示
	}
}

//设置时间
void Set_Time(void)
{

	u16 year;
	u8 mon,dat,wek,hour,min,sec;
	u16 time5=0;
	u8 tbuf[40];
	int key_num;
	int st=0;

	year=calendar.w_year;
	mon=calendar.w_month;
	dat=calendar.w_date;
	wek=calendar.week;
	hour=calendar.hour;
	min=calendar.min;
	sec=calendar.sec;
	OLED_Clear();
	Show_Str(98,38,128,12,"<--",12,0);
	Show_Str(0,52,128,12,"减  加   切换  确定",12,0);
	
	OLED_Refresh_Gram();//更新显示
	while(1)
	{
		time5++;
		key_num=KEY_Matrix_Scan();
			if(key_num==12 | time5==30){
				OLED_Clear();//清屏
				return ;
			}
			if(key_num==13){
				switch(st)
				{
					case 0:if(hour>0)hour--;break;
					case 1:if(min>0)min--;break;
					case 2:if(sec>0)sec--;break;
					case 3:if(wek>0)wek--;break;
					case 4:if(year>0)year--;break;
					case 5:if(mon>0)mon--;break;
					case 6:if(dat>0)dat--;break;
				}
			}
			if(key_num==14){
				switch(st)
				{
					case 0:if(hour<23)hour++;break;
					case 1:if(min<59)min++;break;
					case 2:if(sec<59)sec++;break;
					case 3:if(wek<7)wek++;break;
					case 4:if(year<2099)year++;break;
					case 5:if(mon<12)mon++;break;
					case 6:if(dat<31)dat++;break;
				}
			}
			if(key_num==15){
				if(st<7)st++;
				if(st==7)st=0;
			}
			if(key_num==16){
				break;
			}
		if(time5%5==0)
		{
			switch(st)			//闪烁
				{
					case 0:OLED_ShowString(0,0,"  ",24);break;
					case 1:OLED_ShowString(36,0,"  ",24);break;
					case 2:OLED_ShowString(72,0,"  ",24);break;
					case 3:OLED_ShowString(110,12,"   ",12);break;
					case 4:OLED_ShowString(68,26,"    ",12);break;
					case 5:OLED_ShowString(98,26,"  ",12);break;
					case 6:OLED_ShowString(116,26,"  ",12);break;
				}
			OLED_Refresh_Gram();//更新显示
		}
		if(time5%5==0)
		{
			time5=0;
			sprintf((char*)tbuf,"%02d:%02d:%02d",hour,min,sec); 
			OLED_ShowString(0,0,tbuf,24);	
		
			sprintf((char*)tbuf,"%04d-%02d-%02d",year,mon,dat); 
			OLED_ShowString(68,26,tbuf,12);		
			sprintf((char*)tbuf,"%s",week[wek]); 
			OLED_ShowString(110,12,tbuf,12);	
			OLED_Refresh_Gram();//更新显示
		}delay_ms(1);
	}

	RTC_Set(year,mon,dat,hour, min,sec);
	OLED_Clear();
	Show_Str(20,48,128,16,"设置成功！",16,0);
	OLED_Refresh_Gram();//更新显示
	delay_ms(1000);
}

/*********************************时间显示***********************************/
