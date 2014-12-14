/** @file gio.h
*   @brief GIO Driver Definition File
*   @date 25.July.2013
*   @version 03.06.00
*   
*/

/* (c) Texas Instruments 2009-2013, All rights reserved. */

#ifndef __GIO_H__
#define __GIO_H__


/** @struct gioPort
*   @brief GIO Port Register Definition
*/
/** @typedef gioPORT_t
*   @brief GIO Port Register Type Definition
*
*   This type is used to access the GIO Port Registers.
*/


typedef volatile struct gioBase
{
    unsigned int GCR0;      /**< 0x0000: Global Control Register */
    unsigned int   rsvd;    /**< 0x0004: Reserved*/
    unsigned int INTDET;    /**< 0x0008: Interrupt Detect Register*/
    unsigned int POL;       /**< 0x000C: Interrupt Polarity Register */
    unsigned int ENASET;    /**< 0x0010: Interrupt Enable Set Register */
    unsigned int ENACLR;    /**< 0x0014: Interrupt Enable Clear Register */
    unsigned int LVLSET;    /**< 0x0018: Interrupt Priority Set Register */
    unsigned int LVLCLR;    /**< 0x001C: Interrupt Priority Clear Register */
    unsigned int FLG;       /**< 0x0020: Interrupt Flag Register */
    unsigned int OFF1;      /**< 0x0024: Interrupt Offset A Register */
    unsigned int OFF2;      /**< 0x0028: Interrupt Offset B Register */
} bl_gioBASE_t;


typedef volatile struct gioPort
{
	unsigned int DIR;    /**< 0x0000: Data Direction Register */
	unsigned int DIN;    /**< 0x0004: Data Input Register */
	unsigned int DOUT;   /**< 0x0008: Data Output Register */
	unsigned int DSET;   /**< 0x000C: Data Output Set Register */
	unsigned int DCLR;   /**< 0x0010: Data Output Clear Register */
	unsigned int PDR;    /**< 0x0014: Open Drain Register */
	unsigned int PULDIS; /**< 0x0018: Pullup Disable Register */
	unsigned int PSL;    /**< 0x001C: Pull Up/Down Selection Register */
} bl_gioPORT_t;


void bl_gioInit(void);

#define bl_gioREG   ((bl_gioBASE_t *)0xFFF7BC00U)

#define bl_gioPORTA ((bl_gioPORT_t *)0xFFF7BC34U)

/**@}*/
#endif
