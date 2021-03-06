#include "2440addr.h"					//将"2440addr.h"包含进来,这里面放的是所有寄存器的地址宏

void UART0Init(void)					//初始化UART0
{
	rGPHCON = (2<<6) | (2<<4);			//设定GPH{2,3}为TXD[0]和RXD[0]
	rGPHUP  = (1<<3) | (1<<2);			//将GPH{2,3}的上拉电阻禁用
	
	rUBRDIV0= 658;					//波特率除数寄存器,UBRDIVn=(int)(UART_clock/(buad_rate*16))-1, 这里波特率	9600  PCLK=101.25M ;   101.25M/(9600*16) - 1 = 658
	
	rULCON0 = (0<<6) | (0<<3) | (0<<2) | (3<<0);	//发送接收每帧的数据位数为8bit,每帧一个停止位,没有奇偶校验，正常模式操作(非红外模式),将0号通道的线性控制寄存器设定为(0000 0011)
	rUCON0  = (0<<12) | (0<<10) | (0<<9) | (0<<8) | (0<<7) | (0<<6) | (0<<5) | (0<<4) | (1<<2) | (1<<0);	//(0000 0101) UART控制寄存器,设定为收发中断模式
	rUFCON0 = (0<<6) | (0<<4) | (0<<2) | (0<<1) | (0<<0);	//(0000)发送fifo触发为空,读取fifo触发为1字节,发送fifo重置后不自动清除,读取fifo重置后不自动清除,禁用fifo特性
	rUMCON0 = (0<<4);				//串行Modem控制寄存器,关闭AFC(Auto Flow Control),使用高电平去激活nRTS('H' level)
}

void RTC_Time_Set( void )				//设定时间函数
{
	rRTCCON = 1 ;					//RTCCON的RTCEN被置1后,就解锁了BCD寄存器的读写操作

	rBCDYEAR = 0x15 ;				//年份设为2015年
	rBCDMON  = 0x09 ;				//月份设为09月
	rBCDDATE = 0x28 ;				//日期设为28日	
	rBCDDAY  = 0x05 ;				//星期设为星期五
	rBCDHOUR = 0x14 ;				//时间设为14小时
	rBCDMIN  = 0x52 ;				//分钟设为52分
	rBCDSEC  = 0x27 ;				//秒钟设为27秒
	
	rRTCCON &= ~1 ;					//RTCCON的RTCEN被置0后,就锁住了BCD寄存器的读写操作
}


void Delay(unsigned long Cnt)				//延时程序
{
 	while(Cnt--);
}

int main()
{
	unsigned char year,month,day,hour,minute,second,oldsecond;	//定义年月日时分秒和旧秒的存放处

	UART0Init();					//初始化UART0
	RTC_Time_Set();					//设定时间

	rRTCCON = 1 ;					//RTCCON的RTCEN被置1后,就解锁了BCD寄存器的读写操作
	oldsecond = rBCDSEC  ;				//将原有时间保存
	rRTCCON &= ~1 ;					//RTCCON的RTCEN被置0后,就锁住了BCD寄存器的读写操作

	while(1)					//无限循环
	{
		rRTCCON = 1 ;				//RTCCON的RTCEN被置1后,就解锁了BCD寄存器的读写操作

		year   = rBCDYEAR  ;			//保存年
		month  = rBCDMON  ;			//保存月
		day    = rBCDDATE  ;			//保存日	
		hour   = rBCDHOUR  ;			//保存小时
		minute = rBCDMIN  ;			//保存分
		second = rBCDSEC  ;			//保存秒
		
		rRTCCON &= ~1 ;				//RTCCON的RTCEN被置0后,就锁住了BCD寄存器的读写操作
		if(second != oldsecond)			//这个条件几乎一直都为真
		{
			oldsecond =  second;		//将秒数保存
			WrUTXH0(0x32);	  		//写一个'2'
			Delay(0x50000);			//延时
			WrUTXH0(0x30);			//写一个'0'
			Delay(0x50000);
			WrUTXH0(0x30 + (year >> 4));	//写出年的十位
			Delay(0x50000);
			WrUTXH0(0x30 + (year & 0x0F));	//写出年的个位
			Delay(0x50000);
			WrUTXH0(0x30 + (month >> 4));	//写出月的十位	
			Delay(0x50000);
			WrUTXH0(0x30 + (month & 0x0F));	//写出月的个位
			Delay(0x50000);
			WrUTXH0(0x30 + (day >> 4));	//写出日的十位
			Delay(0x50000);
			WrUTXH0(0x30 + (day & 0x0F));	//写出日的个位
			Delay(0x50000);
			WrUTXH0(0x30 + (hour >> 4));	//写出时的十位
			Delay(0x50000);
			WrUTXH0(0x30 + (hour & 0x0F));	//写出时的个位
			Delay(0x50000);
			WrUTXH0(0x30 + (minute >> 4));	//写出分的十位
			Delay(0x50000);
			WrUTXH0(0x30 + (minute & 0x0F));//写出分的个位
			Delay(0x50000);
			WrUTXH0(0x30 + (second >> 4));	//写出秒的十位
			Delay(0x50000);
			WrUTXH0(0x30 + (second & 0x0F));//写出秒的个位
			Delay(0x50000);
			
			WrUTXH0(0x0A);			//写出'\n'
		}
	}
}
