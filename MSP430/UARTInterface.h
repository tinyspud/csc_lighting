#ifndef __UART_INTERFACE_H
#define __UART_INTERFACE_H

#define UART_FLAG_NOTHING_NEW	0x01
#define UART_FLAG_NEW_AVAILABLE	0x00

//#define TX_NOTIFICATION_ON_PIN

#define UART_RX_BUFFER_LEN	310

#define UART_RX_TIMEOUT	SYS_TIMER_HALF_SEC

typedef enum uart_status{
	UART_IDLE,
	UART_RECEIVING,
}uart_status_t;

typedef enum uart_tx_mode{
	FIFO,
	QUEUED,
}uart_tx_mode_t;

typedef enum uart_tx_endiannessmode{
	LEAVE_ENDIANNESS,
	REVERSE_ENDIANNESS,
}uart_tx_endiannessmode_t;

typedef struct uart_complex_tx_parameters{
	unsigned char * ptr2buffhead;
	int bufflen;
	uart_tx_endiannessmode_t needtoreverse;
}uart_complex_tx_parameters_t;

#define MAX_NUM_PARAM_IN_EXTENDED_UART_QUEUE	60

/* Init functions */
void init_uart_uca();

/* Function called in loop */
unsigned char check_and_lower_rx_flag(void);

void UARTServiceUCA1Rx();
void service_uart_UCA1_TX();

void start_long_tx();
void transmit_long_uart_message();


/* Transmit functions (do not use from ISR) */
void UARTTxNext();

/* Enqueue */
void EnqueueToSendUART(char*, unsigned int);

void EnqueueCharToSendUART(char);

void EnqueueUCharToSendUART(unsigned char);

void SetADCEnqueueFlag(void);

void EnqueueStringUART(char*);

void EnqueueCharHexUART(char);

void EnqueueULongHexUART(unsigned long);

void EnqueueUIntHexUART(unsigned int);

unsigned char* GetUARTTxBufferPtr(unsigned int *);

void AppendToComplexUARTTxQueue(unsigned char*, int, signed char *, uart_tx_endiannessmode_t);

void SetPotentialCommandStartingPointer(unsigned char *);

#endif

