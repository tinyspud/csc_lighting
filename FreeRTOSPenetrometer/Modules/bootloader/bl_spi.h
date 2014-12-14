/** @file spi.h
*   @brief SPI Driver Definition File
*   @date 25.July.2013
*   @version 03.06.00
*   
*   (c) Texas Instruments 2009-2013, All rights reserved.
*/


#ifndef __BL_SPI_H__
#define __BL_SPI_H__

#include "bl_reg_spi.h"

//typedef enum SpiTxRxDataStatus
//{
//    SPI_READY = 0U,
//	SPI_PENDING = 1U,
//    SPI_COMPLETED = 2U
//}SpiDataStatus_t;
//
//
///** @struct spiDAT1RegConfig
//*   @brief SPI data register configuration
//*/

typedef enum dataformat
{
    SPI_FMT_0 = 0U,
    SPI_FMT_1 = 1U,
    SPI_FMT_2 = 2U,
    SPI_FMT_3 = 3U
}bl_SPIDATAFMT_t;

typedef struct spiDAT1RegConfig
{
    unsigned int CS_HOLD;
    unsigned int WDEL;
    bl_SPIDATAFMT_t DFSEL;
    unsigned char CSNR;
}bl_spiDAT1_t;


/** 
 *  @defgroup SPI SPI
 *  @brief Serial Peripheral Interface Module.
 *  
 *  SPI is a high-speed synchronous serial input/output port that allows a serial bit stream of
 *  programmed length (2 to 16 bits) to be shifted in and out of the device at a programmed bit-transfer rate.
 *
 *	Related Files
 *   - reg_spi.h
 *   - spi.h
 *   - spi.c
 *  @addtogroup SPI
 *  @{
 */
 
/* SPI Interface Functions */
void bl_spiInit(void);
unsigned int bl_spiTransmitAndReceiveData(bl_spiBASE_t *spi, bl_spiDAT1_t *dataconfig_t, unsigned int blocksize, unsigned short * srcbuff, unsigned short * destbuff);

/**@}*/
/* USER CODE BEGIN (1) */
/* USER CODE END */

#endif
