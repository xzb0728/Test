#ifndef _IO_H_
#define _IO_H_

#include "sys.h"

#define Input  0
#define Output 1
typedef struct
{
	char Sta;
	char Val;
}IO;

extern IO IO1,IO2;

void IO1_Init_Input(void);
char IO1_ReadInput(void);



#endif
