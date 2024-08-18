#ifndef __SERIAL_H
#define __SERIAL_H

#include <stdio.h>

extern uint8_t Serial_TxPacket[];
extern uint8_t Serial_RxPacket[];

void Serial_Init(void);
void Serial_SendByte(uint8_t Byte);
void Serial_SendArray(uint8_t *Array, uint16_t Length);
void Serial_SendString(char *Str);
void Serial_SendNumber(uint32_t Num, uint32_t Length);


// uint8_t Serial_GetRxFlag(void);
uint8_t Serial_K210DataFlag(void);
void Serial_SendPacket(void);
#endif
