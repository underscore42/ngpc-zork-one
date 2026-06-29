/* engine.h - Zork Neo: Game engine */
#ifndef ENGINE_H
#define ENGINE_H

#include "zork_types.h"

/* Global mutable game state - defined in engine.c */
extern u8  g_player_room;
extern u16 g_score;
extern u16 g_moves;
extern u8  g_lamp_fuel;
extern u8  g_dead;
extern u8  g_obj_loc[NUM_OBJECTS];
extern u8  g_obj_flags[NUM_OBJECTS];
extern u8  g_room_flags[NUM_ROOMS];

void engine_init(void);
void engine_execute(u8 verb, u8 obj);   /* 0xFF obj = no object */
void engine_do_look(void);
void engine_do_inventory(void);
void engine_refresh_screen(void);       /* redraw all UI rows */
void engine_add_score(u16 pts);

/* Room description builder */
void engine_describe_room(u8 room_id, u8 force_long);

#endif
