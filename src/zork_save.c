/* zork_save.c - Zork Neo: Flash save/load
 *
 * 256 u16s (512 bytes) packed layout:
 *   [0]   SAVE_MAGIC_1  (0xA5A5)
 *   [1]   SAVE_MAGIC_2  (0x5A5A)
 *   [2]   player_room
 *   [3]   score
 *   [4]   moves
 *   [5]   lamp_fuel
 *   [6]   dead | (troll_alive<<1) | (thief_alive<<2) | (cyclops_here<<3)
 *   [7..67]   obj_loc   (122 objects, 2 per word, hi=even idx)
 *   [68..128] obj_flags (122 objects, 2 per word)
 *   [129..135] room_visited (110 rooms, 1 bit per room, 16 per word)
 *   [136..255] reserved
 *
 * Room ABOVE/DARK flags are NOT saved - restored from g_room_base_flags at init.
 * Only ROOM_VISITED bit needs persistence.
 */
#include "zork_save.h"
#include "engine.h"
#include "text.h"

#define OBJ_WORDS         ((NUM_OBJECTS + 1) / 2)
#define ROOM_VISIT_WORDS  ((NUM_ROOMS + 15) / 16)

#define BASE_OBJ_LOC      7
#define BASE_OBJ_FLAGS    (BASE_OBJ_LOC + OBJ_WORDS)
#define BASE_ROOM_VISITED (BASE_OBJ_FLAGS + OBJ_WORDS)
#define BASE_END          (BASE_ROOM_VISITED + ROOM_VISIT_WORDS)

/* BASE_END = 136 with 122 objects, 110 rooms. Must stay < SAVE_BUF_WORDS (256). */

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
    save_buf[6] = (u16)g_dead
                | ((u16)(g_troll_alive  ? 1 : 0) << 1)
                | ((u16)(g_thief_alive  ? 1 : 0) << 2)
                | ((u16)(g_cyclops_here ? 1 : 0) << 3);

    /* obj_loc: 2 per word, even index in high byte */
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

    /* obj_flags: 2 per word */
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

    /* room visited: 1 bit per room, 16 rooms per word */
    for (i = 0; i < NUM_ROOMS; i++) {
        u8 word_off;
        u8 bit_off;
        word_off = i >> 4;
        bit_off  = i & 15;
        if (g_room_flags[i] & ROOM_VISITED) {
            save_buf[BASE_ROOM_VISITED + word_off] |= (u16)(1 << bit_off);
        }
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

    g_player_room  = (u8)save_buf[2];
    g_score        = save_buf[3];
    g_moves        = save_buf[4];
    g_lamp_fuel    = (u8)save_buf[5];
    g_dead         = (u8)(save_buf[6] & 0x01);
    g_troll_alive  = (u8)((save_buf[6] >> 1) & 0x01);
    g_thief_alive  = (u8)((save_buf[6] >> 2) & 0x01);
    g_cyclops_here = (u8)((save_buf[6] >> 3) & 0x01);

    for (i = 0; i < NUM_OBJECTS; i++) {
        pair = i >> 1;
        word = save_buf[BASE_OBJ_LOC + pair];
        g_obj_loc[i] = (u8)((i & 1) ? (word & 0xFF) : (word >> 8));
    }

    for (i = 0; i < NUM_OBJECTS; i++) {
        pair = i >> 1;
        word = save_buf[BASE_OBJ_FLAGS + pair];
        g_obj_flags[i] = (u8)((i & 1) ? (word & 0xFF) : (word >> 8));
    }

    /* Restore room flags: start from base flags (ABOVE/DARK from ROM)
     * then apply saved VISITED bits on top */
    for (i = 0; i < NUM_ROOMS; i++) {
        u8 word_off;
        u8 bit_off;
        word_off = i >> 4;
        bit_off  = i & 15;
        g_room_flags[i] = g_room_base_flags[i];
        if (save_buf[BASE_ROOM_VISITED + word_off] & (u16)(1 << bit_off)) {
            g_room_flags[i] |= ROOM_VISITED;
        }
    }
    /* Re-apply runtime lit overrides */
    g_room_flags[ROOM_KITCHEN]     |= ROOM_ABOVE;
    g_room_flags[ROOM_LIVING_ROOM] |= ROOM_ABOVE;
    g_room_flags[ROOM_TORCH_ROOM]  |= ROOM_ABOVE;

    text_println("Game restored.");
}
