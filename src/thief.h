#ifndef THIEF_H
#define THIEF_H
#include "zork_types.h"
extern u8 g_thief_room;
extern u8 g_thief_visible;
extern u8 g_thief_wounded;
void thief_init(void);
void thief_tick(void);
void thief_attack(u8 weapon_obj);
#endif
