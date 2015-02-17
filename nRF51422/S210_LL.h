#ifndef __S210_LL_H
#define __S210_LL_H

#include <stdint.h>

#define BUFFER_INDEX_MESG_ID          0x01u             /**< Index for Message ID. */
#define BUFFER_INDEX_MESG_DATA        0x03u             /**< Index for Data. */

void init_S210_LL(void);

void open_channel_0(void);

uint32_t handle_ANT_events(void);

#endif
