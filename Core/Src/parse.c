#include <stdio.h>
#include "usart.h"
#include <string.h>
#include "parse.h"

int validar(uint8_t *ptr_p){
	
	ptr_p = &Rx_Buffer[0];
	//uint8_t index_cmd;
	uint8_t op1, op2;
	int calc;
	
			//if (*ptr_p != 0) //rever esta condição para integração com o putty
	op1 = (*ptr_p << 1);
	ptr_p++;
	op2 = (*ptr_p << 1);
	calc = op1 ^ op2;
	calc += 0x20; //considerar o caracter ascii SPACE, com o valor 0x20h
	
	return calc;
}

void match () {
	char op=0;
	switch (op)
	{
		case 0:
			cmd_MR() ;
		
		break;
	}
}
void ascii2hex() { //prioridade, fazer isto primeiro

}
void cmd_MR(){

}

void cmd_MW(){

}

void cmd_MI(){

}

void cmd_MO(){

}

void cmd_RD(){

}

void cmd_WD(){

}

void cmd_RA(){

}

void parse(){
	
	

}

