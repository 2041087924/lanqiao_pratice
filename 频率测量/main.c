#include <STC15F2K60S2.h>

#define uchar unsigned char
#define uint unsigned int
	
uchar code number[]={0xc0,0xf9,0xa4,0xb0,0x99,0x92,0x82,0xf8,0x80,0x90,0xbf,0x8e,0xff};
uint count_f=0;  
uint dat_f=0;   //�˴�����������int ,��Ϊchar �����ã��˴�Ҳ�ǹؼ�
uchar count_t=0;

void display();
void display_bit(uchar pos , uchar dat);
void SMGdelayms(uchar ms);
void Timer01_Init();
void System_Init();

void main()
{
	System_Init();
	Timer01_Init();
	while(1)
	{
		display();
	}
}

void System_Init()
{
	P2 = (P2 & 0x1f) | 0x80;
	P0=0xff;
	P2 = (P2 & 0x1f) | 0xa0;
	P0=0x00;
	P2 = (P2 & 0x1f) | 0xc0;
	P0=0xff;
	P2 = (P2 & 0x1f) | 0xe0;
	P0=0xff;
}
void Timer01_Init()  //��ʱ��0������������ʱ��1������ʱ
{
	TMOD = 0X16;
	
	TH0 = 255;  //��ʱ��0��8λ�Զ���װģʽ
	TL0 = 255;
	
	TH1 = (65536-50000)/256;
	TL1 = (65536-50000)%256;
	
	TR0 = 1;  //��ʱ��0��ʼ����
	TR1 = 1;  //��ʱ��1��ʼ��ʱ
	ET0 = 1;  //��ʱ��0ʹ���жϴ�
	ET1 = 1;  //��ʱ��1ʹ���жϴ�
	EA = 1;   //ʹ�����жϴ�
}

void Timer0() interrupt 1
{
	count_f++;
}
void Timer1() interrupt 3
{
	TH1 = (65536-50000)/256; //����װ�ؼ�ʱ��ֵ
	TL1 = (65536-50000)%256;
	count_t++;
	if(count_t==20)
	{
		count_t=0;
		dat_f=count_f;
		count_f=0;
	}
}
void SMGdelayms(uchar ms)
{
	uint i,j;
	for(i=ms;i>0;i--)
		for(j=815;j>0;j--);
}
void display_bit(uchar pos , uchar dat)
{
	P0 = 0xff;  //���������
	P2 = (P2 & 0x1f) | 0xc0;
	P0=0x01 << pos;
	P2 = (P2 & 0x1f) | 0xe0;
	P0=number[dat];
}
void display()
{
	display_bit(0,11);
	SMGdelayms(1);
	display_bit(1,12);
	SMGdelayms(1);
	display_bit(2,12);
	SMGdelayms(1);
	if(dat_f > 9999)   //�����λΪ0����ʾ�������if����ж�
	{
		display_bit(3,dat_f / 10000);
		SMGdelayms(1);
	}
	if(dat_f > 999)
	{
		display_bit(4,(dat_f / 1000) %10);
		SMGdelayms(1);
	}
	if(dat_f > 99)
	{
		display_bit(5,(dat_f / 100) % 10);
		SMGdelayms(1);
	}
	if(dat_f > 9)
	{
		display_bit(6,(dat_f / 10) %10);
		SMGdelayms(1);
	}
	display_bit(7,dat_f % 10);
	SMGdelayms(1);
}