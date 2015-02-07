
 /*
 Modified by C Chock Nov 28, '14 for penetrometer project
 under contract from Beaumont
 */

#ifndef __BOARD_ANT_IO_V_5
#define __BOARD_ANT_IO_V_5

#include "nrf_gpio.h"

#define LED_0          8
#define LED_1          3
#define LED_2          15
#define LED_3          30
#define LED_START      LED_0
#define LED_STOP       LED_1

#define LED_RED				LED_0
#define LED_GREEN			LED_1
#define LED_YELLOW		LED_2
#define LED_ORANGE		LED_3

#define LED_TURN_ON(x)	nrf_gpio_pin_clear(x)
#define LED_TURN_OFF(x)	nrf_gpio_pin_set(x)

#define BUTTON_0       23
#define BUTTON_1       2
#define BUTTON_2       12
#define BUTTON_3       11
#define BUTTON_START   BUTTON_0
#define BUTTON_STOP    BUTTON_1
#define BUTTON_PULL    NRF_GPIO_PIN_NOPULL

#define RX_PIN_NUMBER  16    // UART RX pin number.
#define TX_PIN_NUMBER  17    // UART TX pin number.
#define CTS_PIN_NUMBER 18    // UART Clear To Send pin number. Not used if HWFC is set to false. 
#define RTS_PIN_NUMBER 19    // UART Request To Send pin number. Not used if HWFC is set to false. 
#define HWFC           false // UART hardware flow control.

#define SPIS_MISO_PIN  20    // SPI MISO signal. 
#define SPIS_CSN_PIN   21    // SPI CSN signal. 
#define SPIS_MOSI_PIN  22    // SPI MOSI signal. 
#define SPIS_SCK_PIN   23    // SPI SCK signal. 

#define SPIM0_SCK_PIN       11u     /**< SPI clock GPIO pin number. */
#define SPIM0_MOSI_PIN      24u     /**< SPI Master Out Slave In GPIO pin number. */
#define SPIM0_MISO_PIN      23u     /**< SPI Master In Slave Out GPIO pin number. */
#define SPIM0_SS_PIN        5u     /**< SPI Slave Select GPIO pin number. */

#define EEPROM_CS_PIN			SPIM0_SS_PIN
#define EEPROM_CLK_PIN		SPIM0_SCK_PIN
#define EEPROM_MOSI_PIN		SPIM0_MOSI_PIN
#define EEPROM_MISO_PIN		SPIM0_MISO_PIN
#define EEPROM_WP_PIN			9U
#define EEPROM_RST_PIN		6U


#define SPIM1_SCK_PIN       16u     /**< SPI clock GPIO pin number. */
#define SPIM1_MOSI_PIN      18u     /**< SPI Master Out Slave In GPIO pin number. */
#define SPIM1_MISO_PIN      17u     /**< SPI Master In Slave Out GPIO pin number. */
#define SPIM1_SS_PIN        19u     /**< SPI Slave Select GPIO pin number. */

// serialization APPLICATION board
#define SER_APP_RX_PIN              16    // UART RX pin number.
#define SER_APP_TX_PIN              17    // UART TX pin number.
#define SER_APP_CTS_PIN             18    // UART Clear To Send pin number.
#define SER_APP_RTS_PIN             19    // UART Request To Send pin number.

#if 0
#define SER_APP_SPIM0_SCK_PIN       20     // SPI clock GPIO pin number.
#define SER_APP_SPIM0_MOSI_PIN      17     // SPI Master Out Slave In GPIO pin number
#define SER_APP_SPIM0_MISO_PIN      16     // SPI Master In Slave Out GPIO pin number
#define SER_APP_SPIM0_SS_PIN        21     // SPI Slave Select GPIO pin number
#define SER_APP_SPIM0_RDY_PIN       19     // SPI READY GPIO pin number
#define SER_APP_SPIM0_REQ_PIN       18     // SPI REQUEST GPIO pin number
#else
#define SER_APP_SPIM0_SCK_PIN       23     // SPI clock GPIO pin number.
#define SER_APP_SPIM0_MOSI_PIN      20     // SPI Master Out Slave In GPIO pin number
#define SER_APP_SPIM0_MISO_PIN      22     // SPI Master In Slave Out GPIO pin number
#define SER_APP_SPIM0_SS_PIN        21     // SPI Slave Select GPIO pin number
#define SER_APP_SPIM0_RDY_PIN       29     // SPI READY GPIO pin number
#define SER_APP_SPIM0_REQ_PIN       28     // SPI REQUEST GPIO pin number

#endif

// serialization CONNECTIVITY board
#if 0
#define SER_CON_RX_PIN              17    // UART RX pin number.
#define SER_CON_TX_PIN              16    // UART TX pin number.
#define SER_CON_CTS_PIN             19    // UART Clear To Send pin number. Not used if HWFC is set to false.
#define SER_CON_RTS_PIN             18    // UART Request To Send pin number. Not used if HWFC is set to false.
#else
#define SER_CON_RX_PIN              16    // UART RX pin number.
#define SER_CON_TX_PIN              17    // UART TX pin number.
#define SER_CON_CTS_PIN             18    // UART Clear To Send pin number. Not used if HWFC is set to false.
#define SER_CON_RTS_PIN             19    // UART Request To Send pin number. Not used if HWFC is set to false.
#endif

#if 0
#define SER_CON_SPIS_SCK_PIN        20    // SPI SCK signal.
#define SER_CON_SPIS_MISO_PIN       16    // SPI MISO signal.
#define SER_CON_SPIS_MOSI_PIN       17    // SPI MOSI signal.
#define SER_CON_SPIS_CSN_PIN        21    // SPI CSN signal.
#define SER_CON_SPIS_RDY_PIN        19     // SPI READY GPIO pin number.
#define SER_CON_SPIS_REQ_PIN        18     // SPI REQUEST GPIO pin number.
#else
#define SER_CON_SPIS_SCK_PIN        23    // SPI SCK signal.
#define SER_CON_SPIS_MOSI_PIN       22    // SPI MOSI signal.
#define SER_CON_SPIS_MISO_PIN       20    // SPI MISO signal.
#define SER_CON_SPIS_CSN_PIN        21    // SPI CSN signal.
#define SER_CON_SPIS_RDY_PIN        29     // SPI READY GPIO pin number.
#define SER_CON_SPIS_REQ_PIN        28     // SPI REQUEST GPIO pin number.
#endif

#define SER_CONN_ASSERT_LED_PIN     LED_2


#endif
