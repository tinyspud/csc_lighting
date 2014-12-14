#include <FreeRTOS.h>
#include "os_timer.h"

#include "reg_sci.h"
#include "sci.h"
#include "uart_mux.h"
//#include "sciRTOS.h"
#include "spi.h"
#include "common.h"


static TimerHandle_t xUARTMUXTimer;
static uint32_t uartmux_timer_expired_flag = pdFALSE;


/* UARTMUX timer callback function */
void prvUARTMUXTimerCallback(TimerHandle_t xExpiredTimer){
	uartmux_timer_expired_flag = TRUE_FLAG;
}

static struct UARTStatus_t
{
    uint32 currentPort;
    uint32 baudRate[4];
    uint32 prescaler[4];
} UARTStatus;

/* changes UARTStatus struct with given inputs */
void set_uart_status(uint32_t currentPort, uint32_t baudrate, uint32_t prescalar)
{
	UARTStatus.currentPort = currentPort;
	UARTStatus.baudRate[currentPort] = baudrate;
	UARTStatus.prescaler[currentPort] = prescalar;
}

/* wrapper function to check if current port is set for mux */
uint32 send_byte_on_uart(uint8 portNumber,uint8 sendByte) {
	uint32 tempSendStatus = 0;
	// mask to ensure no incorrect port number
	portNumber &= UART_PORTNUMMASK;

	if((UARTStatus.currentPort & UART_PORTNUMMASK) != portNumber) {
		reconfigure_uart(portNumber, FALSE_FLAG);
	}

	 /* send out text when ready   */
	tempSendStatus = xSerialPutChar( sendByte, 0 );


	return tempSendStatus;
}


/* switches mux for the given port
 * 	 TRUTH TABLE FOR UART MUX
 *	 B | A | Y_OUT
 *	 0 | 0 | PORT0_EN
 *	 0 | 1 | PORT1_EN
 *	 1 | 0 | PORT2_EN
 *	 1 | 1 | PORT3_EN
 * */
void switch_uart_mux(uint8 portNumber) {
	// Filters out of invalid Range Values
	portNumber &= 0x03U;

	switch(portNumber){
		case GSM_UART_MUX_PORT:
			/*SPI5CLK = UART_MUX_CONT_B */
			//spiPORT5->PC3 &= ~BIT9;
//			gioSetBit(spiPORT5, 9, 0);
			/*SPI4SIMO = UART_MUX_CONT_A */
			//spiPORT4->PC3 &= ~BIT10;
//			gioSetBit(spiPORT4, 10, 0);
			break;

		case GPS_UART_MUX_PORT:
			/*SPI5CLK = UART_MUX_CONT_B */
			//spiPORT5->PC3 &= ~BIT9;
//			gioSetBit(spiPORT5, 9, 0);
			/*SPI4SIMO = UART_MUX_CONT_A */
			//spiPORT4->PC3 |= BIT10;
//			gioSetBit(spiPORT4, 10, 1);
			break;

		case PRINTER_UART_MUX_PORT:
			/*SPI5CLK = UART_MUX_CONT_B */
			//spiPORT5->PC3 |= BIT9;
//			gioSetBit(spiPORT5, 9, 1);
			/*SPI4SIMO = UART_MUX_CONT_A */
			//spiPORT4->PC3 &= ~BIT10;
//			gioSetBit(spiPORT4, 10, 0);
			break;

		case ALFAT_UART_MUX_PORT:
			/*SPI5CLK = UART_MUX_CONT_B */
			//spiPORT5->PC3 |= BIT9;
//			gioSetBit(spiPORT5, 9, 1);
			/*SPI4SIMO = UART_MUX_CONT_A */
			//spiPORT4->PC3 &= ~BIT10;
//			gioSetBit(spiPORT4, 10, 1);
			break;

		default:
			/*SPI5CLK = UART_MUX_CONT_B */
			//spiPORT5->PC3 &= ~BIT9;
//			gioSetBit(spiPORT5, 9, 0);
			/*SPI4SIMO = UART_MUX_CONT_A */
			//spiPORT4->PC3 &= ~BIT10;
//			gioSetBit(spiPORT4, 10, 0);
			break;
	}

}


/* parse incoming GPS UART stream query response */
uint32_t flush_uart(const uint32_t timeout)
{

	char cByteRxed;
	uint32_t error_flag = pdPASS;


	/*set and start timer */
	uartmux_timer_expired_flag = FALSE_FLAG;
	if(xTimerChangePeriod(xUARTMUXTimer, timeout, portMAX_DELAY) != pdPASS){
		error_flag = pdFAIL;
	}
	if( xTimerStart(xUARTMUXTimer, portMAX_DELAY) != pdPASS){
		error_flag = pdFAIL;
	}


	for(;;){

		/* timed out waiting for this response */
		if(uartmux_timer_expired_flag == TRUE_FLAG){
			error_flag = pdPASS;
			break;
		}

		//keep reading
		xSerialGetChar( &cByteRxed, ( ( portTickType ) 0x007d ) );

	}

	return error_flag;
}

/* reconfigures uart on MCS for the given port */
uint32_t reconfigure_uart(uint8 portNumber, uint32_t force)
{
	uint32_t error_flag = pdPASS;

	portNumber &= UART_PORTNUMMASK;

	/*need to delay switching the baudrate for the switch baudrate command to send first, flush uart for 125 ms minimum */
	flush_uart(125);

	/* skip if it is already on the current uart port */
	if( ((UARTStatus.currentPort & UART_PORTNUMMASK) != portNumber) || force) {


		portNumber &= UART_PORTNUMMASK;

		switch_uart_mux(portNumber & UART_PORTNUMMASK);
		UARTStatus.currentPort = portNumber;

		/* set baudrate */
		sciSetBaudrate(scilinREG, UARTStatus.baudRate[portNumber]);
		//sciREG->BRS = UARTStatus.prescaler[portNumber];  /* baudrate *///715U;  /* baudrate */

	}

    return error_flag;
}

/* initializes uartmux by setting default values */
uint32_t init_uartmux(void) {
	uint32_t error_flag = pdFAIL;

	/* init uartmux status struct */
	UARTStatus.currentPort = UART_DEFAULT_PORT_NUM;
	UARTStatus.baudRate[0] = UART_DEFAULT_BAUD_RATE_CH0;
	UARTStatus.baudRate[1] = UART_DEFAULT_BAUD_RATE_CH1;
	UARTStatus.baudRate[2] = UART_DEFAULT_BAUD_RATE_CH2;
	UARTStatus.baudRate[3] = UART_DEFAULT_BAUD_RATE_CH3;
	UARTStatus.prescaler[0] = UART_DEFAULT_PRESCALE_CH0;
	UARTStatus.prescaler[1] = UART_DEFAULT_PRESCALE_CH1;
	UARTStatus.prescaler[2] = UART_DEFAULT_PRESCALE_CH2;
	UARTStatus.prescaler[3] = UART_DEFAULT_PRESCALE_CH3;


	/* Create generic UARTMUX timer */
	xUARTMUXTimer = xTimerCreate(
			"UARTMUX generic Timer",		/* Unique name */
			(0.125 * configTICK_RATE_HZ),	/* Period (# ticks) */
			pdFALSE,						/* Auto reload timer? */
			"UART_MUX_TIMER_ID",			/* Unique ID (so callback can link to several timers) */
			prvUARTMUXTimerCallback			/* Callback function (see above) */
	);

	/* Start UARTMUX timer - if it's already started, calling this just restarts it */
	uartmux_timer_expired_flag = FALSE_FLAG;

	switch_uart_mux(UART_DEFAULT_PORT_NUM);



	/* this is done in sci init already */
	//void initialize_uart_mux(void) {
	//	// Configure Direction
	//	UART_MUX_SW_A_PORT->DIR  |=  (1 << UART_MUX_SW_A_PIN);
	//	UART_MUX_SW_B_PORT->DIR  |=  (1 << UART_MUX_SW_B_PIN);
	//
	//
	//	// Enable 3.3V and 5V Regulator
	//	REG_3V3POWER_PORT->DIR  |=  (1 << REG_3V3POWER_PIN);
	//	REG_5VPOWER_PORT->DIR  	|=  (1 << REG_5VPOWER_PIN);
	//
	//	REG_5VPOWER_PORT->DOUT  |=  (1 << REG_5VPOWER_PIN);
	//	REG_3V3POWER_PORT->DOUT  |=  (1 << REG_3V3POWER_PIN);
	//
	//}

	return error_flag;
}

/* called by system init */
void uartmux_sci_init()
{
	/*initialize the sci peripherals */
	sciInit();

	/*init uart mux default i/o states */
	init_uartmux();
}


void UART_putChar(sciBASE_t* scireg, char cByteRxed){

}
