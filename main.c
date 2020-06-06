#include "main.h"
#include "config.h"

#define ROOM	2
#define PASSWORDLENGTH	4

#define COLUME	3
#define ROW			4
#define BIT_IS_CLEAR(REG,BIT) ( !(REG & (1<<BIT)) )

uint16_t nRoomNum;
uint16_t nPassword[ROOM][PASSWORDLENGTH];
char nNewPassword[PASSWORDLENGTH], nLoadPassword[PASSWORDLENGTH], i, nNewIndex, nLoadIndex;
bool bResetPassword, bCheckPassword, bSetRoom;
int main(void){
	
	char nRoomNumber, nRX_Buf, nKeyStatus;
	nRoomNumber = 0;
	nNewIndex = 0;
	nLoadIndex = 0;
	GPIO_Init(); /*Initialization OF GPIO*/
	UART_Init();	/*Initialization OF UART*/
	bResetPassword = FALSE;
	bCheckPassword = FALSE;
	bSetRoom = FALSE;
	for(i = 0; i < PASSWORDLENGTH; i++){
		nNewPassword[i] = 0;
	}
	for(i = 0; i < ROOM; i++){
		Clear_Password(i);
		Solenoide_Lock(i);
	}
	while(1){
		//--------The UART_RX is actived------------------//
		if( bSetRoom == FALSE){						// Room Number detection.
			nRoomNumber = UART_receive();
			if((nRoomNumber > 0)&&(nRoomNumber <= ROOM)) bSetRoom =TRUE;
		}
		else{															// After detecte room number, read the password of the room
			nRX_Buf = UART_receive();
			if((nRX_Buf > 0)&&(nRX_Buf < 10)){
				nNewPassword[nNewIndex] = nRX_Buf;
				nNewIndex++;
				if(nNewIndex >= PASSWORDLENGTH){	// read the password of the room
					nNewIndex = 0;
				}
			}
			else{															// New password setting or the password of the room checking by the receptionist's order
				if(nRX_Buf == 11) bResetPassword = TRUE;
				else if(nRX_Buf == 12) bCheckPassword = TRUE;
			}
		}
		//-------------The keypad is pressed------------//
		nKeyStatus = KeyPad();						// Keypad detection.
		if(nKeyStatus != 10){							// when the keypad is pressed
			if(nKeyStatus == 11){						// when the command keypad is pressed
				for(i = 0; i < PASSWORDLENGTH; i++){  // load the password by the keypad's password
					nNewPassword[i] = nLoadPassword[i];
				}
				while(bCheckPassword == TRUE){		// control the Solenoide by the password
					if(nRoomNumber >= ROOM){ 
						bCheckPassword = FALSE;
						nRoomNumber = 0;
					}
					else{
						if(Check_Password(nRoomNumber) == TRUE){
							bCheckPassword = FALSE;
							Solenoide_Unlock(nRoomNumber);
						}
						else{
							Solenoide_Lock(nRoomNumber);
							nRoomNumber++;
						}
					}					
				}
			}
			else{   															// input the password
				nLoadPassword[nLoadIndex] = nKeyStatus;
				nLoadIndex++;
				if(nLoadIndex >= PASSWORDLENGTH) nLoadIndex = 0;
			}
		}
		//-------------process------------//	
		if(bResetPassword == TRUE){							// reset the password of the room.
			bResetPassword = FALSE;
			Clear_Password(nRoomNumber);
			Set_Password(nRoomNumber);
		}
		if(bCheckPassword == TRUE){							// control the Solenoide by the password
			bCheckPassword = FALSE;
			if(Check_Password(nRoomNumber) == TRUE) Solenoide_Unlock(nRoomNumber);
			else Solenoide_Lock(nRoomNumber);
		}
	}
}

/*Function to Lock_Solenoide*/
void Solenoide_Lock(int room_number){
	switch(room_number){
		case 1: GPIO_PORTF_DATA_R = GPIO_PORTF_DATA_R | 0x01; break;
		case 2: GPIO_PORTF_DATA_R = GPIO_PORTF_DATA_R | 0x02; break;
		default: GPIO_PORTF_DATA_R = 0x00; break;
	}
}
        /*Function to Unlock_Solenoide*/
void Solenoide_Unlock(int room_number){
	switch(room_number){
		case 1: GPIO_PORTF_DATA_R = GPIO_PORTF_DATA_R & (~0x01); break;
		case 2: GPIO_PORTF_DATA_R = GPIO_PORTF_DATA_R & (~0x02); break;
		default: break;
	}
}
        /*Function to Set_Password*/
void Set_Password(int room_number){
	for(i = 0; i < PASSWORDLENGTH; i++){
		nPassword[room_number][i] = nNewPassword[i];
	}
}
        /*Function to Clear_Password*/
void Clear_Password(int room_number){
	for(i = 0; i < PASSWORDLENGTH; i++){
		nPassword[room_number][i] = 0;
	}
}
        /*Function to Check_Password*/
bool Check_Password(int room_number){
	for(i = 0; i < PASSWORDLENGTH; i++){
		if(nPassword[room_number][i] != nNewPassword[i]) return FALSE;
	}
	return TRUE;
}
char KeyPad(){
	char nCol, nRow, status;
	status = 10;
	nCol = 0x01;
	for(i = 0; i < COLUME; i++){
		GPIO_PORTE_DATA_R = nCol;
		GPIO_PORTC_DATA_R = 0xFFFFFFFF;
		for(nRow = 0; nRow < ROW; nRow++){
			if(BIT_IS_CLEAR(GPIO_PORTC_DATA_R,nRow)) break;
		}
		status = Table_KeyPad(nCol, nRow);
	}
	return status;
}
char Table_KeyPad(char nColume, char nRow){
	switch(nColume){
		case 0: 
			switch(nRow){
				case 0: return 1;
				case 1: return 2;
				case 2: return 3;
				case 3: return 4;
				default: return 10;
			}
		case 1:
			switch(nRow){
				case 0: return 5;
				case 1: return 6;
				case 2: return 7;
				case 3: return 8;
				default: return 10;
			}
		case 2:
			switch(nRow){
				case 0: return 9;
				case 1: bResetPassword = TRUE; return 11;
				case 2: return 0;
				case 3: bCheckPassword = TRUE; return 11;
				default: return 10;
			}
		default: return 10;
	}
}
