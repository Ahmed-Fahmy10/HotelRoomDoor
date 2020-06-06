#include "stdint.h"
#include "std_types.h"
#include "tm4c123gh6pm.h"
#include "uart.h"

#ifndef __main_h
#define __main_h

void Solenoide_Lock(int room_number);
void Solenoide_Unlock(int room_number);
void Set_Password(int room_number);
void Clear_Password(int room_number);
bool Check_Password(int room_number);
char KeyPad(void);
char Table_KeyPad(char nColume, char nRow);

#endif /* __main_h */
/* end of main.h */
