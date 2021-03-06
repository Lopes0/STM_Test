#ifndef __PARSE_H__
#define __PARSE_H__

#include <stdio.h>
#include <stdint.h>

static uint8_t size;
void parse(void);
int validar(uint8_t *ptr_p);
void transmit(void);
void cmd_MR(void);
void cmd_MW(void);
void cmd_MI(void);
void cmd_MO(void);
void cmd_RD(void);
void cmd_WD(void);
void cmd_RA(void);
void match(void);
void (*command[])(void)={cmd_MR, cmd_MW, cmd_MI, cmd_MO, cmd_RD, cmd_WD,cmd_RA};

#endif 



