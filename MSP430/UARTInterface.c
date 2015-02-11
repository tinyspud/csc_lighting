#include "Common.h"

#include "UARTInterface.h"

#include "LEDControl.h"
#include "SystemTimer.h"

#define UART_RX_TIMEOUT_TICKS	SYS_TIMER_QUARTER_SEC /* 32768 Hz this corresponds to 1/4 second */
//#define UART_RX_TIMEOUT_TICKS	32768 /* 1 second (for manual typing since I'm slow) */

unsigned long _last_rx_tick;

volatile uart_status_t _uart_0_status;

volatile unsigned char send_adc_flag;

unsigned char _uart_rx_buffer[UART_RX_BUFFER_LEN];
unsigned char* _p_uart_rx_ptr;
unsigned char* _last_scanned_uart_rx_ptr;

volatile unsigned char * _potential_command_start_ptr;

volatile uint8_t transmit_out_queued = 0;
volatile uart_tx_mode_t _uart_tx_operation_mode = FIFO;

#define UART_TX_BUFFER_LEN	310
unsigned char _uart_tx_buffer[UART_TX_BUFFER_LEN];
unsigned char* _p_uart_tx_ptr;
unsigned char* _last_sent_uart_tx_ptr;

uart_complex_tx_parameters_t complex_tx_sequence[MAX_NUM_PARAM_IN_EXTENDED_UART_QUEUE];
int num_param_in_extended_uart_queue = 0;

int _possible_cmd;
#define UNKNOWN_CMD		-2
#define POTENTIAL_CMD	-1

uint32_t _last_uart_rx_time = 0;

/* Local prototype (internal) */
int _try_link_command(unsigned char*, unsigned char*);
void _reset_buffer_ptr_and_reset_uart_rx();
void _tx_char(unsigned char);
void _clear_complex_tx_list();
void _resetpotentialcommandstartingpointer();
unsigned char * _get_potential_command_str_ptr();


#ifdef DEBUG_W_PC
#define NUMBER_OF_COMMANDS	6
#else
#define NUMBER_OF_COMMANDS	1
#endif
const cmd_struct_t _uart_commands[NUMBER_OF_COMMANDS] = {
#ifdef DEBUG_W_PC

		/* LED command */
		{
				"L",
				UART_LED_CMD_LEN,
				UART_LED_LEN,
				UART_LED_LEN,
				&LEDCommand,
		},
#endif
};

void _reset_tx_buffer() {
	_p_uart_tx_ptr = &_uart_tx_buffer[0];
	_last_sent_uart_tx_ptr = _p_uart_tx_ptr;
}

void init_uart_uca() {
	/* Set up the UART */
	/* Set up the UART clock to the XT2 (4.000 MHz):
	 * BRCLK = 4 000 000 Hz
	 * Baud rate: 57600
	 * UCBRx: 69
	 * UCBRSx: 4
	 * UCBRFx: 0
	 *
	 * */

	int i = 0;
	_uart_0_status = UART_IDLE;
	_uart_tx_operation_mode = FIFO;
	transmit_out_queued = 0;
	_reset_tx_buffer();

	P4SEL |= BIT4 | BIT5;                       // P4.4,5 = USCI_A0 TXD/RXD
	UCA1CTL1 |= UCSWRST;                      // **Put state machine in reset**
#ifdef USE_4_MHZ_SMCLK
	UCA1CTL1 |= UCSSEL_2;                     // SMCLK (4MHz)
	UCA1BR0 = 69;                             // 4MHz 57600 (see User's Guide)
	UCA1BR1 = 0;
	UCA1MCTL |= UCBRS_4 + UCBRF_0;            // Modulation UCBRSx=1, UCBRFx=0
#else
	UCA1CTL1 |= UCSSEL_2;                     // SMCLK (1MHz)
	UCA1BR0 = 17;// 1MHz 57600 (see User's Guide)
	UCA1BR1 = 0;
	UCA1MCTL |= UCBRS_3 + UCBRF_0;// Modulation UCBRSx=1, UCBRFx=0
#endif

	UCA1CTL0 = 0;

	/* Init the buffer */
	for (i = 0; i < UART_RX_BUFFER_LEN; i++)
		_uart_rx_buffer[i] = 0x00;

	_last_rx_tick = 0;

	for (i = 0; i < UART_TX_BUFFER_LEN; i++)
		_uart_tx_buffer[i] = 0x00;

	send_adc_flag = 0;
	_clear_complex_tx_list();
	_reset_buffer_ptr_and_reset_uart_rx();
	_resetpotentialcommandstartingpointer();

	_last_uart_rx_time = 0;

	UCA1CTL1 &= ~UCSWRST;                   // **Initialize USCI state machine**
	UCA1IE |= UCRXIE;                         // Enable USCI_A0 RX interrupt

#ifdef TX_NOTIFICATION_ON_PIN
	TX_ACTIVE_PORT |= TX_ACTIVE_PIN;
	TX_ACTIVE_DIR |= TX_ACTIVE_PIN;
#endif
}

#if defined(__TI_COMPILER_VERSION__) || defined(__IAR_SYSTEMS_ICC__)
#pragma vector=USCI_A1_VECTOR
__interrupt void USCI_A1_ISR(void)
#elif defined(__GNUC__)
void __attribute__ ((interrupt(USCI_A0_VECTOR))) USCI_A0_ISR (void)
#else
#error Compiler not supported!
#endif
{
	switch (__even_in_range(UCA1IV, 4)) {
	case 0:
		break;                             // Vector 0 - no interrupt
	case 2:                                   // Vector 2 - RXIFG

		/* Place char in rx buffer */
		*_p_uart_rx_ptr = UCA1RXBUF;
		if (_p_uart_rx_ptr >= &_uart_rx_buffer[UART_RX_BUFFER_LEN - 1]) {
			/* Circle the buffer; message is longer than accepted in buffer */
			//			_reset_buffer_ptr_and_reset_uart_rx();
			/* Circle the buffer */
			_p_uart_rx_ptr = &_uart_rx_buffer[0];
		} else {
			_p_uart_rx_ptr++;
		}

		/* Set flag to check up */
		_uart_0_status = UART_RECEIVING;

		/* TODO figure out of this is taking too much time in the ISR */
		_last_uart_rx_time = GetCurrentLongTick();
		/* Make the CPU think again (go back to main for loop) */
		LPM3_EXIT;
		break;
	case 4:
		break;                             // Vector 4 - TXIFG
	default:
		break;
	}
}

/* Send character (1) */
void _tx_char(unsigned char to_send) {
	while (!(UCA1IFG & UCTXIFG))
		;             // USCI_A0 TX buffer ready?
	UCA1TXBUF = to_send;                  // TX -> RXed character
}

int _try_link_command(unsigned char* start, unsigned char* end) {
	unsigned int i = 0;
	int rtnval = UNKNOWN_CMD;
	for (i = 0; i < NUMBER_OF_COMMANDS; i++) {
		/* Check the command to see if it's plausible */
		if (_uart_commands[i].CmdHeaderLen == 1) {
			if (start[0] == _uart_commands[i].CmdHeader[0]) {
				return i;
			}
		} else {
			/* Multiple character header */
			/* See if the header's length is appropriate */
			if ((end - start) >= _uart_commands[i].CmdHeaderLen) {
				/* Might work */
				unsigned int j = 0;
				for (j = 0; j < _uart_commands[i].CmdHeaderLen; j++) {
					if (_uart_commands[i].CmdHeader[j] != start[j])
						break;
				}
				/* If it made it all the way through then the length is the
				 * counter's value */
				if (j == _uart_commands[i].CmdHeaderLen)
					return i;
			} else
				rtnval = POTENTIAL_CMD;
		}
	}
	return rtnval;
}

void _clear_complex_tx_list(){
	unsigned int i = 0;
	for (i = 0; i < MAX_NUM_PARAM_IN_EXTENDED_UART_QUEUE; i++) {
		complex_tx_sequence[i].bufflen = 0;
		complex_tx_sequence[i].ptr2buffhead = NULL;
		complex_tx_sequence[i].needtoreverse = LEAVE_ENDIANNESS;
	}
	num_param_in_extended_uart_queue = 0;
}

/* TODO make cognizant of the endianess of the data being passed in */
void AppendToComplexUARTTxQueue(unsigned char* dataptr, int datalen, signed char * checksum, uart_tx_endiannessmode_t reverseendianness){
	if (((num_param_in_extended_uart_queue + 1) < MAX_NUM_PARAM_IN_EXTENDED_UART_QUEUE)
			&& (datalen > 0)) {
		complex_tx_sequence[num_param_in_extended_uart_queue].bufflen = datalen;
		complex_tx_sequence[num_param_in_extended_uart_queue].ptr2buffhead =
				dataptr;
		complex_tx_sequence[num_param_in_extended_uart_queue].needtoreverse = reverseendianness;

		num_param_in_extended_uart_queue++;

		if(checksum != NULL){
			unsigned int i = 0;
			for(i = 0; i < datalen; i++)
				(*checksum) += (signed char)(*(dataptr + i));
		}
	}
}

void start_long_tx(){
	_uart_tx_operation_mode = QUEUED;
	_reset_tx_buffer();
	_clear_complex_tx_list();
}

unsigned char* GetUARTTxBufferPtr(unsigned int * len){
	(*len) = 0;
	switch(_uart_tx_operation_mode){
	case QUEUED:
		(*len) = UART_TX_BUFFER_LEN;
		return &_uart_tx_buffer[0];;
	default:
		return NULL;
	}
}

void _resetpotentialcommandstartingpointer(){
	_potential_command_start_ptr = NULL;
}

void SetPotentialCommandStartingPointer(unsigned char * ptr2next){
	_potential_command_start_ptr = ptr2next;
}

void _complex_UART_Tx(){
	if (transmit_out_queued != 0) {
		unsigned int i = 0;
		unsigned int j = 0;
		for(i = 0; i < num_param_in_extended_uart_queue; i++){
			/* Do check for endianness of the parameter */
			switch(complex_tx_sequence[i].needtoreverse){
			case REVERSE_ENDIANNESS:
				for(j = complex_tx_sequence[i].bufflen; j != 0; j--){
					_tx_char(*(complex_tx_sequence[i].ptr2buffhead + j - 1));
				}
				break;
			case LEAVE_ENDIANNESS:
			default:
				for(j = 0; j < complex_tx_sequence[i].bufflen; j++){
					_tx_char(*(complex_tx_sequence[i].ptr2buffhead + j));
				}
				break;
			}
		}

		transmit_out_queued = 0;
		_uart_tx_operation_mode = FIFO;
		_clear_complex_tx_list();
	}
}

void service_uart_UCA1_TX(){
	/* Clear out the Tx queue */
	switch (_uart_tx_operation_mode) {
	case FIFO:
	default:
		while (_last_sent_uart_tx_ptr != _p_uart_tx_ptr) {
#ifdef TX_NOTIFICATION_ON_PIN
			turn_debug_led_on();
#endif
			UARTTxNext();
#ifdef TX_NOTIFICATION_ON_PIN
			turn_debug_led_off();
#endif
		}
		break;
	case QUEUED:
		_complex_UART_Tx();
		break;
	}
	_reset_tx_buffer();
}

unsigned char * _get_potential_command_str_ptr(){
	return (unsigned char *) _potential_command_start_ptr;
}

void transmit_long_uart_message(){
	transmit_out_queued = 1;

	/* Force the output */
	_complex_UART_Tx();
}

void UARTServiceUCA1Rx(){
#ifdef RX_SERVICE_LED_DEBUG
	turn_debug_led_on();
#endif

	/* Guard against this being incremented by ISR when routine is running */
	unsigned char * _p_uart_rx_ptr_old = _p_uart_rx_ptr;

	static uint32_t cur_time_uart = 0;
	uart_command_process_status_t x = CMD_UNKNOWN;
	cur_time_uart = GetCurrentLongTick();

	/* Start to decompose the message */
	if (_last_scanned_uart_rx_ptr != _p_uart_rx_ptr_old) {
		/* Get the current tick time - this is only true if a character has
		 * been received */
		//		_last_uart_rx_time = GetCurrentLongTick();

		/* There's a string that may be able to be processed; try */
		if ((_possible_cmd == UNKNOWN_CMD)
				|| (_possible_cmd == POTENTIAL_CMD)) {
			/* Unknown command, try to find what possible command it can be */
			_possible_cmd = _try_link_command(_last_scanned_uart_rx_ptr,
					_p_uart_rx_ptr_old);
		}

		if (_possible_cmd == UNKNOWN_CMD) {
			/* Command is unknown AND not a potential command, reset */
			_reset_buffer_ptr_and_reset_uart_rx();
			//			_last_scanned_uart_rx_ptr = _p_uart_rx_ptr;
		} else {
			if (_possible_cmd != POTENTIAL_CMD) {
				/* We know what command we're probably at */
				x = _uart_commands[_possible_cmd].CmdCallbackPtr(_last_scanned_uart_rx_ptr, _p_uart_rx_ptr_old);
				switch (x) {
				case CMD_NOT_COMPLETE:
					/* Do nothing - still processing but index the "new" pointer */
					break;
				case CMD_SUCCESS:
					if(_potential_command_start_ptr != NULL){
						_last_scanned_uart_rx_ptr = _get_potential_command_str_ptr();

						_resetpotentialcommandstartingpointer();
					}
					else{
						//						_reset_buffer_ptr_and_reset_uart_rx();
						_last_scanned_uart_rx_ptr = _p_uart_rx_ptr_old;
					}
					_possible_cmd = UNKNOWN_CMD;
					break;
				case CMD_UNKNOWN:
				case CMD_SYNTAX_ERROR:
				case CMD_NOT_IMPLEMENTED:
				default:
					//					_reset_buffer_ptr_and_reset_uart_rx();
					_last_scanned_uart_rx_ptr = _p_uart_rx_ptr_old;
					_possible_cmd = UNKNOWN_CMD;
					break;
				}
			}
		}
	}
#ifdef RX_SERVICE_LED_DEBUG
	turn_debug_led_off();
#endif

	/* after UART_RX_TIMEOUT ticks reset the buffer pointers */
	if((_last_uart_rx_time + UART_RX_TIMEOUT) < cur_time_uart){
		_reset_buffer_ptr_and_reset_uart_rx();
	}
}

void UARTTxNext() {
	/* Bounds check the pointer */
	if ((_last_sent_uart_tx_ptr > &_uart_tx_buffer[UART_TX_BUFFER_LEN - 1])
			|| (_last_sent_uart_tx_ptr < &_uart_tx_buffer[0])) {
		/* Wrap the buffer */
		_last_sent_uart_tx_ptr = &_uart_tx_buffer[0];
	}

	/* Tx the character */
	_tx_char(*_last_sent_uart_tx_ptr);

	/* Increment the pointer */
	_last_sent_uart_tx_ptr++;
}

void _reset_buffer_ptr_and_reset_uart_rx() {
	/* Reset the pointer in the circular buffer */
	_last_scanned_uart_rx_ptr = &_uart_rx_buffer[0];
	_p_uart_rx_ptr = &_uart_rx_buffer[0];
	_uart_0_status = UART_IDLE;
	_possible_cmd = UNKNOWN_CMD;
}

unsigned char check_and_lower_rx_flag() {
	if ((_uart_0_status == UART_RECEIVING) || (send_adc_flag != 0)) {
		_uart_0_status = UART_IDLE;
		return UART_FLAG_NEW_AVAILABLE;
	} else
		return UART_FLAG_NOTHING_NEW;
}

void EnqueueCharToSendUART(char char2send) {
	if ((_p_uart_tx_ptr > &_uart_tx_buffer[UART_TX_BUFFER_LEN - 1])
			|| (_p_uart_tx_ptr < &_uart_tx_buffer[0])) {
		/* Wrap the buffer */
		_p_uart_tx_ptr = &_uart_tx_buffer[0];
	}
	*_p_uart_tx_ptr++ = char2send;
}

void EnqueueUCharToSendUART(unsigned char char2send) {
	if ((_p_uart_tx_ptr > &_uart_tx_buffer[UART_TX_BUFFER_LEN - 1])
			|| (_p_uart_tx_ptr < &_uart_tx_buffer[0])) {
		/* Wrap the buffer */
		_p_uart_tx_ptr = &_uart_tx_buffer[0];
	}
	*_p_uart_tx_ptr++ = char2send;
}

void EnqueueToSendUART(char* ptr2firstchar, unsigned int length) {
	int i = 0;
	for (i = 0; i < length; i++) {
		EnqueueCharToSendUART(ptr2firstchar[i]);
	}
}

/* Tx a null termianted string */
void EnqueueStringUART(char* str_to_tx) {
	int max_msg_len = UART_TX_BUFFER_LEN;
	unsigned int i = 0;
	while (i < max_msg_len) {
		if (str_to_tx[i] == 0x00)
			return;

		EnqueueCharToSendUART(str_to_tx[i]);
		i++;
	}
}

void EnqueueULongHexUART(unsigned long to_send) {
	int i = 0;
	for (i = 0; i < 4; i++) {
		EnqueueCharHexUART((char) ((to_send & 0xFF000000) >> 24));
		to_send <<= 8;
	}
}

void EnqueueUIntRawUART(unsigned int to_send){
	int i = 0;
	for (i = 0; i < 2; i++) {
		EnqueueCharToSendUART((char) ((to_send & 0xFF00) >> 8));
		to_send <<= 8;
	}
}

void EnqueueUIntHexUART(unsigned int to_send) {
	int i = 0;
	for (i = 0; i < 2; i++) {
		EnqueueCharHexUART((char) ((to_send & 0xFF00) >> 8));
		to_send <<= 8;
	}
}

/* Tx hex (1 byte) */
void EnqueueCharHexUART(char to_send) {
	unsigned int i = 0;
	unsigned char x = 0;
	for (i = 0; i < 2; i++) {
		x = (to_send & 0xF0);
		if (x > 0x90)
			EnqueueCharToSendUART(((x >> 4) - 10) + 0x41);
		else
			EnqueueCharToSendUART((x >> 4) + 0x30);

		to_send <<= 4;
	}
}

void SetADCEnqueueFlag() {
	send_adc_flag = 1;
}

