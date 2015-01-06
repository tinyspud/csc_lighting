#include <stdbool.h>
#include <stdint.h>
#include "app_error.h"
#include "nrf.h"
#include "ant_interface.h"
#include "ant_parameters.h"
#include "nrf_soc.h"
#include "nrf_sdm.h"
#include "app_timer.h"
#include "nrf_delay.h"
#include "nrf_temp.h"
#include "nordic_common.h"
#include "bsp.h"
#include "nrf_temp.h"

#include "LEDs.h"
#include "slld_hal.h"
#include "FlashApp.h"
#include "S210_LL.h"
#include "system_error_callbacks.h"



#define APP_TIMER_PRESCALER      0                     /**< Value of the RTC1 PRESCALER register. */
#define APP_TIMER_MAX_TIMERS     BSP_APP_TIMERS_NUMBER /**< Maximum number of simultaneously created timers. */
#define APP_TIMER_OP_QUEUE_SIZE  2                     /**< Size of timer operation queues. */

// Channel configuration. 
#define CHANNEL_0                       0x00                 /**< ANT Channel 0. */
#define CHANNEL_0_ANT_EXT_ASSIGN        0x00                 /**< ANT Ext Assign. */

#define CHANNEL_0_PERIOD				16384				/* Channel period (n/32768 s period) */	

// Channel ID configuration. 
#define CHANNEL_0_CHAN_ID_DEV_TYPE      0x02u                /**< Device type. */
#define CHANNEL_0_CHAN_ID_DEV_NUM       0x02u                /**< Device number. */
#define CHANNEL_0_CHAN_ID_TRANS_TYPE    0x01u                /**< Transmission type. */

// Miscellaneous defines. 
#define ANT_CHANNEL_DEFAULT_NETWORK     0x00                 /**< ANT Channel Network. */
#define ANT_EVENT_MSG_BUFFER_MIN_SIZE   32u                  /**< Minimum size of ANT event message buffer. */
#define BROADCAST_DATA_BUFFER_SIZE      8u                   /**< Size of the broadcast data buffer. */

// Static variables and buffers. 
static uint8_t m_broadcast_data[BROADCAST_DATA_BUFFER_SIZE]; /**< Primary data transmit buffer. */
static uint8_t m_counter = 1u;                               /**< Counter to increment the ANT broadcast data payload. */
// ANT event message buffer. 
static uint8_t event_message_buffer[ANT_EVENT_MSG_BUFFER_MIN_SIZE];

/**@brief Function for handling ANT TX channel events.
 *
 * @param[in] event The received ANT event to handle.
 */
static void channel_event_handle(uint32_t event)
{
	uint32_t err_code;
	int32_t temp = 0xFFFFFFFF;

	switch (event)
	{
		// ANT broadcast success.
		// Send a new broadcast and increment the counter.
	case EVENT_TX:
		
	/* Busy wait while temperature measurement is not finished, you can skip waiting if you enable interrupt for DATARDY event and read the result in the interrupt. */
	/*lint -e{845} // A zero has been given as right argument to operator '|'" */
//	while (NRF_TEMP->EVENTS_DATARDY == 0)
//	{
//		// Do nothing.
//	}
	if(NRF_TEMP->EVENTS_DATARDY == 1){
		NRF_TEMP->EVENTS_DATARDY = 0;

		/**@note Workaround for PAN_028 rev2.0A anomaly 29 - TEMP: Stop task clears the TEMP register. */
		temp = (nrf_temp_read() / 4);

		/**@note Workaround for PAN_028 rev2.0A anomaly 30 - TEMP: Temp module analog front end does not power down when DATARDY event occurs. */
		NRF_TEMP->TASKS_STOP = 1; /** Stop the temperature measurement. */
	}
	NRF_TEMP->TASKS_START = 1; /** Start the temperature measurement. */

		// Assign a new value to the broadcast data. 
		m_broadcast_data[BROADCAST_DATA_BUFFER_SIZE - 1] = m_counter;
		m_broadcast_data[0] = (uint8_t)temp;
	
		// Broadcast the data. 
		err_code = sd_ant_broadcast_message_tx(CHANNEL_0,
			BROADCAST_DATA_BUFFER_SIZE,
			m_broadcast_data);
		APP_ERROR_CHECK(err_code);

		// Increment the counter.
		m_counter++;

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

	/* Set channel period */
	err_code = sd_ant_channel_period_set(CHANNEL_0, CHANNEL_0_PERIOD);
	APP_ERROR_CHECK(err_code);
	
	// Open channel. 
	err_code = sd_ant_channel_open(CHANNEL_0);
	APP_ERROR_CHECK(err_code);
}

uint32_t handle_ANT_events(){
	uint8_t event;
	uint8_t ant_channel;
	uint32_t err_code = 0;
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
	return err_code;
}

static uint8_t capabilities_buff[8];
void get_chip_capabilities(){
	uint32_t capabilities_rtn = 0;
	
	
	capabilities_rtn = sd_ant_capabilities_get(&capabilities_buff[0]);

	/* https://devzone.nordicsemi.com/documentation/nrf51/4.2.0/html/group__ant__interface.html#gaa8c0f97d3a06806ecbb5259d34803ec8 */
	/* 08 08 00 B2 36 00 F5 01 */
	
	/* Look at the capabilities buffer */
	if(capabilities_rtn == NRF_SUCCESS){
		if((CAPABILITIES_ADVANCED_BURST_ENABLED & capabilities_buff[3]) == CAPABILITIES_ADVANCED_BURST_ENABLED){
			/*  */
		}
	}
}

void init_S210_LL(){
	// Enable SoftDevice. 
	uint32_t err_code;
	err_code = sd_softdevice_enable(NRF_CLOCK_LFCLKSRC_XTAL_50_PPM, softdevice_assert_callback);
	APP_ERROR_CHECK(err_code);

	APP_TIMER_INIT(APP_TIMER_PRESCALER, APP_TIMER_MAX_TIMERS, APP_TIMER_OP_QUEUE_SIZE, false);

	err_code = bsp_init(BSP_INIT_LED, APP_TIMER_TICKS(100, APP_TIMER_PRESCALER), NULL);
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
	m_broadcast_data[0] = 0x00;
	m_broadcast_data[BROADCAST_DATA_BUFFER_SIZE - 1] = m_counter;

	// Initiate the broadcast loop by sending a packet on air, 
	// then start waiting for an event on this broadcast message.
	err_code = sd_ant_broadcast_message_tx(CHANNEL_0, BROADCAST_DATA_BUFFER_SIZE, m_broadcast_data);
	APP_ERROR_CHECK(err_code);

	get_chip_capabilities();

}



