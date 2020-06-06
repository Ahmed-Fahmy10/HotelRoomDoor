#include "config.h"

void GPIO_Init(void){
	SYSCTL_RCGCGPIO_R |= 0x00000004; //activate portc
	while( (SYSCTL_RCGCGPIO_R & 0x04)==0 ) ;   
	GPIO_PORTC_DIR_R = 0X00; //input 
	GPIO_PORTC_DEN_R = 0XFF;  //enable digital
	GPIO_PORTC_DATA_R=0x00; 
	
	SYSCTL_RCGCGPIO_R |= 0x00000008; //activate porte
	while( (SYSCTL_RCGCGPIO_R & 0x08)==0 ) ; 
	GPIO_PORTE_DIR_R = 0XFF ; //output 
	GPIO_PORTE_DEN_R = 0XFF;  //enable digital
	GPIO_PORTE_DATA_R=0x00; 
	
	SYSCTL_RCGCGPIO_R |= 0x00000020; //activate portf
	while( (SYSCTL_RCGCGPIO_R & 0x20)==0 ) ;   
	GPIO_PORTF_DIR_R = 0X03; // pf0, pf1 is output, other pins is input.  
	GPIO_PORTF_DEN_R = 0X03;  //enable digital
	GPIO_PORTF_LOCK_R = 0x4C4F434B; //unlock portf
	GPIO_PORTF_DATA_R=0x00; //led off initially
}
