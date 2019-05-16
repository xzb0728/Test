#include "Tim2.h"
#include "stm32f10x.h"

//��ʱ��2����
//��ʱ�����ʱ����㷽��:Tout=((4999+1)*(8399+1))/Ft us.
//Ft=��ʱ������Ƶ��,��λ:Mhz
void Tim2_Init(void)//50MS�ж�
{
	TIM_TimeBaseInitTypeDef TIM_TimeBaseInitStructure;
	
	NVIC_InitTypeDef NVIC_InitStructure;
	
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM2,ENABLE);  ///ʹ��TIM2ʱ��
	
	TIM_TimeBaseInitStructure.TIM_Period = 499; 	//�Զ���װ��ֵ
	TIM_TimeBaseInitStructure.TIM_Prescaler = 7199;  //��ʱ����Ƶ
	TIM_TimeBaseInitStructure.TIM_CounterMode=TIM_CounterMode_Up; //���ϼ���ģʽ
	TIM_TimeBaseInitStructure.TIM_ClockDivision=TIM_CKD_DIV1; 
	
	TIM_TimeBaseInit(TIM2,&TIM_TimeBaseInitStructure);//��ʼ��TIM2
	
	TIM_ITConfig(TIM2,TIM_IT_Update,ENABLE); //����ʱ��2�����ж�
	TIM_Cmd(TIM2,ENABLE); //ʹ�ܶ�ʱ��2
	TIM_ClearITPendingBit(TIM2,TIM_IT_Update);
	
	/* Enable CANx RX0 interrupt IRQ channel */
	
	NVIC_InitStructure.NVIC_IRQChannel = TIM2_IRQn;
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 2;
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 3;
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
	NVIC_Init(&NVIC_InitStructure);
	
	
}



