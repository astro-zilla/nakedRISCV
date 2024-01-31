#pragma once

#include <stdint.h>
#define IO_ADDR 0xfffff500

struct NAKEDUART {
	uint8_t UART_CTRL_EN : 1;
	uint8_t UART_CTRL_SIM_MODE : 1;
	uint8_t UART_CTRL_HWFC_EN : 1;
	uint8_t UART_CTRL_PRSC : 3;
	uint8_t UART_CTRL_BAUD_U : 2;

	uint8_t UART_CTRL_BAUD_L;

	uint8_t UART_CTRL_RX_NEMPTY : 1;
	uint8_t UART_CTRL_RX_HALF : 1;
	uint8_t UART_CTRL_RX_FULL : 1;
	uint8_t UART_CTRL_TX_NEMPTY : 1;
	uint8_t UART_CTRL_TX_HALF : 1;
	uint8_t UART_CTRL_TX_FULL : 1;
	uint8_t UART_CTRL_IRQ_RX_NEMPTY : 1;
	uint8_t UART_CTRL_IRQ_RX_HALF : 1;

	uint8_t UART_CTRL_IRQ_RX_FULL : 1;
	uint8_t UART_CTRL_IRQ_TX_NEMPTY : 1;
	uint8_t UART_CTRL_IRQ_TX_NHALF : 1;
	uint8_t __RESERVED_1 : 3;
	uint8_t UART_CTRL_RX_OVER : 1;
	uint8_t UART_CTRL_TX_BUSY : 1;

	uint8_t UART_DATA_RTX : 8;

	uint8_t UART_DATA_RX_FIFO_SIZE : 4;
	uint8_t UART_DATA_TX_FIFO_SIZE : 4;

	uint8_t __RESERVED_2;
	uint8_t __RESERVED_3;

	uint8_t __RESERVED_4[248];

};

extern volatile struct NAKEDUART *uart;
