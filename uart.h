#include "stdint.h"
#include "tm4c123gh6pm.h"

#ifndef __uart_h
#define __uart_h

void UART_Init(void);
char UART_receive (void);
void UART0_send(char data);

#endif /* __uart_h */
/* end of uart.h */
