#ifndef __SYSTEM_ERROR_CALLBACKS_H
#define __SYSTEM_ERROR_CALLBACKS_H

#include <stdbool.h>
#include <stdint.h>

/**@brief Function for error handling, which is called when an error has occurred.
*
* @param[in] error_code	Error code supplied to the handler.
* @param[in] line_num		Line number where the handler is called.
* @param[in] p_file_name Pointer to the file name.
*/
void app_error_handler(uint32_t error_code, uint32_t line_num, const uint8_t * p_file_name);
/**@brief Function for handling SoftDevice asserts.
 *
 * @param[in] pc          Value of the program counter.
 * @param[in] line_num    Line number where the assert occurred.
 * @param[in] p_file_name Pointer to the file name.
 */
void softdevice_assert_callback(uint32_t pc, uint16_t line_num, const uint8_t * p_file_name);

/**@brief Function for handling HardFault.
 */
void HardFault_Handler(void);



#endif
