#include "delay.h"
#include "sys.h"
#include "usart.h"
#include "stmflash.h"
#include "iap.h"
#include "led.h"
#include "IO.h"

//#define Debug

//程序需要修改为上位机分块发送数据，单片机每接收50K写入一次数据，直到写完为止
int main(void)
{		
	u8 Recv_Flag=1; 
	u16 oldcount=0;				//老的串口接收数据值
	u16 applenth=0;				//接收到的app代码长度
	LED_Init(); 
  NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2); 
	uart_init(115200);	//串口初始化为115200
	delay_init();	   	 	//延时初始化 
	IO1_Init_Input(); 
#ifdef Debug
	printf("*******   RTU BootLoader V1.0   *******\r\n");
#endif
	delay_ms(10);
	if(!IO1_ReadInput())  ;
	else if(STMFLASH_ReadHalfWord(Addr_RebootFlag) == Program_Loaded_Flag)//程序已装载的标志
	{
		#ifdef Debug
			printf("Booting User Program\r\n");
		#endif
			delay_ms(100);
			iap_load_app(FLASH_APP1_ADDR);//执行FLASH APP代码
	}
	else if(STMFLASH_ReadHalfWord(Addr_RebootFlag) == ReBoot_Flag)
	{
			FLASH_WriteByte(Addr_RebootFlag,Program_Loaded_Flag);//写标志位
	}
	for(;;)
	{
	 	if(USART_RX_CNT)
		{
				if(Recv_Flag)
				{
					#ifdef Debug
					printf("固件接收中\r\n");
					#endif
					Recv_Flag = 0;
				}
				if(oldcount==USART_RX_CNT)//新周期内,没有收到任何数据,认为本次数据接收完成.
				{
						applenth=USART_RX_CNT;
						oldcount=0;
						USART_RX_CNT=0;
						#ifdef Debug
						printf("用户程序接收完成!\r\n");
						printf("代码长度:%dBytes\r\n",applenth);
						#endif
					if(applenth)
					{
							#ifdef Debug
							printf("开始更新固件...\r\n");	
							#endif
							if(((*(vu32*)(0X20001000+4))&0xFF000000)==0x08000000)//判断是否为0X08XXXXXX.
							{	 
									iap_write_appbin(FLASH_APP1_ADDR,USART_RX_BUF,applenth);//更新FLASH代码   
									#ifdef Debug
									printf("固件更新完成!\r\n");	
									#endif								
									if(((*(vu32*)(FLASH_APP1_ADDR+4))&0xFF000000)==0x08000000)//判断是否为0X08XXXXXX.
									{								 
											LED_ProgramOK();
											FLASH_WriteByte(Addr_RebootFlag,Program_Loaded_Flag);//写标志位
											delay_ms(3000);	
											#ifdef Debug
											delay_ms(2000);
											printf("执行Flash程序中!\r\n");
											#endif
											delay_ms(100);
											iap_load_app(FLASH_APP1_ADDR);//执行FLASH APP代码
									}
									else 
									{
											#ifdef Debug
											printf("程序执行异常！\r\n");
											#endif
									}							
							 }	
						}
						else 
						{  
								#ifdef Debug
								printf("程序执行异常！\r\n");
								#endif
						}
					}
			}
		oldcount=USART_RX_CNT;	
		delay_ms(100);
	}   	   
}













