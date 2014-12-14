/** @file het.h
*   @brief HET Driver Definition File
*   @date 25.July.2013
*   @version 03.06.00
*   
*/

/* (c) Texas Instruments 2009-2013, All rights reserved. */


#ifndef __BL_HET_H__
#define __BL_HET_H__

//#include "reg_het.h"
//#include <string.h>
#include "bl_gio.h"

typedef volatile struct bl_hetBase
{
    unsigned int GCR;     /**< 0x0000: Global control register              */
    unsigned int PFR;     /**< 0x0004: Prescale factor register             */
    unsigned int ADDR;    /**< 0x0008: Current address register             */
    unsigned int OFF1;    /**< 0x000C: Interrupt offset register 1          */
    unsigned int OFF2;    /**< 0x0010: Interrupt offset register 2          */
    unsigned int INTENAS; /**< 0x0014: Interrupt enable set register        */
    unsigned int INTENAC; /**< 0x0018: Interrupt enable clear register      */
    unsigned int EXC1;    /**< 0x001C: Exception control register 1          */
    unsigned int EXC2;    /**< 0x0020: Exception control register 2          */
    unsigned int PRY;     /**< 0x0024: Interrupt priority register          */
    unsigned int FLG;     /**< 0x0028: Interrupt flag register              */
    unsigned int AND;     /**< 0x002C: AND share control register         */
    unsigned int   rsvd1; /**< 0x0030: Reserved                             */
    unsigned int HRSH;    /**< 0x0034: High resolution share register        */
    unsigned int XOR;     /**< 0x0038: XOR share register                   */
    unsigned int REQENS;  /**< 0x003C: Request enable set register          */
    unsigned int REQENC;  /**< 0x0040: Request enable clear register        */
    unsigned int REQDS;   /**< 0x0044: Request destination select register  */
    unsigned int   rsvd2; /**< 0x0048: Reserved                             */
    unsigned int DIR;     /**< 0x004C: Direction register                   */
    unsigned int DIN;     /**< 0x0050: Data input register                  */
    unsigned int DOUT;    /**< 0x0054: Data output register                 */
    unsigned int DSET;    /**< 0x0058: Data output set register             */
    unsigned int DCLR;    /**< 0x005C: Data output clear register           */
    unsigned int PDR;     /**< 0x0060: Open drain register                  */
    unsigned int PULDIS;  /**< 0x0064: Pull disable register                */
    unsigned int PSL;     /**< 0x0068: Pull select register                 */
    unsigned int   rsvd3; /**< 0x006C: Reserved                             */
    unsigned int   rsvd4; /**< 0x0070: Reserved                             */
    unsigned int PCR;   /**< 0x0074: Parity control register              */
    unsigned int PAR;     /**< 0x0078: Parity address register              */
    unsigned int PPR;     /**< 0x007C: Parity pin select register           */
    unsigned int SFPRLD;  /**< 0x0080: Suppression filter preload register  */
    unsigned int SFENA;   /**< 0x0084: Suppression filter enable register   */
    unsigned int   rsvd5; /**< 0x0088: Reserved                             */
    unsigned int LBPSEL;  /**< 0x008C: Loop back pair select register       */
    unsigned int LBPDIR;  /**< 0x0090: Loop back pair direction register    */
    unsigned int PINDIS;  /**< 0x0094: Pin disable register                 */
} bl_hetBASE_t;


/** @def hetREG1
*   @brief HET Register Frame Pointer
*
*   This pointer is used by the HET driver to access the het module registers.
*/
#define bl_hetREG1 ((bl_hetBASE_t *)0xFFF7B800U)


/** @def hetPORT1
*   @brief HET GIO Port Register Pointer
*
*   Pointer used by the GIO driver to access I/O PORT of HET1
*   (use the GIO drivers to access the port pins).
*/
#define bl_hetPORT1 ((bl_gioPORT_t *)0xFFF7B84CU)


//#define hetRAM1 ((hetRAMBASE_t *)0xFF460000U)

//#define NHET1RAMPARLOC	(*(volatile unsigned int *)0xFF462000U)
//#define NHET1RAMLOC		(*(volatile unsigned int *)0xFF460000U)




/* Configuration registers */
//typedef struct het_config_reg
//{
//    unsigned int CONFIG_GCR;
//    unsigned int CONFIG_PFR;
//    unsigned int CONFIG_INTENAS;
//    unsigned int CONFIG_INTENAC;
//    unsigned int CONFIG_PRY;
//    unsigned int CONFIG_AND;
//    unsigned int CONFIG_HRSH;
//    unsigned int CONFIG_XOR;
//    unsigned int CONFIG_DIR;
//    unsigned int CONFIG_PDR;
//	unsigned int CONFIG_PULDIS;
//	unsigned int CONFIG_PSL;
//	unsigned int CONFIG_PCR;
//} het_config_reg_t;


/* HET Interface Functions */
void bl_hetInit(void);


/**@}*/
/* USER CODE BEGIN (2) */
/* USER CODE END */
#endif
