#ifndef __CUSTOM_BOARD_H
#define __CUSTOM_BOARD_H

/* Include which version of the custom board */

#ifdef BOARD_ANT_IO
  #include "ant_io_board_v_5.h"
#else
#error "Custom board is not defined"
#endif




#endif
