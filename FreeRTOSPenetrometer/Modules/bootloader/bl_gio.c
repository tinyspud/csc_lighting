/** @file gio.c 
*   @brief GIO Driver Implementation File
*   @date 25.July.2013
*   @version 03.06.00
*
*/

/* (c) Texas Instruments 2009-2013, All rights reserved. */

/* USER CODE BEGIN (0) */
/* USER CODE END */

#include "bl_gio.h"
//#include "sys_vim.h"

/* USER CODE BEGIN (1) */
/* USER CODE END */

/** @fn void gioInit(void)
*   @brief Initializes the GIO Driver
*
*   This function initializes the GIO module and set the GIO ports 
*   to the initial values.
*/
/* SourceId : GIO_SourceId_001 */
/* Requirements : HL_SR115, HL_SR116, HL_SR117, HL_SR118, HL_SR119, HL_SR120, HL_SR121, HL_SR122 */
void bl_gioInit(void)
{
/* USER CODE BEGIN (2) */
/* USER CODE END */

    /** bring GIO module out of reset */
    bl_gioREG->GCR0      = 1U;
    bl_gioREG->ENACLR = 0xFFU;
    bl_gioREG->LVLCLR    = 0xFFU;

    /** @b initialize @b Port @b A */

    /** - Port A output values */
    bl_gioPORTA->DOUT =  0U         /* Bit 0 */
                   | (1U << 1U)  /* Bit 1 */
                   | (0U << 2U)  /* Bit 2 */
                   | (1U << 3U)  /* Bit 3 */
                   | (0U << 4U)  /* Bit 4 */
                   | (0U << 5U)  /* Bit 5 */
                   | (0U << 6U)  /* Bit 6 */
                   | (0U << 7U); /* Bit 7 */

    /** - Port A direction */
    bl_gioPORTA->DIR  =  1U         /* Bit 0 */
                   | (0U << 1U)  /* Bit 1 */
                   | (1U << 2U)  /* Bit 2 */
                   | (0U << 3U)  /* Bit 3 */
                   | (1U << 4U)  /* Bit 4 */
                   | (1U << 5U)  /* Bit 5 */
                   | (1U << 6U)  /* Bit 6 */
                   | (0U << 7U); /* Bit 7 */

    /** - Port A open drain enable */
    bl_gioPORTA->PDR  =  0U         /* Bit 0 */
                   | (0U << 1U)  /* Bit 1 */
                   | (0U << 2U)  /* Bit 2 */
                   | (0U << 3U)  /* Bit 3 */
                   | (0U << 4U)  /* Bit 4 */
                   | (0U << 5U)  /* Bit 5 */
                   | (0U << 6U)  /* Bit 6 */
                   | (0U << 7U); /* Bit 7 */

    /** - Port A pullup / pulldown selection */
    bl_gioPORTA->PSL  =  0U         /* Bit 0 */
                   | (1U << 1U)  /* Bit 1 */
                   | (0U << 2U)  /* Bit 2 */
                   | (1U << 3U)  /* Bit 3 */
                   | (0U << 4U)  /* Bit 4 */
                   | (0U << 5U)  /* Bit 5 */
                   | (1U << 6U)  /* Bit 6 */
                   | (1U << 7U); /* Bit 7 */

    /** - Port A pullup / pulldown enable*/
    bl_gioPORTA->PULDIS  =  0U         /* Bit 0 */
                      | (0U << 1U)  /* Bit 1 */
                      | (0U << 2U)  /* Bit 2 */
                      | (0U << 3U)  /* Bit 3 */
                      | (0U << 4U)  /* Bit 4 */
                      | (0U << 5U)  /* Bit 5 */
                      | (0U << 6U)  /* Bit 6 */
                      | (0U << 7U); /* Bit 7 */

    /** @b initialize @b Port @b B */


    /** @b initialize @b interrupts */

    /** - interrupt polarity */
    bl_gioREG->POL =  0U         /* Bit 0 */
                | (0U << 1U)  /* Bit 1 */
                | (0U << 2U)  /* Bit 2 */
                | (0U << 3U)  /* Bit 3 */
                | (0U << 4U)  /* Bit 4 */
                | (0U << 5U)  /* Bit 5 */
                | (0U << 6U)  /* Bit 6 */
                | (0U << 7U)  /* Bit 7 */

                | (0U << 8U)  /* Bit 8  */
                | (0U << 9U)  /* Bit 9  */
                | (0U << 10U) /* Bit 10 */
                | (0U << 11U) /* Bit 11 */
                | (1U << 12U) /* Bit 12 */
                | (1U << 13U) /* Bit 13 */
                | (1U << 14U) /* Bit 14 */
                | (1U << 15U);/* Bit 15 */


    /** - interrupt level */
    bl_gioREG->LVLSET =  0U         /* Bit 0 */
                   | (0U << 1U)  /* Bit 1 */
                   | (0U << 2U)  /* Bit 2 */
                   | (0U << 3U)  /* Bit 3 */
                   | (0U << 4U)  /* Bit 4 */
                   | (0U << 5U)  /* Bit 5 */
                   | (0U << 6U)  /* Bit 6 */
                   | (0U << 7U)  /* Bit 7 */

                   | (0U << 8U)  /* Bit 8  */
                   | (0U << 9U)  /* Bit 9  */
                   | (0U << 10U) /* Bit 10 */
                   | (0U << 11U) /* Bit 11 */
                   | (0U << 12U) /* Bit 12 */
                   | (0U << 13U) /* Bit 13 */
                   | (0U << 14U) /* Bit 14 */
                   | (0U << 15U);/* Bit 15 */

    


    /** - clear all pending interrupts */
    bl_gioREG->FLG = 0xFFU;

    /** - enable interrupts */
    bl_gioREG->ENASET =  0U         /* Bit 0 */
                      | (0U << 1U)  /* Bit 1 */
                      | (0U << 2U)  /* Bit 2 */
                      | (0U << 3U)  /* Bit 3 */
                      | (0U << 4U)  /* Bit 4 */
                      | (0U << 5U)  /* Bit 5 */
                      | (0U << 6U)  /* Bit 6 */
                      | (0U << 7U)  /* Bit 7 */

                      | (1U << 8U)  /* Bit 8  */
                      | (0U << 9U)  /* Bit 9  */
                      | (0U << 10U) /* Bit 10 */
                      | (0U << 11U) /* Bit 11 */
                      | (1U << 12U) /* Bit 12 */
                      | (1U << 13U) /* Bit 13 */
                      | (1U << 14U) /* Bit 14 */
                      | (1U << 15U);/* Bit 15 */

/* USER CODE BEGIN (4) */
/* USER CODE END */
}

