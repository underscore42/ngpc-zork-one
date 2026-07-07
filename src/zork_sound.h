/* zork_sound.h - Zork Neo: UI click sounds */
#ifndef ZORK_SOUND_H
#define ZORK_SOUND_H

#include "zork_types.h"

/* Sound effect slot IDs */
#define SND_NAV      0   /* L/R cycle verb or noun */
#define SND_CONFIRM  1   /* A press */
#define SND_CANCEL   2   /* B press */
#define SND_TICK     3   /* U/D scroll (edge press only) */

void zork_sound_init(void);   /* call once after InitNGPC(), before game loop */

#endif
