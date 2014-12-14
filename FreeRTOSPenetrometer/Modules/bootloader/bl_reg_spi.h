/** @file reg_spi.h
*   @brief SPI Register Layer Header File
*   @date 25.July.2013
*   @version 03.06.00
*   
*   This file contains:
*   - Definitions
*   - Types
*   - Interface Prototypes
*   .
*   which are relevant for the SPI driver.
*/

/* (c) Texas Instruments 2009-2013, All rights reserved. */

#ifndef __BL_REG_SPI_H__
#define __BL_REG_SPI_H__


#include "bl_gio.h"



/* USER CODE BEGIN (0) */
/* USER CODE END */

/* Spi Register Frame Definition */
/** @struct spiBase
*   @brief SPI Register Definition
*
*   This structure is used to access the SPI module registers.
*/
/** @typedef spiBASE_t
*   @brief SPI Register Frame Type Definition
*
*   This type is used to access the SPI Registers.
*/
typedef volatile struct spiBase
{
    unsigned int  GCR0;                 /**< 0x0000: Global Control 0 */
    unsigned int  GCR1;                 /**< 0x0004: Global Control 1 */
    unsigned int  INT0;                 /**< 0x0008: Interrupt Register */
    unsigned int  LVL;                  /**< 0x000C: Interrupt Level */
    unsigned int  FLG;                  /**< 0x0010: Interrupt flags */
    unsigned int  PC0;                  /**< 0x0014: Function Pin Enable */
    unsigned int  PC1;                  /**< 0x0018: Pin Direction */
    unsigned int  PC2;                  /**< 0x001C: Pin Input Latch */
    unsigned int  PC3;                  /**< 0x0020: Pin Output Latch */
    unsigned int  PC4;                  /**< 0x0024: Output Pin Set */
    unsigned int  PC5;                  /**< 0x0028: Output Pin Clr */
    unsigned int  PC6;                  /**< 0x002C: Open Drain Output Enable */
    unsigned int  PC7;                  /**< 0x0030: Pullup/Pulldown Disable */
    unsigned int  PC8;                  /**< 0x0034: Pullup/Pulldown Selection */
    unsigned int  DAT0;                 /**< 0x0038: Transmit Data */
    unsigned int  DAT1;                 /**< 0x003C: Transmit Data with Format and Chip Select */
    unsigned int  BUF;                  /**< 0x0040: Receive Buffer */
    unsigned int  EMU;                  /**< 0x0044: Emulation Receive Buffer */
    unsigned int  DELAY;                /**< 0x0048: Delays */
    unsigned int  DEF;                  /**< 0x004C: Default Chip Select */
    unsigned int  FMT0;                 /**< 0x0050: Data Format 0 */
    unsigned int  FMT1;                 /**< 0x0054: Data Format 1 */
    unsigned int  FMT2;                 /**< 0x0058: Data Format 2 */
    unsigned int  FMT3;                 /**< 0x005C: Data Format 3 */
    unsigned int  INTVECT0;             /**< 0x0060: Interrupt Vector 0 */
    unsigned int  INTVECT1;             /**< 0x0064: Interrupt Vector 1 */
    unsigned int  RESERVED[51U];        /**< 0x0068 to 0x0130: Reserved */
    unsigned int  IOLPKTSTCR;           /**< 0x0134: IO loopback */
} bl_spiBASE_t;



/** @def spiREG3
*   @brief SPI3 (MIBSPI - Compatibility Mode) Register Frame Pointer
*
*   This pointer is used by the SPI driver to access the spi module registers.
*/
#define bl_spiREG3 ((bl_spiBASE_t *)0xFFF7F800U)


/** @def spiPORT3
*   @brief SPI3 (MIBSPI - Compatibility Mode) GIO Port Register Pointer
*
*   Pointer used by the GIO driver to access I/O PORT of SPI3
*   (use the GIO drivers to access the port pins).
*/
#define bl_spiPORT3 ((bl_gioPORT_t *)0xFFF7F818U)

/* USER CODE BEGIN (1) */
/* USER CODE END */


#endif
