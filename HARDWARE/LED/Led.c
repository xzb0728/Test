#include "led.h" 
#include "Tim2.h"
//LED 运行参数
struct LED
{
	unsigned short CountRun_Max;
	unsigned short CountCom_Max;
	unsigned short CounterRun;
	unsigned short CounterCom;
}LED_Para;

//LED IO初始化
void LED_Init(void)
{
	
	GPIO_InitTypeDef  GPIO_InitStructure;
	/* Enable the GPIO_LED Clock */
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA, ENABLE); 		
	
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_11 | GPIO_Pin_12;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(GPIOA, &GPIO_InitStructure);
	/*初始化完后，关闭2个LED*/ 
	LED1 = OFF;
	LED2 = OFF;
	//初始化定时器2
	Tim2_Init();

}
//正在工作
void LED_Running()
{
		TIM_Cmd(TIM2,DISABLE);
		LED1 = ON;
		
}
//正在发信
void LED_COMing()
{
		TIM_Cmd(TIM2,DISABLE);
		LED2 = ON;
		
}
//不工作，深度休眠
void LED_Sleep()
{
		TIM_Cmd(TIM2,DISABLE);	
		LED1 = OFF;
			
}
//发信结束
void LED_COMFinish()
{
		TIM_Cmd(TIM2,DISABLE);
		LED2 = OFF;
		
}
//正工作在调试状态
void LED_Debug()//2s间闪
{
		LED_Para.CountRun_Max = LED_SLOW;
		LED_Para.CounterCom = 0;
		TIM_Cmd(TIM2,ENABLE);
}
//一起表示数据下载成功
void LED_DownloadOK()//500ms慢闪
{
		LED_Para.CountRun_Max = LED_MID;
		LED_Para.CounterRun = 0;
		LED_Para.CountCom_Max = LED_MID;
		LED_Para.CounterCom = 0;
		TIM_Cmd(TIM2,ENABLE);
}
//TEST按下超过5秒或者程序更新成功
void LED_ProgramOK()//50ms快闪
{
		LED_Para.CountRun_Max = LED_FAST;
		LED_Para.CounterRun = 0;
		TIM_Cmd(TIM2,ENABLE);
}
//发信失败
void LED_COMFailed()//50ms快闪
{
		LED_Para.CountCom_Max = LED_FAST;
		LED_Para.CounterCom = 0;
		TIM_Cmd(TIM2,ENABLE);
}

//定时器2中断函数
//每隔	time（50ms）中断一次
void TIM2_IRQHandler(void)
{
	if(TIM_GetITStatus(TIM2,TIM_IT_Update) != RESET) 
	{
		if(LED_Para.CountRun_Max)//Run运行最大值不为0
		{
			if(LED_Para.CounterRun >= LED_Para.CountRun_Max)
			{
					LED_Para.CounterRun = 0;
					LED1 = ~LED1;
			}
			LED_Para.CounterRun++;
		}
		if(LED_Para.CountCom_Max)//Com运行最大值不为0
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




