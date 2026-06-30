/* engine.h - Zork Neo */
#ifndef ENGINE_H
#define ENGINE_H

#include "zork_types.h"

void engine_init(void);
void engine_execute(u8 verb, u8 obj);
void engine_do_look(void);
void engine_do_inventory(void);
void engine_refresh_screen(void);
void engine_describe_room(u8 room_id, u8 force_long);
void engine_add_score(u16 pts);

#endif
