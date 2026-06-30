#ifndef WIZARD_H
#define WIZARD_H
#include "zork_types.h"
extern u8 g_wizard_mode;
void wizard_check_konami(u8 pad_press);
void wizard_tick(u8 pad_press, u8 pad_cur);
void wizard_log(const char *s);
void wizard_log_cmd(const char *verb, const char *noun);
u8   wizard_is_lit(void);
void wizard_draw_status(void);
#endif
