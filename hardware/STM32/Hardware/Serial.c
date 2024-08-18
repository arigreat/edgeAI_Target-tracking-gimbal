#include "stm32f10x.h"                  // Device header
#include <stdio.h>

uint8_t Serial_TxPacket[2];
uint8_t Serial_RxPacket[2];
uint8_t Serial_RxFlag;

void Serial_Init(void)
{
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_USART2,ENABLE);
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_USART1,ENABLE);
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA,ENABLE);
	
	GPIO_InitTypeDef GPIO_InitStructure;
	// USART1 配置PA9 TX引脚
	// USART2 配置PA2 TX引脚
	// TX引脚选择输出模式 复用推挽输出（可由外设控制输出控制）
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_9|GPIO_Pin_2;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(GPIOA, &GPIO_InitStructure);
	// USART1 配置PA10 RX引脚
	// USART2 配置PA3 RX引脚
	// RX引脚选择输入模式 串口空闲状态是高电平 使用上拉输入
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPU;
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_10|GPIO_Pin_3;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(GPIOA, &GPIO_InitStructure);
	
	
	// 配置USART
	USART_InitTypeDef USART_InitStructure;
	USART_InitStructure.USART_BaudRate = 9600; // 波特率
	USART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None; // 硬件流控制 无流控
	USART_InitStructure.USART_Mode = USART_Mode_Tx | USART_Mode_Rx; // 设置TX RX模式
	USART_InitStructure.USART_Parity = USART_Parity_No; // 校验位
	USART_InitStructure.USART_StopBits = USART_StopBits_1; // 停止位
	USART_InitStructure.USART_WordLength = USART_WordLength_8b; // 8位字长
	USART_Init(USART2, &USART_InitStructure);
	USART_Init(USART1, &USART_InitStructure);
	
	// 开启中断 USART_IT_RXNE置1就会申请中断
	USART_ITConfig(USART1,USART_IT_RXNE, ENABLE);
	USART_ITConfig(USART2,USART_IT_RXNE, ENABLE);
	
	
	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2);
	NVIC_InitTypeDef NVIC_InitStructure;
	
	NVIC_InitStructure.NVIC_IRQChannel = USART1_IRQn;
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 1;
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 1;
	NVIC_Init(&NVIC_InitStructure);
	
	NVIC_InitStructure.NVIC_IRQChannel = USART2_IRQn;
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 1;
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 1;
	NVIC_Init(&NVIC_InitStructure);
	
	USART_Cmd(USART1,ENABLE);
	USART_Cmd(USART2,ENABLE);
}


void Serial_SendByte(uint8_t Byte)
{
	USART_SendData(USART2,Byte); 
	// 检查数据是否发送成功 检查TXE发送数据寄存器空标志位
	while(USART_GetFlagStatus(USART2,USART_FLAG_TXE) == RESET);
}

void Serial_SendArray(uint8_t *Array, uint16_t Length)
{
	for(uint16_t i=0;i<Length;i++)
	{
		Serial_SendByte(Array[i]);
	}
}

void Serial_SendString(char *Str)
{
	for(uint16_t i=0;Str[i]!='\0';i++)
	{
		Serial_SendByte(Str[i]);
	}
}

uint32_t Serial_Pow(uint32_t x, uint32_t y)
{
	uint32_t result = 1;
	while(y--)
	{
		result *= x;
	}
	return result;
}

void Serial_SendNumber(uint32_t Num, uint32_t Length)
{
	uint8_t i;
	for(i = 0; i < Length ; i++)
	{
		Serial_SendByte(Num / Serial_Pow(10,Length-i-1) % 10 + '0');
	}
}

// 使用printf函数 注意开启魔法棒 Use MicroLIB 选项
// 重定向printf到串口(printf通过调用fputc进行输出)
int fputc(int ch, FILE *f)
{
	Serial_SendByte(ch);
	return ch;
}

uint8_t Serial_GetRxFlag(void)
{
	if(Serial_RxFlag==1)
	{
		Serial_RxFlag = 0;
		return 1;
	}
	return 0;
}

uint8_t Serial_K210DataFlag(void)
{
	return Serial_GetRxFlag();
}

void Serial_SendPacket(void)
{
	Serial_SendByte(0xFF);
	Serial_SendArray(Serial_TxPacket,2);
	Serial_SendByte(0xFE);

}



// 中断接收函数
void USART2_IRQHandler(void)
{
	// Serial_SendString("USART2HandlerStart\r\n");
	static uint8_t RxState = 0;
	static uint8_t PtrRxPacket = 0;
	if(USART_GetFlagStatus(USART2,USART_IT_RXNE)==SET)
	{
		uint8_t RxData = USART_ReceiveData(USART2);
		if(RxState == 0)
		{
			if(RxData == 0xFF)
			{
				RxState = 1;
				PtrRxPacket = 0;
				// Serial_SendString("USART2HandlerGetHead\r\n");
			}
		}
		else if (RxState == 1)
		{
			Serial_RxPacket[PtrRxPacket] = RxData;
			Serial_SendByte(Serial_RxPacket[PtrRxPacket++]);
			if(PtrRxPacket >= 2)
			{
				RxState = 2;
				// Serial_SendString("GetData\r\n");
			}
		}
		else if (RxState == 2)
		{
			if(RxData == 0xFE)
			{
				RxState = 0;
				Serial_RxFlag = 1;
				// Serial_SendString("USART2HandlerGetTail\r\n");

			}
		}
	
		USART_ClearITPendingBit(USART2,USART_IT_RXNE);
		
		// Serial_SendString("USART2HandlerSEnd\r\n");
	}
}

