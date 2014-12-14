/** @file het.c 
*   @brief HET Driver Implementation File
*   @date 25.July.2013
*   @version 03.06.00
*
*/

/* (c) Texas Instruments 2009-2013, All rights reserved. */

#include "bl_het.h"
//#include "sys_vim.h"




/** @fn void hetInit(void)
*   @brief Initializes the het Driver
*
*   This function initializes the het 1 module.
*/
void bl_hetInit(void)
{
    /** @b initialize @b HET */

    /** - Set HET pins default output value */
    bl_hetREG1->DOUT = (unsigned int)(0U << 31U)
                 | (unsigned int)(0U << 30U)
                 | (unsigned int)(1U << 29U)
                 | (unsigned int)(0U << 28U)
                 | (unsigned int)(0U << 27U)
                 | (unsigned int)(0U << 26U)
                 | (unsigned int)(0U << 25U)
                 | (unsigned int)(0U << 24U)
                 | (unsigned int)(1U << 23U)
                 | (unsigned int)(0U << 22U)
                 | (unsigned int)(0U << 21U)
                 | (unsigned int)(0U << 20U)
                 | (unsigned int)(0U << 19U)
                 | (unsigned int)(0U << 18U)
                 | (unsigned int)(1U << 17U)
                 | (unsigned int)(0U << 16U)
                 | (unsigned int)(0U << 15U)
                 | (unsigned int)(0U << 14U)
                 | (unsigned int)(0U << 13U)
                 | (unsigned int)(0U << 12U)
                 | (unsigned int)(0U << 11U)
                 | (unsigned int)(0U << 10U)
                 | (unsigned int)(0U << 9U)
                 | (unsigned int)(0U << 8U)
                 | (unsigned int)(0U << 7U)
                 | (unsigned int)(0U << 6U)
                 | (unsigned int)(0U << 5U)
                 | (unsigned int)(0U << 4U)
                 | (unsigned int)(0U << 3U)
                 | (unsigned int)(0U << 2U)
                 | (unsigned int)(0U << 1U)
                 | (unsigned int)(0U);

    /** - Set HET pins direction */
    bl_hetREG1->DIR = (unsigned int) 0x80000000U
                | (unsigned int) 0x00000000U
                | (unsigned int) 0x20000000U
                | (unsigned int) 0x00000000U
                | (unsigned int) 0x08000000U
                | (unsigned int) 0x00000000U
                | (unsigned int) 0x00000000U
                | (unsigned int) 0x00000000U
                | (unsigned int) 0x00800000U
                | (unsigned int) 0x00000000U
                | (unsigned int) 0x00200000U
                | (unsigned int) 0x00000000U
                | (unsigned int) 0x00080000U
                | (unsigned int) 0x00040000U
                | (unsigned int) 0x00020000U
                | (unsigned int) 0x00000000U
                | (unsigned int) 0x00000000U
                | (unsigned int) 0x00004000U
                | (unsigned int) 0x00000000U
                | (unsigned int) 0x00000000U
                | (unsigned int) 0x00000000U
                | (unsigned int) 0x00000400U
                | (unsigned int) 0x00000000U
                | (unsigned int) 0x00000100U
                | (unsigned int) 0x00000000U
                | (unsigned int) 0x00000000U
                | (unsigned int) 0x00000020U
                | (unsigned int) 0x00000010U
                | (unsigned int) 0x00000000U
                | (unsigned int) 0x00000000U
                | (unsigned int) 0x00000000U
                | (unsigned int) 0x00000001U;

    /** - Set HET pins open drain enable */
    bl_hetREG1->PDR = (unsigned int) 0x00000000U
                | (unsigned int) 0x00000000U
                | (unsigned int) 0x00000000U
                | (unsigned int) 0x00000000U
                | (unsigned int) 0x00000000U
                | (unsigned int) 0x00000000U
                | (unsigned int) 0x00000000U
                | (unsigned int) 0x00000000U
                | (unsigned int) 0x00000000U
                | (unsigned int) 0x00000000U
                | (unsigned int) 0x00000000U
                | (unsigned int) 0x00000000U
                | (unsigned int) 0x00000000U
                | (unsigned int) 0x00000000U
                | (unsigned int) 0x00000000U
                | (unsigned int) 0x00000000U
                | (unsigned int) 0x00000000U
                | (unsigned int) 0x00000000U
                | (unsigned int) 0x00000000U
                | (unsigned int) 0x00000000U
                | (unsigned int) 0x00000000U
                | (unsigned int) 0x00000000U
                | (unsigned int) 0x00000000U
                | (unsigned int) 0x00000000U
                | (unsigned int) 0x00000000U
                | (unsigned int) 0x00000000U
                | (unsigned int) 0x00000000U
                | (unsigned int) 0x00000000U
                | (unsigned int) 0x00000000U
                | (unsigned int) 0x00000000U
                | (unsigned int) 0x00000000U
                | (unsigned int) 0x00000000U;

    /** - Set HET pins pullup/down enable */
    bl_hetREG1->PULDIS = (unsigned int) 0x00000000U
                   | (unsigned int) 0x00000000U
                   | (unsigned int) 0x00000000U
                   | (unsigned int) 0x00000000U
                   | (unsigned int) 0x00000000U
                   | (unsigned int) 0x00000000U
                   | (unsigned int) 0x00000000U
                   | (unsigned int) 0x00000000U
                   | (unsigned int) 0x00000000U
                   | (unsigned int) 0x00000000U
                   | (unsigned int) 0x00000000U
                   | (unsigned int) 0x00000000U
                   | (unsigned int) 0x00000000U
                   | (unsigned int) 0x00000000U
                   | (unsigned int) 0x00000000U
                   | (unsigned int) 0x00000000U
                   | (unsigned int) 0x00000000U
                   | (unsigned int) 0x00000000U
                   | (unsigned int) 0x00000000U
                   | (unsigned int) 0x00000000U
                   | (unsigned int) 0x00000000U
                   | (unsigned int) 0x00000000U
                   | (unsigned int) 0x00000200U
                   | (unsigned int) 0x00000000U
                   | (unsigned int) 0x00000000U
                   | (unsigned int) 0x00000000U
                   | (unsigned int) 0x00000020U
                   | (unsigned int) 0x00000000U
                   | (unsigned int) 0x00000000U
                   | (unsigned int) 0x00000000U
                   | (unsigned int) 0x00000000U
                   | (unsigned int) 0x00000000U;

    /** - Set HET pins pullup/down select */
    bl_hetREG1->PSL = (unsigned int) 0x00000000U
                | (unsigned int) 0x00000000U
                | (unsigned int) 0x20000000U
                | (unsigned int) 0x00000000U
                | (unsigned int) 0x08000000U
                | (unsigned int) 0x00000000U
                | (unsigned int) 0x02000000U
                | (unsigned int) 0x00000000U
                | (unsigned int) 0x00800000U
                | (unsigned int) 0x00000000U
                | (unsigned int) 0x00200000U
                | (unsigned int) 0x00000000U
                | (unsigned int) 0x00000000U
                | (unsigned int) 0x00040000U
                | (unsigned int) 0x00020000U
                | (unsigned int) 0x00010000U
                | (unsigned int) 0x00000000U
                | (unsigned int) 0x00000000U
                | (unsigned int) 0x00000000U
                | (unsigned int) 0x00001000U
                | (unsigned int) 0x00000800U
                | (unsigned int) 0x00000000U
                | (unsigned int) 0x00000200U
                | (unsigned int) 0x00000000U
                | (unsigned int) 0x00000080U
                | (unsigned int) 0x00000000U
                | (unsigned int) 0x00000020U
                | (unsigned int) 0x00000000U
                | (unsigned int) 0x00000000U
                | (unsigned int) 0x00000000U
                | (unsigned int) 0x00000000U
                | (unsigned int) 0x00000001U;

    /** - Set HET pins high resolution share */
    bl_hetREG1->HRSH = (unsigned int) 0x00008000U
                 | (unsigned int) 0x00004000U
                 | (unsigned int) 0x00002000U
                 | (unsigned int) 0x00001000U
                 | (unsigned int) 0x00000000U
                 | (unsigned int) 0x00000000U
                 | (unsigned int) 0x00000000U
                 | (unsigned int) 0x00000000U
                 | (unsigned int) 0x00000000U
                 | (unsigned int) 0x00000000U
                 | (unsigned int) 0x00000000U
                 | (unsigned int) 0x00000000U
                 | (unsigned int) 0x00000000U
                 | (unsigned int) 0x00000004U
                 | (unsigned int) 0x00000000U
                 | (unsigned int) 0x00000000U;

    /** - Set HET pins AND share */
    bl_hetREG1->AND = (unsigned int) 0x00000000U
                 | (unsigned int) 0x00000000U
                 | (unsigned int) 0x00000000U
                 | (unsigned int) 0x00000000U
                 | (unsigned int) 0x00000000U
                 | (unsigned int) 0x00000000U
                 | (unsigned int) 0x00000000U
                 | (unsigned int) 0x00000000U
                 | (unsigned int) 0x00000000U
                 | (unsigned int) 0x00000000U
                 | (unsigned int) 0x00000000U
                 | (unsigned int) 0x00000000U
                 | (unsigned int) 0x00000000U
                 | (unsigned int) 0x00000000U
                 | (unsigned int) 0x00000000U
                 | (unsigned int) 0x00000000U;

    /** - Set HET pins XOR share */
    bl_hetREG1->XOR = (unsigned int) 0x00000000U
                 | (unsigned int) 0x00000000U
                 | (unsigned int) 0x00000000U
                 | (unsigned int) 0x00000000U
                 | (unsigned int) 0x00000000U
                 | (unsigned int) 0x00000000U
                 | (unsigned int) 0x00000000U
                 | (unsigned int) 0x00000000U
                 | (unsigned int) 0x00000000U
                 | (unsigned int) 0x00000000U
                 | (unsigned int) 0x00000000U
                 | (unsigned int) 0x00000000U
                 | (unsigned int) 0x00000000U
                 | (unsigned int) 0x00000000U
                 | (unsigned int) 0x00000000U
                 | (unsigned int) 0x00000000U;
                 
/* USER CODE BEGIN (1) */
/* USER CODE END */

   /** - Parity control register 
    *     - Enable/Disable Parity check
    */
    bl_hetREG1->PCR = (unsigned int) 0x00000005U;

   /** - Setup control register 
    *     - Enable output buffers
    *     - Ignore software breakpoints
	*     - Master mode since only one HET is active
    *     - Enable HET
    */
    bl_hetREG1->GCR = 0x01030001U;


}


