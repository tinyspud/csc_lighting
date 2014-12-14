/** @file spi.c
*   @brief SPI Driver Implementation File
*   @date 25.July.2013
*   @version 03.06.00
*
*   (c) Texas Instruments 2009-2013, All rights reserved.
*/

/* USER CODE BEGIN (0) */
/* USER CODE END */

#include "bl_spi.h"
//#include "sys_vim.h"

/** @struct g_spiPacket
*   @brief globals
*
*/
//static struct g_spiPacket
//{
//    spiDAT1_t g_spiDataFormat;
//    unsigned int  tx_length;
//    unsigned int  rx_length;
//    unsigned short  * txdata_ptr;
//    unsigned short  * rxdata_ptr;
//    SpiDataStatus_t tx_data_status;
//    SpiDataStatus_t rx_data_status;
//} g_spiPacket_t[5U];

void bl_spiInit(void)
{
    /** @b initialize @b SPI3 */

    /** bring SPI out of reset */
    bl_spiREG3->GCR0 = 1U;

    /** SPI3 master mode and clock configuration */
    bl_spiREG3->GCR1 = (bl_spiREG3->GCR1 & 0xFFFFFFFCU) | ((1U << 1U)  /* CLOKMOD */
                  | 1U);  /* MASTER */

    /** SPI3 enable pin configuration */
    bl_spiREG3->INT0 = (bl_spiREG3->INT0 & 0xFEFFFFFFU) | (0U << 24U);  /* ENABLE HIGHZ */

    /** - Delays */
    bl_spiREG3->DELAY = (255U << 24U)  /* C2TDELAY */
                   | (255U << 16U)  /* T2CDELAY */
                   | (0U << 8U)  /* T2EDELAY */
                   | 0U;  /* C2EDELAY */

    /** - Data Format 0 */
    bl_spiREG3->FMT0 = (63U << 24U)  /* wdelay */
                  | (0U << 23U)  /* parity Polarity */
                  | (0U << 22U)  /* parity enable */
                  | (0U << 21U)  /* wait on enable */
                  | (0U << 20U)  /* shift direction */
                  | (0U << 17U)  /* clock polarity */
                  | (1U << 16U)  /* clock phase */
                  | (109U << 8U) /* baudrate prescale */
                  | 8U;  /* data word length */

    /** - Data Format 1 */
    bl_spiREG3->FMT1 = (0U << 24U)  /* wdelay */
                  | (0U << 23U)  /* parity Polarity */
                  | (0U << 22U)  /* parity enable */
                  | (0U << 21U)  /* wait on enable */
                  | (0U << 20U)  /* shift direction */
                  | (0U << 17U)  /* clock polarity */
                  | (0U << 16U)  /* clock phase */
                  | (109U << 8U) /* baudrate prescale */
                  | 16U;  /* data word length */

    /** - Data Format 2 */
    bl_spiREG3->FMT2 = (0U << 24U)  /* wdelay */
                  | (0U << 23U)  /* parity Polarity */
                  | (0U << 22U)  /* parity enable */
                  | (0U << 21U)  /* wait on enable */
                  | (0U << 20U)  /* shift direction */
                  | (0U << 17U)  /* clock polarity */
                  | (0U << 16U)  /* clock phase */
                  | (109U << 8U) /* baudrate prescale */
                  | 16U;  /* data word length */

    /** - Data Format 3 */
    bl_spiREG3->FMT3 = (0U << 24U)  /* wdelay */
                  | (0U << 23U)  /* parity Polarity */
                  | (0U << 22U)  /* parity enable */
                  | (0U << 21U)  /* wait on enable */
                  | (0U << 20U)  /* shift direction */
                  | (0U << 17U)  /* clock polarity */
                  | (0U << 16U)  /* clock phase */
                  | (109U << 8U) /* baudrate prescale */
                  | 16U;  /* data word length */

    /** - set interrupt levels */
    bl_spiREG3->LVL = (0U << 9U)  /* TXINT */
                 | (0U << 8U)  /* RXINT */
                 | (0U << 6U)  /* OVRNINT */
                 | (0U << 4U)  /* BITERR */
                 | (0U << 3U)  /* DESYNC */
                 | (0U << 2U)  /* PARERR */
                 | (0U << 1U) /* TIMEOUT */
                 | (0U);  /* DLENERR */

    /** - clear any pending interrupts */
    bl_spiREG3->FLG |= 0xFFFFU;

    /** - enable interrupts */
    bl_spiREG3->INT0 = (bl_spiREG3->INT0 & 0xFFFF0000U)
                  | (0U << 9U)  /* TXINT */
                  | (0U << 8U)  /* RXINT */
                  | (0U << 6U)  /* OVRNINT */
                  | (0U << 4U)  /* BITERR */
                  | (0U << 3U)  /* DESYNC */
                  | (0U << 2U)  /* PARERR */
                  | (0U << 1U) /* TIMEOUT */
                  | (0U);  /* DLENERR */

    /** @b initialize @b SPI3 @b Port */

    /** - SPI3 Port output values */
    bl_spiREG3->PC3 =  1U        /* SCS[0] */
                    | (0U << 8U)  /* ENA */
                    | (0U << 9U)  /* CLK */
                    | (1U << 10U)  /* SIMO */
                    | (0U << 11U); /* SOMI */

    /** - SPI3 Port direction */
    bl_spiREG3->PC1  =  1U        /* SCS[0] */
                    | (0U << 8U)  /* ENA */
                    | (1U << 9U)  /* CLK */
                    | (1U << 10U)  /* SIMO */
                    | (0U << 11U); /* SOMI */

    /** - SPI3 Port open drain enable */
    bl_spiREG3->PC6  =  0U        /* SCS[0] */
                    | (0U << 8U)  /* ENA */
                    | (0U << 9U)  /* CLK */
                    | (0U << 10U)  /* SIMO */
                    | (0U << 11U); /* SOMI */

    /** - SPI3 Port pullup / pulldown selection */
    bl_spiREG3->PC8  =  1U       /* SCS[0] */
                    | (1U << 8U)  /* ENA */
                    | (1U << 9U)  /* CLK */
                    | (1U << 10U)  /* SIMO */
                    | (1U << 11U); /* SOMI */

    /** - SPI3 Port pullup / pulldown enable*/
    bl_spiREG3->PC7  =  0U        /* SCS[0] */
                    | (0U << 8U)  /* ENA */
                    | (0U << 9U)  /* CLK */
                    | (0U << 10U)  /* SIMO */
                    | (0U << 11U); /* SOMI */

    /* SPI3 set all pins to functional */
    bl_spiREG3->PC0  =  0U        /* SCS[0] */
                    | (1U << 8U)  /* ENA */
                    | (1U << 9U)  /* CLK */
                    | (0U << 10U)  /* SIMO */
                    | (1U << 11U); /* SOMI */

    /** - Initialize TX and RX data buffer Status */                    
    //g_spiPacket_t[2U].tx_data_status  = SPI_READY;
    ///g_spiPacket_t[2U].rx_data_status  = SPI_READY;
                    
    /** - Finally start SPI3 */
    bl_spiREG3->GCR1 = (bl_spiREG3->GCR1 & 0xFEFFFFFFU) | (1U << 24U);


}


/** @fn uint32 spiTransmitAndReceiveData(spiBASE_t *spi, spiDAT1_t *dataconfig_t, uint32 blocksize, uint16 * srcbuff, uint16 * destbuff)
*   @brief Transmits and Receive Data using polling method
*   @param[in] spi           - Spi module base address
*   @param[in] dataconfig_t    - Spi DAT1 register configuration
*   @param[in] blocksize    - number of data
*   @param[in] srcbuff        - Pointer to the source data ( 16 bit).
*   @param[in] destbuff        - Pointer to the destination data ( 16 bit).
*
*   @return flag register value.
*
*   This function transmits and receives blocksize number of data from source buffer using polling method.
*/
unsigned int bl_spiTransmitAndReceiveData(bl_spiBASE_t *spi, bl_spiDAT1_t *dataconfig_t, unsigned int blocksize, unsigned short * srcbuff, unsigned short * destbuff)
{

	unsigned int Chip_Select_Hold = 0U;

/* USER CODE BEGIN (14) */
/* USER CODE END */
/*SAFETYMCUSW 28 D MR:NA <APPROVED> " loop to send data continuously" */
    do
    {
        if((spi->FLG & 0x000000FFU) !=0U)
        {
           break;
        }
          
        if(blocksize < 2U)
        {
           Chip_Select_Hold = 0U;
        }
        else
        {
           Chip_Select_Hold = (unsigned int)(dataconfig_t->CS_HOLD) << 28U;
        }       

        /*SAFETYMCUSW 51 S MR:12.3 <APPROVED> "Needs shifting for 32-bit value" */
		/*SAFETYMCUSW 45 D MR:21.1 <APPROVED> "Valid non NULL input parameters are only allowed in this driver" */
        spi->DAT1 = ((unsigned int)(dataconfig_t->DFSEL) << 24U) |
                   ((unsigned int)(dataconfig_t->CSNR)  << 16U) |
                   ((unsigned int)(dataconfig_t->WDEL)  << 26U) |
                   (Chip_Select_Hold)                     |
                   (*srcbuff);
	    srcbuff++;
	    /*SAFETYMCUSW 28 D MR:NA <APPROVED> "Hardware status bit read check" */
        while((spi->FLG & 0x00000100U) != 0x00000100U)
        { 
        } /* Wait */
	    /*SAFETYMCUSW 45 D MR:21.1 <APPROVED> "Valid non NULL input parameters are only allowed in this driver" */
        *destbuff = (unsigned short)spi->BUF;
	    destbuff++;
    
        blocksize--;
		
    }while(blocksize > 0U);

/* USER CODE BEGIN (15) */
/* USER CODE END */

    return (spi->FLG & 0xFFU);
}









