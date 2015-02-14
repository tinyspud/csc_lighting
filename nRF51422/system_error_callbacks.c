#include <stdbool.h>
#include <stdint.h>
#include "custom_board.h"

#include "app_error.h"
#include "app_timer.h"
#include "nrf_delay.h"

#include "LEDs.h"
#include "system_error_callbacks.h"


void shift_out_error_and_line_num(uint32_t error_code, uint32_t line_num){
	int i = 0;
	
	/* Turn off the LEDs */
	LED_TURN_OFF(LED_RED);
	LED_TURN_OFF(LED_GREEN);
	LED_TURN_OFF(LED_ORANGE);
	LED_TURN_OFF(LED_YELLOW);

	for (;;){
		/* Flash out the error code on the red LED, line number on orange */
		LED_TURN_OFF(LED_YELLOW);
		for(i = 0; i < 32; i++){
			LED_TURN_OFF(LED_YELLOW);
			(((error_code << i) & 0x80000000) == 0x80000000) ? LED_TURN_ON(LED_RED) : LED_TURN_OFF(LED_RED);
			(((line_num << i) & 0x80000000) == 0x80000000) ? LED_TURN_ON(LED_ORANGE) : LED_TURN_OFF(LED_ORANGE);
			nrf_delay_ms(115);
			LED_TURN_ON(LED_YELLOW);
			nrf_delay_ms(10);
		}
		LED_TURN_ON(LED_RED);
		LED_TURN_ON(LED_YELLOW);
		LED_TURN_ON(LED_ORANGE);
		nrf_delay_ms(500);
	}

}
/**@brief Function for error handling, which is called when an error has occurred.
*
* @param[in] error_code	Error code supplied to the handler.
* @param[in] line_num		Line number where the handler is called.
* @param[in] p_file_name Pointer to the file name.
*/
void app_error_handler(uint32_t error_code, uint32_t line_num, const uint8_t * p_file_name)
{
	shift_out_error_and_line_num(error_code, line_num);
}

/**@brief Function for handling SoftDevice asserts.
 *
 * @param[in] pc          Value of the program counter.
 * @param[in] line_num    Line number where the assert occurred.
 * @param[in] p_file_name Pointer to the file name.
 */
void softdevice_assert_callback(uint32_t pc, uint16_t line_num, const uint8_t * p_file_name)
{
	shift_out_error_and_line_num(pc, line_num);
}


/**@brief Function for handling HardFault.
 */
void HardFault_Handler(void)
{
	shift_out_error_and_line_num(0, 0);
}


