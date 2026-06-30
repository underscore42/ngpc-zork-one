/* zork_save.c - Zork Neo: Flash save/load
 *
 * Pack all mutable game state into exactly 128 u16s (256 bytes) -
 * the minimum flash block write size for GetSavedData/Flash.
 *
 * Word offsets are computed from NUM_OBJECTS / NUM_ROOMS at compile
 * time via macros, so adding objects or rooms later can't silently
 * desync save and load (it just needs OBJ_WORDS/ROOM_WORDS to still
 * fit under SAVE_BUF_WORDS).
 *
 * Save layout (words):
 *   [0]  SAVE_MAGIC_1  (0xA5A5)
 *   [1]  SAVE_MAGIC_2  (0x5A5A)
 *   [2]  player_room
 *   [3]  score
 *   [4]  moves
 *   [5]  lamp_fuel
 *   [6]  dead | (troll_alive << 1)
 *   [BASE_OBJ_LOC   .. +OBJ_WORDS-1]   obj_loc,   2 objects/word (hi=even idx)
 *   [BASE_OBJ_FLAGS .. +OBJ_WORDS-1]   obj_flags, 2 objects/word
 *   [BASE_ROOM_FLAGS.. +ROOM_WORDS-1]  room_flags, 4 rooms/word, 4 bits each
 *   [BASE_END .. 127] = 0 (reserved / padding)
 *
 * Uses GetSavedData/Flash from library.c (same flash driver as asteroids).
 */
#include "zork_save.h"
#include "engine.h"
#include "text.h"

/* ---- Word offset layout, computed from current object/room counts ---- */
#define OBJ_WORDS        ((NUM_OBJECTS + 1) / 2)
#define ROOM_WORDS       ((NUM_ROOMS + 3) / 4)

#define BASE_OBJ_LOC     7
#define BASE_OBJ_FLAGS   (BASE_OBJ_LOC + OBJ_WORDS)
#define BASE_ROOM_FLAGS  (BASE_OBJ_FLAGS + OBJ_WORDS)
#define BASE_END         (BASE_ROOM_FLAGS + ROOM_WORDS)

/* As of writing, BASE_END = 41 with NUM_OBJECTS=27, NUM_ROOMS=24.
 * Must stay below SAVE_BUF_WORDS (128). If you add enough objects/
 * rooms to threaten that, widen this comment into an actual
 * preprocessor #if check. */

static u16 save_buf[SAVE_BUF_WORDS];

void zork_save(void) {
    u8 i;
    u8 pair;

    for (i = 0; i < SAVE_BUF_WORDS; i++) save_buf[i] = 0;

    save_buf[0] = SAVE_MAGIC_1;
    save_buf[1] = SAVE_MAGIC_2;

    save_buf[2] = (u16)g_player_room;
    save_buf[3] = g_score;
    save_buf[4] = g_moves;
    save_buf[5] = (u16)g_lamp_fuel;
    save_buf[6] = (u16)g_dead | ((u16)(g_troll_alive ? 1 : 0) << 1);

    /* obj_loc: 2 objects per word, even index in high byte */
    for (i = 0; i < NUM_OBJECTS; i++) {
        pair = i >> 1;
        if (i & 1) {
            save_buf[BASE_OBJ_LOC + pair] =
                (save_buf[BASE_OBJ_LOC + pair] & 0xFF00u) | (u16)g_obj_loc[i];
        } else {
            save_buf[BASE_OBJ_LOC + pair] =
                ((u16)g_obj_loc[i] << 8) | (save_buf[BASE_OBJ_LOC + pair] & 0x00FFu);
        }
    }

    /* obj_flags: 2 objects per word */
    for (i = 0; i < NUM_OBJECTS; i++) {
        pair = i >> 1;
        if (i & 1) {
            save_buf[BASE_OBJ_FLAGS + pair] =
                (save_buf[BASE_OBJ_FLAGS + pair] & 0xFF00u) | (u16)g_obj_flags[i];
        } else {
            save_buf[BASE_OBJ_FLAGS + pair] =
                ((u16)g_obj_flags[i] << 8) | (save_buf[BASE_OBJ_FLAGS + pair] & 0x00FFu);
        }
    }

    /* room_flags: 4 rooms per word, 4 bits each (VISITED, DARK, ABOVE, spare) */
    for (i = 0; i < NUM_ROOMS; i++) {
        u8 word_off;
        u8 bit_off;
        u16 mask;
        u16 val;
        word_off = i >> 2;
        bit_off  = (u8)((i & 3) << 2);
        val  = (u16)(g_room_flags[i] & 0x0F) << bit_off;
        mask = (u16)0x000F << bit_off;
        save_buf[BASE_ROOM_FLAGS + word_off] =
            (save_buf[BASE_ROOM_FLAGS + word_off] & (u16)(~mask)) | val;
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
    g_dead        = (u8)(save_buf[6] & 0x01);
    g_troll_alive = (u8)((save_buf[6] >> 1) & 0x01);

    /* obj_loc */
    for (i = 0; i < NUM_OBJECTS; i++) {
        pair = i >> 1;
        word = save_buf[BASE_OBJ_LOC + pair];
        if (i & 1) {
            g_obj_loc[i] = (u8)(word & 0xFF);
        } else {
            g_obj_loc[i] = (u8)(word >> 8);
        }
    }

    /* obj_flags */
    for (i = 0; i < NUM_OBJECTS; i++) {
        pair = i >> 1;
        word = save_buf[BASE_OBJ_FLAGS + pair];
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
        bit_off  = (u8)((i & 3) << 2);
        g_room_flags[i] = (u8)((save_buf[BASE_ROOM_FLAGS + word_off] >> bit_off) & 0x0F);
    }

    text_println("Game restored.");
}
