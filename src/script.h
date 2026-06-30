#ifndef SCRIPT_H
#define SCRIPT_H
#include "zork_types.h"
void script_start(u8 checkpoint);
void script_stop(void);
u8   script_is_active(void);
void script_tick(void);
#endif
