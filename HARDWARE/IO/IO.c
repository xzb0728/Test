#include "IO.h" 
IO IO1,IO2;


void IO1_Init_Input()
{
	
		GPIO_InitTypeDef  GPIO_InitStructure;
	/* Enable the GPIO_LED Clock */
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB, ENABLE); 		
	
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_1;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPU;
	GPIO_Init(GPIOB, &GPIO_InitStructure);
	IO1.Sta = Input;
}

char IO1_ReadInput()
{
	IO1.Val = GPIO_ReadInputDataBit(GPIOB,GPIO_Pin_1);
	return IO1.Val;
}

