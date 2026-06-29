/* zork_save.c - Zork Neo: Flash save/load
 *
 * Pack all mutable game state into exactly 128 u16s (256 bytes) -
 * the minimum flash block write size.
 *
 * Save layout (words):
 *   [0]  SAVE_MAGIC_1  (0xA5A5)
 *   [1]  SAVE_MAGIC_2  (0x5A5A)
 *   [2]  player_room
 *   [3]  score
 *   [4]  moves
 *   [5]  lamp_fuel
 *   [6]  dead flag
 *   [7..21]  obj_loc[0..29]  packed 2 per word (hi=even, lo=odd)
 *   [22..36] obj_flags[0..29] packed 2 per word
 *   [37..42] room_flags[0..23] packed 4 per word (2 bits each: visited+dark)
 *   [43..127] = 0 (reserved / padding)
 *
 * Using GetSavedData/Flash from library.c (same as asteroids save)
 * Important: save_buf MUST be 128 u16s exactly for the flash driver.
 */
#include "zork_save.h"
#include "engine.h"
#include "text.h"

/* Using library.c GetSavedData / Flash */
/* These are declared in library.h */

static u16 save_buf[SAVE_BUF_WORDS];

void zork_save(void) {
    u8 i;
    u8 pair;

    /* Clear buffer */
    for (i = 0; i < SAVE_BUF_WORDS; i++) save_buf[i] = 0;

    /* Magic */
    save_buf[0] = SAVE_MAGIC_1;
    save_buf[1] = SAVE_MAGIC_2;

    /* Core state */
    save_buf[2] = (u16)g_player_room;
    save_buf[3] = g_score;
    save_buf[4] = g_moves;
    save_buf[5] = (u16)g_lamp_fuel;
    save_buf[6] = (u16)g_dead;

    /* obj_loc: pack 2 per word */
    for (i = 0; i < NUM_OBJECTS; i++) {
        pair = i >> 1;       /* word index relative to base */
        if (i & 1) {
            save_buf[7 + pair] = (save_buf[7 + pair] & 0xFF00u)
                                 | (u16)g_obj_loc[i];
        } else {
            save_buf[7 + pair] = ((u16)g_obj_loc[i] << 8)
                                 | (save_buf[7 + pair] & 0x00FFu);
        }
    }

    /* obj_flags: pack 2 per word, base word 22 */
    for (i = 0; i < NUM_OBJECTS; i++) {
        pair = i >> 1;
        if (i & 1) {
            save_buf[22 + pair] = (save_buf[22 + pair] & 0xFF00u)
                                  | (u16)g_obj_flags[i];
        } else {
            save_buf[22 + pair] = ((u16)g_obj_flags[i] << 8)
                                  | (save_buf[22 + pair] & 0x00FFu);
        }
    }

    /* room_flags: pack 4 per word (only bottom 2 bits matter: VISITED, DARK)
     * word 37: rooms 0-3, word 38: rooms 4-7, etc. */
    for (i = 0; i < NUM_ROOMS; i++) {
        u8 word_off;
        u8 bit_off;
        u16 mask;
        u16 val;
        word_off = i >> 2;
        bit_off  = (i & 3) << 2;  /* 4 bits per room */
        val  = (u16)(g_room_flags[i] & 0x0F) << bit_off;
        mask = (u16)0x000F << bit_off;
        save_buf[37 + word_off] = (save_buf[37 + word_off] & ~mask) | val;
    }

    Flash((void*)save_buf);

    text_println("Game saved.");
}

void zork_load(void) {
    u8 i;
    u8 pair;
    u16 word;

    GetSavedData((void*)save_buf);

    if (save_buf[0] != SAVE_MAGIC_1 || save_buf[1] != SAVE_MAGIC_2) {
        text_println("No saved game found.");
        return;
    }

    g_player_room = (u8)save_buf[2];
    g_score       = save_buf[3];
    g_moves       = save_buf[4];
    g_lamp_fuel   = (u8)save_buf[5];
    g_dead        = (u8)save_buf[6];

    /* obj_loc */
    for (i = 0; i < NUM_OBJECTS; i++) {
        pair = i >> 1;
        word = save_buf[7 + pair];
        if (i & 1) {
            g_obj_loc[i] = (u8)(word & 0xFF);
        } else {
            g_obj_loc[i] = (u8)(word >> 8);
        }
    }

    /* obj_flags */
    for (i = 0; i < NUM_OBJECTS; i++) {
        pair = i >> 1;
        word = save_buf[22 + pair];
        if (i & 1) {
            g_obj_flags[i] = (u8)(word & 0xFF);
        } else {
            g_obj_flags[i] = (u8)(word >> 8);
        }
    }

    /* room_flags */
    for (i = 0; i < NUM_ROOMS; i++) {
        u8 word_off;
        u8 bit_off;
        word_off = i >> 2;
        bit_off  = (i & 3) << 2;
        g_room_flags[i] = (u8)((save_buf[37 + word_off] >> bit_off) & 0x0F);
    }

    text_println("Game restored.");
}
