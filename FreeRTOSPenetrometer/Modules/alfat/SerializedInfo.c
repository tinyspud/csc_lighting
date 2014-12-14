
#include "hal_stdtypes.h"
#include "reg_system.h"
#include "SerializedInfo.h"

uint32 DieID[NUM_DIE_ID_REGS];
char DieIDStringNullTerminated[DIE_ID_STR_LEN];


void GatherIdentifiersAndParameters(){

	uint8_t j = 0, k = 0;
	int i = 0;
	for(i = 0; i < NUM_DIE_ID_REGS; i++)
		DieID[i] = 0;

	/* Retreive the 64-bit die identifier */
	DieID[0] = systemREG2->DIEIDL_REG0;
	DieID[1] = systemREG2->DIEIDH_REG1;
	DieID[2] = systemREG2->DIEIDL_REG2;
	DieID[3] = systemREG2->DIEIDH_REG3;

	/* Init string to all NULLs */
	for(i = 0; i < DIE_ID_STR_LEN; i++)
		DieIDStringNullTerminated[i] = 0x00;

	for(j = 0, i = (NUM_DIE_ID_REGS - 1), k = 0; (i >= 0) & (j < DIE_ID_STR_LEN); i--){
		for(k = 0; (k < 8) & (j < DIE_ID_STR_LEN); k++, j++){
			/* Make lower case so it fits on the screen */
//			DieIDStringNullTerminated[j] = (char)((((DieID[i] >> ((7 - k) * 4)) & 0x0000000F) > 9 ? (87) : (48)) +
//					((DieID[i] >> ((7 - k) * 4)) & 0x0000000F));

			DieIDStringNullTerminated[j] = (char)((((DieID[i] >> ((7 - k) * 4)) & 0x0000000F) > 9 ? (55) : (48)) +
					((DieID[i] >> ((7 - k) * 4)) & 0x0000000F));
		}
	}

}

