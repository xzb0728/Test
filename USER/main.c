#include "delay.h"
#include "sys.h"
#include "usart.h"
#include "stmflash.h"
#include "iap.h"
#include "led.h"
#include "IO.h"

//#define Debug

//������Ҫ�޸�Ϊ��λ���ֿ鷢�����ݣ���Ƭ��ÿ����50Kд��һ�����ݣ�ֱ��д��Ϊֹ
int main(void)
{		
	u8 Recv_Flag=1; 
	u16 oldcount=0;				//�ϵĴ��ڽ�������ֵ
	u16 applenth=0;				//���յ���app���볤��
	LED_Init(); 
  NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2); 
	uart_init(115200);	//���ڳ�ʼ��Ϊ115200
	delay_init();	   	 	//��ʱ��ʼ�� 
	IO1_Init_Input(); 
#ifdef Debug
	printf("*******   RTU BootLoader V1.0   *******\r\n");
#endif
	delay_ms(10);
	if(!IO1_ReadInput())  ;
	else if(STMFLASH_ReadHalfWord(Addr_RebootFlag) == Program_Loaded_Flag)//������װ�صı�־
	{
		#ifdef Debug
			printf("Booting User Program\r\n");
		#endif
			delay_ms(100);
			iap_load_app(FLASH_APP1_ADDR);//ִ��FLASH APP����
	}
	else if(STMFLASH_ReadHalfWord(Addr_RebootFlag) == ReBoot_Flag)
	{
			FLASH_WriteByte(Addr_RebootFlag,Program_Loaded_Flag);//д��־λ
	}
	for(;;)
	{
	 	if(USART_RX_CNT)
		{
				if(Recv_Flag)
				{
					#ifdef Debug
					printf("�̼�������\r\n");
					#endif
					Recv_Flag = 0;
				}
				if(oldcount==USART_RX_CNT)//��������,û���յ��κ�����,��Ϊ�������ݽ������.
				{
						applenth=USART_RX_CNT;
						oldcount=0;
						USART_RX_CNT=0;
						#ifdef Debug
						printf("�û�����������!\r\n");
						printf("���볤��:%dBytes\r\n",applenth);
						#endif
					if(applenth)
					{
							#ifdef Debug
							printf("��ʼ���¹̼�...\r\n");	
							#endif
							if(((*(vu32*)(0X20001000+4))&0xFF000000)==0x08000000)//�ж��Ƿ�Ϊ0X08XXXXXX.
							{	 
									iap_write_appbin(FLASH_APP1_ADDR,USART_RX_BUF,applenth);//����FLASH����   
									#ifdef Debug
									printf("�̼��������!\r\n");	
									#endif								
									if(((*(vu32*)(FLASH_APP1_ADDR+4))&0xFF000000)==0x08000000)//�ж��Ƿ�Ϊ0X08XXXXXX.
									{								 
											LED_ProgramOK();
											FLASH_WriteByte(Addr_RebootFlag,Program_Loaded_Flag);//д��־λ
											delay_ms(3000);	
											#ifdef Debug
											delay_ms(2000);
											printf("ִ��Flash������!\r\n");
											#endif
											delay_ms(100);
											iap_load_app(FLASH_APP1_ADDR);//ִ��FLASH APP����
									}
									else 
									{
											#ifdef Debug
											printf("����ִ���쳣��\r\n");
											#endif
									}							
							 }	
						}
						else 
						{  
								#ifdef Debug
								printf("����ִ���쳣��\r\n");
								#endif
						}
					}
			}
		oldcount=USART_RX_CNT;	
		delay_ms(100);
	}   	   
}













