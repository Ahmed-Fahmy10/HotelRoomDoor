#include "stdint.h"
#include "tm4c123gh6pm.h"
#include "uart.h"

void UART_Init(void){
	SYSCTL_RCGCUART_R |= 0x01 ; //Enable clk to UART0
	SYSCTL_RCGCGPIO_R |= 0x01; //Enable clk to GPIO (tx , Rx)
	UART0_CTL_R &= ~0x01; // disable UART0
	UART0_IBRD_R = 520; //setting BR to 9600 bits/sec -> putting the int of the dividor
	UART0_FBRD_R = 53; //setting BR to 9600 bits/sec -> puting the fraction of the dividor
	UART0_LCRH_R =0x0070;  // 8 bit word length (no parity bits, one stop bit, FIFOs)
	UART0_CTL_R= 0x301;// enable Tx , Rx ,and UART0
	GPIO_PORTA_AFSEL_R |= 0x03; // enable altern function
	GPIO_PORTA_PCTL_R = (GPIO_PORTA_PCTL_R & 0xFFFFFF00) + 0x11; // configure pctl for UART
	GPIO_PORTA_DEN_R |= 0x33; //enable digital
	GPIO_PORTA_AMSEL_R &= ~0x03; // disbale analog
	
}

char UART_receive (void){
	while((UART0_FR_R & 0x0010) !=0); //infinite loop if the register is empty
	return (char)(UART0_DR_R & 0xFF); // return the recived char	
}

void UART0_send(char data){
	while((UART0_FR_R & 0x0020) !=0); //infinite loop if the register is full
	UART0_DR_R = data; // send the data to register 
	
}
