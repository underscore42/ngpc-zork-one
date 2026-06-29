/* parser.h - Zork Neo: Input parser */
#ifndef PARSER_H
#define PARSER_H

#include "zork_types.h"

void parser_init(void);
void parser_tick(void);         /* call once per frame - reads joypad */
void parser_build_nouns(void);  /* rebuild noun list for current verb+room */

#endif
