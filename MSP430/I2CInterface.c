#include "Common.h"

#include "I2CInterface.h"

#include "LightSensor.h"

#define I2C_RX_BUFF_LEN	150
#define I2C_TX_BUFF_LEN	150

#define I2C_STOP_LINE_TIMEOUT	(5000)

volatile unsigned char *_p_i2c_rx_ptr;                     // Pointer to RX data
volatile unsigned char _cntr_i2c_rx_byte;
unsigned char _i2c_rx_buffer[I2C_RX_BUFF_LEN];       // Allocate 150 byte of RAM

unsigned char trash;

volatile unsigned char *_p_i2c_tx_ptr;
volatile unsigned char _cntr_i2c_tx_byte;
unsigned char _cntr_i2c_tx_len;
volatile unsigned char _i2c_tx_buffer[I2C_TX_BUFF_LEN]; // Allocate 150 byte of RAM

volatile unsigned char _call_TRx_Handler = 0;

volatile i2c_mode_t _i2c_state = UNINIT_I2C;
volatile uint8_t rx_buff_has_stuff;

volatile unsigned char _sl_coproc_addr;
static unsigned char _sl_data_len;
static unsigned char* _sl_data_ptr;
static unsigned char _sl_reg_value;

volatile uint8_t _i2c_reg_sent = 0;
volatile uint8_t _isr_went = 0;
volatile i2c_register_read_state_t _i2c_reg_read_state = NOT_ACTIVE;

static queueableFunctionPointer _TRx_Handler;

void _do_coproc_write();
void _write_coproc();



__inline void _quick_i2c_reset(){
	/* Put peripheral into reset */
	UCB1CTL1 |= UCSWRST;
	/* Clear the transmit starts, stops */
	UCB1CTL1 &= ~(UCTXSTT | UCTXSTP);
	/* Bring out of reset */
	UCB1CTL1 &= ~UCSWRST;
	/* Make sure NACK interrupt's enabled */
	UCB1IE |= UCNACKIE;
}

void __insure_free_i2c_bus(){
	int tries = 3;
	uint8_t need2reset = 1;
	int __i = I2C_STOP_LINE_TIMEOUT;

	for(tries = 3; tries > 0; tries--){
		if((UCB1STAT & UCSCLLOW) == UCSCLLOW){
			_quick_i2c_reset();
		}
		else
		{

		}
	}
}



void init_i2c_ucb() {
	if(_i2c_state == UNINIT_I2C){
		/* Set Pins 1 and 2 to SEL */
		P4SEL |= BIT1 | BIT2;

		/* Put UCB1 into SW reset */
		UCB1CTL1 |= UCSWRST;

		/* Set to I2C master mode */
		UCB1CTL0 = UCMST + UCMODE_3 + UCSYNC;	// I2C Master, synchronous mode
		UCB1CTL1 = UCSSEL_2 + UCSWRST;			// Use SMCLK (1 MHz), keep SW reset
#ifdef USE_4_MHZ_SMCLK

#ifdef I2C_FREQ_400_KHZ
		UCB1BR0 = 10;	// Target fSCL = 400 kHz = SMCLK (4.00 MHz) / 10
		UCB1BR1 = 0;
#elif defined(I2C_FREQ_200_KHZ)
		UCB1BR0 = 20;	// Target fSCL = 200 kHz = SMCLK (4.00 MHz) / 20
		UCB1BR1 = 0;
#elif defined(I2C_FREQ_100_KHZ)
		UCB1BR0 = 40;	// Target fSCL = 100 kHz = SMCLK (4.00 MHz) / 40
		UCB1BR1 = 0;
#endif
#else
		UCB1BR0 = 80;	// fSCL = 50 kHz
		UCB1BR1 = 0;
#endif

		UCB1I2CSA = 0x00;                         // Set slave address to 0x00 for
		// now until instructed to write or read from a slave
		UCB1CTL1 &= ~UCSWRST;                    // Clear SW reset, resume operation

		UCB1IE |= UCNACKIE; // enable the NACK interrupt

		/* Set the internal I2C state machine to free */
		_i2c_state = FREE_I2C_BUS;
		rx_buff_has_stuff = 0;

		_i2c_reg_read_state = NOT_ACTIVE;

		/* Init the buffers */
		unsigned int i = 0;
		for (i = 0; i < I2C_RX_BUFF_LEN; i++) {
			_i2c_rx_buffer[i] = 0x00;
		}

		for (i = 0; i < I2C_TX_BUFF_LEN; i++) {
			_i2c_tx_buffer[i] = 0x00;
		}

		_TRx_Handler = NULL;
		_call_TRx_Handler = 0;
	}
}

void SimpleReadI2CSlave(unsigned char slave_address, int data_len) {
	/* Set slave address */
	UCB1I2CSA_H = 0x00;
	UCB1I2CSA_L = slave_address;
	_p_i2c_rx_ptr = &_i2c_rx_buffer[0];    // Start of RX buffer
	_cntr_i2c_rx_byte = data_len;

	__insure_free_i2c_bus();

	/* Set receiver flag */
	UCB1CTL1 &= ~UCTR;

	_i2c_state = READ_I2C_SLAVE;

	UCB1IE |= UCRXIE;                // Enable RX interrupt

	UCB1CTL1 |= UCTXSTT;
}

void read_i2c_slave_to_buff() {

	__insure_free_i2c_bus();

	/* Set slave address */
	UCB1I2CSA_H = 0x00;
	UCB1I2CSA_L = _sl_coproc_addr;
	_p_i2c_rx_ptr = _sl_data_ptr;    // Start of RX buffer
	_cntr_i2c_rx_byte = _sl_data_len;

	/* Set receiver flag */
	UCB1CTL1 &= ~UCTR;

	UCB1IE |= UCRXIE;                // Enable RX interrupt

	UCB1CTL1 |= UCTXSTT;
}

void simple_write_i2c_slave(unsigned char slave_address, int data_len,
		char* data_ptr) {
	UCB1I2CSA_H = 0x00;
	UCB1I2CSA_L = slave_address;
	_p_i2c_tx_ptr = (unsigned char *) _i2c_tx_buffer; // Start of Tx buffer; reset the tx buffer
	_cntr_i2c_tx_len = data_len;
	_cntr_i2c_tx_byte = data_len;

	/* Load up the write pointer */
	unsigned int i = 0;
	for (i = 0; i < data_len; i++)
		_i2c_tx_buffer[i] = data_ptr[i];

	__insure_free_i2c_bus();
	UCB1IE |= UCTXIE;                // Enable TX interrupt

	if (_i2c_state == FREE_I2C_BUS)
		_i2c_state = WRITE_I2C_SLAVE;


	UCB1CTL1 |= UCTR + UCTXSTT;             // I2C TX, start condition
}

/* Typical authentication scheme: */
/*
 * D > A   C   | RequestAuthenticationCertificate
 * D   A > C   | Read Certificate Data Length
 * D   A < C   | Certificate Data Length
 * D   A > C   | Read Certificate Data
 * D   A < C   | Certificate Data
 * D < A   C   | Authentication certificate (certificate data)
 * D > A   C   | RequestAuthenticationChallengeResponse
 * D   A > C   | Write Challenge Length
 * D   A > C   | Write Challenge Data
 * D   A > C   | Write Authentication Control (PROC_CONTROL = 1)
 * D   A > C   | Read Status
 * D   A < C   | Status: (PROC_RESULTS = 1)
 * D   A > C   | Read Challenge Response Length
 * D   A < C   | Challenge Response Length
 * D   A > C   | Read Challenge Response Data
 * D   A < C   | Challenge Response Data
 * D < A   C   | AuthenticationResponse
 * D > A   C   | AuthenticationSucceeded
 *
 *
 * Failure:
 * D > A   C   | AuthenticationFailed
 */



void _do_coproc_read() {
	uint32_t delay_len = (SYS_TIMER_ONE_MSEC); // ticks (1/32768)
	switch (_i2c_reg_read_state) {
	case TRANSMIT_TX_ADDR:
		/* Queue up the tx of the address */
		simple_write_i2c_slave(_sl_coproc_addr, 1, (char *) &_sl_reg_value);
		break;
	case TRANSMIT_RX_ADDR:
		read_i2c_slave_to_buff();
		break;
	default:
		/* Do nothing */
		break;
	}
	if (_i2c_reg_read_state != NOT_ACTIVE)
		TryRegisterWithSysTimer(_do_coproc_read, delay_len);

	if(_call_TRx_Handler != 0)
	{
		_call_TRx_Handler = 0;
		_TRx_Handler();
	}
}

/* This function is what you should use to read the coprocessor */
void set_up_coproc_read(unsigned char reg_value) {
	if (_i2c_state == FREE_I2C_BUS) {
		unsigned int len = 0;
		_i2c_state = READ_I2C_REGISTER;
		_i2c_reg_read_state = TRANSMIT_TX_ADDR;
		_i2c_reg_sent = 0;


		if (len > 0) {
			_sl_coproc_addr = MAX_44008_ADDR >> 1;
			_sl_data_len = len;

			/* Clear out the buffer at the value */
			int i = 0;
			for(i = 0; i < len; i++)
				_i2c_rx_buffer[i] = 0x00;


			_sl_data_ptr = &_i2c_rx_buffer[0];
			_sl_reg_value = reg_value;

			TryRegisterWithSysTimer(_do_coproc_read, 1);
		}
	}
}

void _write_coproc(){
	__insure_free_i2c_bus();

	/* Set slave address */
	UCB1I2CSA_H = 0x00;
	UCB1I2CSA_L = _sl_coproc_addr;

	_i2c_state = WRITE_I2C_SLAVE;

	_cntr_i2c_tx_byte = _cntr_i2c_tx_len;
	_p_i2c_tx_ptr = &_i2c_tx_buffer[0];

	UCB1IE |= UCTXIE;                // Enable TX interrupt
	UCB1CTL1 |= UCTR + UCTXSTT;             // I2C TX, start condition

	TryRegisterWithSysTimer(_do_coproc_write, 1);
}

void _do_coproc_write(){
	if(_i2c_state == FREE_I2C_BUS){
		if(_call_TRx_Handler != 0)
			_TRx_Handler();

		_call_TRx_Handler = 0;
	}
	else{
		if(_i2c_state == WRITE_I2C_NACKED)
			TryRegisterWithSysTimer(_write_coproc, SYS_TIMER_ONE_MSEC);
//		TryRegisterWithSysTimer(_do_coproc_write, (5 * SYS_TIMER_ONE_MSEC));
	}
}

/* TODO combine this with the normal write routine */
void coproc_write(unsigned char reg_value, unsigned char * buffptr,
		queueableFunctionPointer eventhandler){
	if(_i2c_state == FREE_I2C_BUS){
		uint16_t bufflen = 1;

		if(bufflen == 0)
			return;

		_TRx_Handler = eventhandler;
		_call_TRx_Handler = 0;

		_sl_coproc_addr = MAX_44008_ADDR >> 1;

		_p_i2c_tx_ptr = &_i2c_tx_buffer[0]; // Start of Tx buffer; reset the tx buffer
		_cntr_i2c_tx_len = bufflen + 1;

		_i2c_tx_buffer[0] = reg_value;

		/* Load up the write pointer */
		unsigned int i = 0;
		for (i = 0; i < bufflen; i++)
			_i2c_tx_buffer[i + 1] = *(buffptr + i);

		TryRegisterWithSysTimer(_write_coproc, SYS_TIMER_ONE_MSEC);
	}
}

/* TODO combine this with the normal write routine */
void coproc_write_specific(unsigned char addr, unsigned char reg_value, unsigned char * buffptr,
		queueableFunctionPointer eventhandler, unsigned char bufflen){
	if(_i2c_state == FREE_I2C_BUS){
		uint16_t bufflen = 0;

		bufflen = GetAuthRegLength(reg_value);

		if(bufflen == 0)
			return;

		_TRx_Handler = eventhandler;
		_call_TRx_Handler = 0;

		_sl_coproc_addr = addr;

		_p_i2c_tx_ptr = &_i2c_tx_buffer[0]; // Start of Tx buffer; reset the tx buffer
		_cntr_i2c_tx_len = bufflen + 1;

		_i2c_tx_buffer[0] = reg_value;

		/* Load up the write pointer */
		unsigned int i = 0;
		for (i = 0; i < bufflen; i++)
			_i2c_tx_buffer[i + 1] = *(buffptr + i);

		TryRegisterWithSysTimer(_write_coproc, SYS_TIMER_ONE_MSEC);
	}
}

void coproc_read_to_buff(unsigned char reg_value, unsigned char * buffptr,
		//		int * lenptr, int max_buff_len, queueableFunctionPointer eventhandler) {
		int * lenptr, queueableFunctionPointer eventhandler) {
	if (_i2c_state == FREE_I2C_BUS) {
		unsigned int len = 1;

		if(lenptr != NULL)
			*lenptr = len;

		_i2c_state = READ_I2C_REGISTER_TO_BUFF;
		_i2c_reg_read_state = TRANSMIT_TX_ADDR;
		_i2c_reg_sent = 0;

		_TRx_Handler = eventhandler;
		_call_TRx_Handler = 0;

		if (len > 0) {
			_sl_coproc_addr = MAX_44008_ADDR >> 1;
			_sl_data_len = len;
			_sl_data_ptr = buffptr;
			_sl_reg_value = reg_value;

			int i = 0;
			for(i = 0; i < len; i++)
				*(_sl_data_ptr + i) = 0x00;

			TryRegisterWithSysTimer(_do_coproc_read, 1);
		}
	}
}

#pragma vector = USCI_B1_VECTOR
__interrupt void USCI_B1_ISR(void) {
	switch (__even_in_range(UCB1IV, 12)) {
	case 0:
		break;                           // Vector  0: No interrupts
	case 2:
		break;                           // Vector  2: ALIFG
	case 4:                                  // Vector  4: NACKIFG
	{
		/* Nack - wait */
		switch (_i2c_state) {
		case WRITE_I2C_SLAVE:
			_i2c_state = WRITE_I2C_NACKED;
			break;
		case READ_I2C_REGISTER:
		case READ_I2C_REGISTER_TO_BUFF:
			switch(_i2c_reg_read_state){
			case TRANSMIT_TX_ADDR:
				UCB1CTL1 |= UCTR + UCTXSTT;             // I2C TX, start condition
				break;
			case TRANSMIT_RX_ADDR:
				UCB1CTL1 &= ~UCTR;
				UCB1CTL1 |= UCTXSTT;             // I2C TX, start condition
				break;
			case TRANSMIT_CMD:
				_i2c_reg_read_state = TRANSMIT_TX_ADDR;
				break;
			case RECIEVE_DATA:
				_i2c_reg_read_state = TRANSMIT_RX_ADDR;
				break;
			default:
				break;
			}
			//			if(_i2c_reg_read_state != NOT_ACTIVE)
			//				_i2c_reg_read_state--;
			break;
			default:
				break;
		}
	}
	break;
	case 6:
		break;                           // Vector  6: STTIFG
	case 8:
		break;                           // Vector  8: STPIFG
	case 10:                                  // Vector 10: RXIFG
	{
		if (_i2c_reg_read_state == TRANSMIT_RX_ADDR){
			_i2c_reg_read_state = RECIEVE_DATA;
		}
		*_p_i2c_rx_ptr = UCB1RXBUF;   // Move final RX data to PRxData
		if (_cntr_i2c_rx_byte <= 1) {
			UCB1CTL1 |= UCTXSTP;              // Generate I2C stop condition
			UCB1IE &= ~UCRXIE;
			rx_buff_has_stuff = 1;

			_i2c_reg_read_state = NOT_ACTIVE;

			if (_i2c_state == READ_I2C_REGISTER_TO_BUFF){
				_call_TRx_Handler = 1;
			}

			_i2c_state = FREE_I2C_BUS;
		}
		else{
			_p_i2c_rx_ptr++;
			_cntr_i2c_rx_byte--;                  // Decrement RX byte counter
		}
	}
	break;
	case 12:                                  // Vector 12: TXIFG
	{
		if ((_i2c_state == READ_I2C_REGISTER)
				|| (_i2c_state == READ_I2C_REGISTER_TO_BUFF)) {
			switch (_i2c_reg_read_state) {
			case TRANSMIT_TX_ADDR:
				UCB1TXBUF = _sl_reg_value;
				break;
			case TRANSMIT_CMD:
				UCB1CTL1 |= UCTXSTP;
				UCB1IE &= ~UCTXIE;
				break;
			default:
				break;
			}
			_i2c_reg_read_state++;
		} else {
			if (_cntr_i2c_tx_byte > 0)                      // Check TX byte counter
			{
				UCB1TXBUF = *_p_i2c_tx_ptr++;               // Load TX buffer
				_cntr_i2c_tx_byte--;                // Decrement TX byte counter
			} else {
				UCB1CTL1 |= UCTXSTP;                  // I2C stop condition
				UCB1IFG &= ~UCTXIFG;                // Clear USCI_B0 TX int flag
				UCB1IE &= ~UCTXIE;
				_call_TRx_Handler = 1;
				_i2c_state = FREE_I2C_BUS;
			}
		}
	}
	break;
	default:
		break;
	}
}
