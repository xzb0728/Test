#include "Tim2.h"
#include "stm32f10x.h"

//定时器2配置
//定时器溢出时间计算方法:Tout=((4999+1)*(8399+1))/Ft us.
//Ft=定时器工作频率,单位:Mhz
void Tim2_Init(void)//50MS中断
{
	TIM_TimeBaseInitTypeDef TIM_TimeBaseInitStructure;
	
	NVIC_InitTypeDef NVIC_InitStructure;
	
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM2,ENABLE);  ///使能TIM2时钟
	
	TIM_TimeBaseInitStructure.TIM_Period = 499; 	//自动重装载值
	TIM_TimeBaseInitStructure.TIM_Prescaler = 7199;  //定时器分频
	TIM_TimeBaseInitStructure.TIM_CounterMode=TIM_CounterMode_Up; //向上计数模式
	TIM_TimeBaseInitStructure.TIM_ClockDivision=TIM_CKD_DIV1; 
	
	TIM_TimeBaseInit(TIM2,&TIM_TimeBaseInitStructure);//初始化TIM2
	
	TIM_ITConfig(TIM2,TIM_IT_Update,ENABLE); //允许定时器2更新中断
	TIM_Cmd(TIM2,ENABLE); //使能定时器2
	TIM_ClearITPendingBit(TIM2,TIM_IT_Update);
	
	/* Enable CANx RX0 interrupt IRQ channel */
	
	NVIC_InitStructure.NVIC_IRQChannel = TIM2_IRQn;
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 2;
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 3;
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
	NVIC_Init(&NVIC_InitStructure);
	
	
}



