
#ifndef __UART_PROTOCOL_H
#define __UART_PROTOCOL_H


typedef enum uart_command_process_status{
	COMMAND_SUCCESS = 1,
	COMMAND_NOT_COMPLETE = 2,
	COMMAND_UNKNOWN = 3,
	COMMAND_SYNTAX_ERROR = 4,
	COMMAND_NOT_IMPLEMENTED = 5,
}uart_command_process_status_t;


/* Return values */
#define CMD_SUCCESS			COMMAND_SUCCESS
#define CMD_NOT_COMPLETE	COMMAND_NOT_COMPLETE
#define CMD_UNKNOWN			COMMAND_UNKNOWN
#define CMD_SYNTAX_ERROR	COMMAND_SYNTAX_ERROR
#define CMD_NOT_IMPLEMENTED	COMMAND_NOT_IMPLEMENTED

/* Command execution prototype */
typedef uart_command_process_status_t (*cmd_callback)(unsigned char*, unsigned char*);


typedef struct cmd_struct{
	const char*		CmdHeader;
	unsigned int	CmdHeaderLen;
	unsigned int	CmdMinLength;
	unsigned int	CmdMaxLength;
	cmd_callback	CmdCallbackPtr;
}cmd_struct_t;

/* Servo commands */
#define UART_SERVO_CMD_LEN	1
#define UART_SERVO_LEN		3
/* Move to ABS position command (byte 1 = position, byte 2 = don't care) */
#define UART_SERVO_ABS_CMD	'A'
/* Move in sinusoid command (byte 1 = amplitude (degrees/2), byte 2 = period (seconds)) */
#define UART_SERVO_SIN_CMD	'O'


/* Thermistor commands */
#define UART_THERM_CMD_LEN	1
#define UART_THERM_LEN		2
/* Byte 0 is 0/1 (off/on)
 * Byte 1 is R val (2.5k / 256)
 *  */

/* LEDx commands */
#define UART_LED_CMD_LEN	1
#define UART_LED_LEN		3
/* Byte 0: LED number (0 is broadcast or all [depending on command])
 * Byte 1: LED command:
 * 			0x01	Set power (byte2 / 255)
 * 			0x02	Strobe (byte 2 = which LEDs)
 *
 * FOR NOW
 * LED is set by byte 1
 *
 * */

/* Define the  */






#endif





