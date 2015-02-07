#ifndef __I2C_INTERFACE_H
#define __I2C_INTERFACE_H

#define I2C_ADDRESS_COPROC		0x23
#define I2C_ADDRESS_POT			0x59
#define SMBUS_ADDRESS_BQ24765	0x12

//#define SMBUS_FREQ_MAX
//#define I2C_FREQ_100_KHZ

#ifndef SMBUS_FREQ_MAX
#define I2C_FREQ_400_KHZ
//#define I2C_FREQ_200_KHZ
#endif

typedef enum i2c_mode {
		UNINIT_I2C,
		READ_I2C_SLAVE,		/* Reading from slave */
		WRITE_I2C_SLAVE,	/* Writing to slave */
		READ_I2C_REGISTER,
		READ_I2C_REGISTER_TO_BUFF,
		FREE_I2C_BUS,
		WRITE_I2C_NACKED,
}i2c_mode_t;

typedef enum i2c_register_read_state{
	NOT_ACTIVE = 0,
	TRANSMIT_TX_ADDR = 1,
	TRANSMIT_CMD = 2,
	TRANSMIT_RX_ADDR = 3,
	RECIEVE_DATA = 4
}i2c_register_read_state_t;


void init_i2c_ucb();

void simple_write_i2c_slave(unsigned char, int, char*);

void SimpleReadI2CSlave(unsigned char, int);

void read_i2c_slave_to_buff();

uint8_t i2c_rx_buff_has_stuff();

void disp_coproc_data();

void set_up_coproc_read(unsigned char);

void set_up_apple_read(unsigned char);

//void coproc_read_to_buff(unsigned char, unsigned char *, int *, int, queueableFunctionPointer);
void coproc_read_to_buff(unsigned char, unsigned char *, int *, queueableFunctionPointer);

void coproc_write(unsigned char, unsigned char *, queueableFunctionPointer);

void coproc_write_specific(unsigned char, unsigned char, unsigned char *, queueableFunctionPointer, unsigned char);




#endif



