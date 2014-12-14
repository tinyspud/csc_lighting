/*
 * uart_mux.h
 *
 *  Created on: Dec 26, 2013
 *      Author: skatsu
 */

#ifndef UART_MUX_H_
#define UART_MUX_H_
#include "sys_common.h"
#include "reg_sci.h"


// DEFINE MUX OUT PORTS
#define UARTMUXOUT0 0U
#define UARTMUXOUT1 1U
#define UARTMUXOUT2 2U
#define UARTMUXOUT3 3U

#define GSM_UART_MUX_PORT UARTMUXOUT0
#define GPS_UART_MUX_PORT UARTMUXOUT1
#define PRINTER_UART_MUX_PORT UARTMUXOUT2
#define ALFAT_UART_MUX_PORT UARTMUXOUT3


//- serial configuration
// CH0_BAUD_RATE (GSM)
// CH1_BAUD_RATE (GPS)
// CH2_BAUD_RATE (RESERVED)
// CH3_BAUD_RATE (RESERVED)
#define UART_DEFAULT_BAUD_RATE_CH0	115200U//38400U//115200U
#define UART_DEFAULT_BAUD_RATE_CH1	9600U
#define UART_DEFAULT_BAUD_RATE_CH2	19200U
#define UART_DEFAULT_BAUD_RATE_CH3	9600U
#define UART_DEFAULT_PRESCALE_CH0	59U//178U//59U
#define UART_DEFAULT_PRESCALE_CH1	715U
#define UART_DEFAULT_PRESCALE_CH2	357U
#define UART_DEFAULT_PRESCALE_CH3	25U
#define UART_DEFAULT_PORT_NUM		0U				// 0 = GSM, 1=GPS, 2=printer

/* operational baudrates */
#define UART_GSM_BAUD_RATE_CH0		38400U
#define UART_GPS_BAUD_RATE_CH1		9600U
#define UART_PRINTER_BAUD_RATE_CH2	9600U
#define UART_GSM_PRESCALE_CH0		178U
#define UART_GPS_PRESCALE_CH1		715U
#define UART_PRINTER_PRESCALE_CH2	715U


#define UART_PORTNUMMASK	3U

//// GPIO Ports Needed
//#define UART_MUX_SW_A_PORT 	gioPORTA
//#define UART_MUX_SW_B_PORT 	gioPORTA
//
//// GPIO ports for Power Regulator
//#define REG_3V3POWER_PORT 	gioPORTA
//#define REG_5VPOWER_PORT 	gioPORTA
//
//
//// GPIO port number
//#define UART_MUX_SW_A_PIN 	5U
//#define UART_MUX_SW_B_PIN 	4U

//// GPIO port number power REGULATORS
//#define REG_3V3POWER_PIN 		3U
//#define REG_5VPOWER_PIN 		1U



uint32 send_byte_on_uart(uint8 portNumber,uint8 sendByte);
uint32_t init_uartmux(void);
uint32_t reconfigure_uart(uint8 portNumber, uint32_t force);
void set_uart_status(uint32_t currentPort, uint32_t baudrate, uint32_t prescalar);
void uartmux_sci_init(void);
uint32_t flush_uart(const uint32_t timeout);

void UART_putChar(sciBASE_t* scireg, char cByteRxed);

#endif /* UART_MUX_H_ */
