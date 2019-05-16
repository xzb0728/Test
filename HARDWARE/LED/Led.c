#include "led.h" 
#include "Tim2.h"
//LED ���в���
struct LED
{
	unsigned short CountRun_Max;
	unsigned short CountCom_Max;
	unsigned short CounterRun;
	unsigned short CounterCom;
}LED_Para;

//LED IO��ʼ��
void LED_Init(void)
{
	
	GPIO_InitTypeDef  GPIO_InitStructure;
	/* Enable the GPIO_LED Clock */
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA, ENABLE); 		
	
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_11 | GPIO_Pin_12;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(GPIOA, &GPIO_InitStructure);
	/*��ʼ����󣬹ر�2��LED*/ 
	LED1 = OFF;
	LED2 = OFF;
	//��ʼ����ʱ��2
	Tim2_Init();

}
//���ڹ���
void LED_Running()
{
		TIM_Cmd(TIM2,DISABLE);
		LED1 = ON;
		
}
//���ڷ���
void LED_COMing()
{
		TIM_Cmd(TIM2,DISABLE);
		LED2 = ON;
		
}
//���������������
void LED_Sleep()
{
		TIM_Cmd(TIM2,DISABLE);	
		LED1 = OFF;
			
}
//���Ž���
void LED_COMFinish()
{
		TIM_Cmd(TIM2,DISABLE);
		LED2 = OFF;
		
}
//�������ڵ���״̬
void LED_Debug()//2s����
{
		LED_Para.CountRun_Max = LED_SLOW;
		LED_Para.CounterCom = 0;
		TIM_Cmd(TIM2,ENABLE);
}
//һ���ʾ�������سɹ�
void LED_DownloadOK()//500ms����
{
		LED_Para.CountRun_Max = LED_MID;
		LED_Para.CounterRun = 0;
		LED_Para.CountCom_Max = LED_MID;
		LED_Para.CounterCom = 0;
		TIM_Cmd(TIM2,ENABLE);
}
//TEST���³���5����߳�����³ɹ�
void LED_ProgramOK()//50ms����
{
		LED_Para.CountRun_Max = LED_FAST;
		LED_Para.CounterRun = 0;
		TIM_Cmd(TIM2,ENABLE);
}
//����ʧ��
void LED_COMFailed()//50ms����
{
		LED_Para.CountCom_Max = LED_FAST;
		LED_Para.CounterCom = 0;
		TIM_Cmd(TIM2,ENABLE);
}

//��ʱ��2�жϺ���
//ÿ��	time��50ms���ж�һ��
void TIM2_IRQHandler(void)
{
	if(TIM_GetITStatus(TIM2,TIM_IT_Update) != RESET) 
	{
		if(LED_Para.CountRun_Max)//Run�������ֵ��Ϊ0
		{
			if(LED_Para.CounterRun >= LED_Para.CountRun_Max)
			{
					LED_Para.CounterRun = 0;
					LED1 = ~LED1;
			}
			LED_Para.CounterRun++;
		}
		if(LED_Para.CountCom_Max)//Com�������ֵ��Ϊ0
		{	
			if(LED_Para.CounterCom >= LED_Para.CountCom_Max)
			{
					LED_Para.CounterCom = 0;
					LED2 = ~LED2;
			}
			LED_Para.CounterCom++;
		}
	}
	TIM_ClearITPendingBit(TIM2,TIM_FLAG_Update);
}




