/* Copyright (c) 2009 Nordic Semiconductor. All Rights Reserved.
*
* The information contained herein is property of Nordic Semiconductor ASA.
* Terms and conditions of usage are described in detail in NORDIC
* SEMICONDUCTOR STANDARD SOFTWARE LICENSE AGREEMENT.
*
* Licensees are granted free, non-transferable use of the information. NO
* WARRANTY of ANY KIND is provided. This heading must NOT be removed from
* the file.
*
*/

/** @file
*
* @defgroup blinky_example_main main.c
* @{
* @ingroup blinky_example
* @brief Blinky Example Application main file.
*
*/

#include <stdbool.h>
#include <stdint.h>
#include "nrf_delay.h"
#include "nrf_gpio.h"
#include "common.h"
#include "app_error.h"
#include "app_util_platform.h"
#include "boards.h"

#include "ant_interface.h"
#include "ant_parameters.h"
#include "nrf_soc.h"
#include "nrf_sdm.h"

#include "spi_master.h"
#include "slld_hal.h"
#include "slld.h"
#include "FlashApp.h"

/**************************************************************************** /

// Channel configuration. 
/* < ANT Channel 0. */
#define CHANNEL_0                       0x00
/* < Channel period 4 Hz. */
#define CHANNEL_0_TX_CHANNEL_PERIOD     8192u
/* < ANT Ext Assign. */
#define CHANNEL_0_ANT_EXT_ASSIGN        0x00                 

// Channel ID configuration. 
/* < Device type. */
#define CHANNEL_0_CHAN_ID_DEV_TYPE      0x02u                
/* < Device number. */
#define CHANNEL_0_CHAN_ID_DEV_NUM       0x02u                
/* < Transmission type. */
#define CHANNEL_0_CHAN_ID_TRANS_TYPE    0x01u                

// Miscellaneous defines. 
/* < ANT Channel Network. */
#define ANT_CHANNEL_DEFAULT_NETWORK     0x00                 
/* < Minimum size of ANT event message buffer. */
#define ANT_EVENT_MSG_BUFFER_MIN_SIZE   32u                  
/* < Size of the broadcast data buffer. */
#define BROADCAST_DATA_BUFFER_SIZE      8u                   

// Static variables and buffers. 
static uint8_t m_broadcast_data[BROADCAST_DATA_BUFFER_SIZE]; /* < Primary data transmit buffer. */
static uint8_t m_counter = 1u;                               /* < Counter to increment the ANT broadcast data payload. */


/**@brief Function for setting up the ANT module to be ready for TX broadcast.
 *
 * The following commands are issued in this order:
 * - assign channel 
 * - set channel ID 
 * - open channel 
 */
static void ant_channel_tx_broadcast_setup(void)
{
    uint32_t err_code;
    
    // Set Channel Number. 
    err_code = sd_ant_channel_assign(CHANNEL_0, 
                                     CHANNEL_TYPE_MASTER_TX_ONLY, 
                                     ANT_CHANNEL_DEFAULT_NETWORK, 
                                     CHANNEL_0_ANT_EXT_ASSIGN);
    
    APP_ERROR_CHECK(err_code);

    // Set Channel ID. 
    err_code = sd_ant_channel_id_set(CHANNEL_0, 
                                     CHANNEL_0_CHAN_ID_DEV_NUM, 
                                     CHANNEL_0_CHAN_ID_DEV_TYPE, 
                                     CHANNEL_0_CHAN_ID_TRANS_TYPE);
    APP_ERROR_CHECK(err_code);

    // Open channel. 
    err_code = sd_ant_channel_open(CHANNEL_0);
    APP_ERROR_CHECK(err_code);
}


/**@brief Function for handling ANT TX channel events. 
 *
 * @param[in] event The received ANT event to handle.
 */
static void channel_event_handle(uint32_t event)
{
    uint32_t err_code;
    
    switch (event)
    {
        // ANT broadcast success.
        // Send a new broadcast and increment the counter.
        case EVENT_TX:
            
            // Assign a new value to the broadcast data. 
            m_broadcast_data[BROADCAST_DATA_BUFFER_SIZE - 1] = m_counter;
            
            // Broadcast the data. 
            err_code = sd_ant_broadcast_message_tx(CHANNEL_0, 
                                                   BROADCAST_DATA_BUFFER_SIZE, 
                                                   m_broadcast_data);
            APP_ERROR_CHECK(err_code);
            
            // Increment the counter.
            m_counter++;
            
            // Activate LED_0 for 20 ms. 
            nrf_gpio_pin_set(LED_0);
            nrf_delay_ms(20);
            nrf_gpio_pin_clear(LED_0);
            break;
            
        default:
            break;
    }
}


/**@brief Function for stack interrupt handling.
 *
 * Implemented to clear the pending flag when receiving 
 * an interrupt from the stack.
 */
void SD_EVT_IRQHandler(void)
{

}


/**@brief Function for handling SoftDevice asserts. 
 *
 * @param[in] pc          Value of the program counter.
 * @param[in] line_num    Line number where the assert occurred.
 * @param[in] p_file_name Pointer to the file name. 
 */
void softdevice_assert_callback(uint32_t pc, uint16_t line_num, const uint8_t * p_file_name)
{
    for (;;)
    {
        // No implementation needed. 
    }
}


/**@brief Function for handling HardFault.
 */
void HardFault_Handler(void)
{
    for (;;)
    {
        // No implementation needed. 
    }
}


/*****************************************************************************/






void init_LEDs(){
	// Configure LED-pins as outputs.
	nrf_gpio_cfg_output(LED_RED);
	nrf_gpio_cfg_output(LED_GREEN);
	nrf_gpio_cfg_output(LED_YELLOW);
	nrf_gpio_cfg_output(LED_ORANGE);

	LED_TURN_OFF(LED_RED);
	LED_TURN_OFF(LED_GREEN);
	LED_TURN_OFF(LED_YELLOW);
	LED_TURN_OFF(LED_ORANGE);

}



/**
* @brief Function for application main entry.
*/
int main(void)
{
	init_LEDs();
	
//	spi_EEPROM_init();
//	
//	init_flash_app();

	
    // ANT event message buffer. 
    static uint8_t event_message_buffer[ANT_EVENT_MSG_BUFFER_MIN_SIZE];
    
    // Configure pins LED_0 and LED_1 as outputs. 
    nrf_gpio_range_cfg_output(LED_START, LED_STOP);

    // Set LED_0 and LED_1 high to indicate that the application is running. 
    nrf_gpio_pin_set(LED_0);
    nrf_gpio_pin_set(LED_1);
    
    // Enable SoftDevice. 
    uint32_t err_code;
    err_code = sd_softdevice_enable(NRF_CLOCK_LFCLKSRC_XTAL_50_PPM, softdevice_assert_callback);
    APP_ERROR_CHECK(err_code);

    // Set application IRQ to lowest priority. 
    err_code = sd_nvic_SetPriority(SD_EVT_IRQn, NRF_APP_PRIORITY_LOW); 
    APP_ERROR_CHECK(err_code);
  
    // Enable application IRQ (triggered from protocol). 
    err_code = sd_nvic_EnableIRQ(SD_EVT_IRQn);
    APP_ERROR_CHECK(err_code);

    // Setup Channel_0 as a TX Master Only. 
    ant_channel_tx_broadcast_setup();
    
    // Write counter value to last byte of the broadcast data.
    // The last byte is chosen to get the data more visible in the end of an printout
    // on the recieving end. 
    m_broadcast_data[BROADCAST_DATA_BUFFER_SIZE - 1] = m_counter;
  
    // Initiate the broadcast loop by sending a packet on air, 
    // then start waiting for an event on this broadcast message.
    err_code = sd_ant_broadcast_message_tx(CHANNEL_0, BROADCAST_DATA_BUFFER_SIZE, m_broadcast_data);
    APP_ERROR_CHECK(err_code);
  
    // Set LED_0 and LED_1 low to indicate that stack is enabled. 
    nrf_gpio_pin_clear(LED_0);
    nrf_gpio_pin_clear(LED_1);
    
    uint8_t event;
    uint8_t ant_channel;
  
    // Main loop. 
    for (;;)
    {   
        // Light up LED_1 to indicate that CPU is going to sleep. 
        nrf_gpio_pin_set(LED_1);
        
        // Put CPU in sleep if possible. 
        err_code = sd_app_evt_wait();
        APP_ERROR_CHECK(err_code);
        
        // Turn off LED_1 to indicate that CPU is going out of sleep. 
        nrf_gpio_pin_clear(LED_1);
    
        // Extract and process all pending ANT events as long as there are any left. 
        do
        {
            // Fetch the event. 
            err_code = sd_ant_event_get(&ant_channel, &event, event_message_buffer);
            if (err_code == NRF_SUCCESS)
            {
                // Handle event. 
                switch (event)
                {
                    case EVENT_TX:
                        channel_event_handle(event);
                        break;
                        
                    default:
                        break;
                }
            }          
        } 
        while (err_code == NRF_SUCCESS);
    }
}
/** @} */
