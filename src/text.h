/* text.h - Zork Neo: Text output engine */
#ifndef TEXT_H
#define TEXT_H

#include "zork_types.h"

void text_init(void);
void text_print(const char *s);    /* word-wrapped print, adds to scroll buffer */
void text_println(const char *s);  /* print + newline */
void text_newline(void);
void text_redraw(void);            /* redraw TEXT_ROWS from current scroll_top */
void text_scroll_up(void);         /* scroll output window up one line */
void text_scroll_down(void);       /* scroll output window down one line */
void text_clear(void);             /* clear buffer and screen */
void text_print_room_name(char *name);  /* in PAL_TITLE */
void text_print_cmd(const char *verb, char *noun); /* row ROW_CMD */
void text_draw_status(void);       /* row ROW_STATUS: score / moves */
void text_draw_divider(void);      /* row ROW_DIVIDER */
void text_draw_selector(void);     /* rows ROW_VERB / ROW_NOUN */

/* Low-level: write a 20-char padded string to a screen row */
void text_put_row(u8 row, u8 pal, char *s);
void text_put_row_centered(u8 row, u8 pal, char *s);

#endif
