

#ifndef __SERIALIZED_INFO_H
#define __SERIALIZED_INFO_H

#define NUM_DIE_ID_REGS	4
#define DIE_ID_STR_LEN ((NUM_DIE_ID_REGS * 8) + 1)

extern uint32 DieID[NUM_DIE_ID_REGS];
extern char DieIDStringNullTerminated[DIE_ID_STR_LEN];

void GatherIdentifiersAndParameters();

#endif
