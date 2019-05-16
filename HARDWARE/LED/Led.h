#ifndef _LED_H_
#define _LED_H_
#include "sys.h"

//LED1ָʾRS485����״̬
//LED2ָʾRS232����״̬
//LED3ָʾsocket����״̬
#define LED1 		PAout(11)
#define LED2 		PAout(12)

#define OFF     1
#define ON      0
//50ms interrupt,led toggle time = LED_XXX * 50ms
#define LED_SLOW 40  
#define LED_MID  10
#define LED_FAST 1

void LED_Init(void);


void LED_Running(void);

void LED_COMing(void);

void LED_Sleep(void);

void LED_COMFinish(void);
void LED_Debug(void);

void LED_DownloadOK(void);

void LED_ProgramOK(void);

void LED_COMFailed(void);

#endif
