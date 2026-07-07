/* zork_save.c - Zork Neo: Flash save/load
 *
 * 256 u16s (512 bytes) packed layout - written whole by Flash() (rbc3=2):
 *   [0]   0xBABE  \ library MAGIC_NB (0xCAFEBABE) as little-endian u32.
 *   [1]   0xCAFE  / REQUIRED: GetSavedData() rejects flash whose first
 *                   u32 != MAGIC_NB and returns an empty buffer instead.
 *   [2]   ZORK_MAGIC (secondary marker + layout version - distinguishes a
 *         real save from the synthetic empty buffer, which also has MAGIC_NB)
 *   [3]   player_room | thief_room << 8
 *   [4]   score
 *   [5]   moves
 *   [6]   lamp_fuel | candle_fuel << 8
 *   [7]   state bits (see save/load below)
 *   [8..68]    obj_loc   (122 objects, 2 per word, hi=even idx)
 *   [69..129]  obj_flags (122 objects, 2 per word)
 *   [130..136] room_visited (110 rooms, 1 bit per room, 16 per word)
 *   [137..255] reserved (zero)
 *
 * Room ABOVE/DARK flags are NOT saved - restored from g_room_base_flags.
 */
#include "zork_save.h"
#include "engine.h"
#include "text.h"
#include "thief.h"

#define ZORK_MAGIC        0x5A02   /* 'Z' + layout version 2 */

#define OBJ_WORDS         ((NUM_OBJECTS + 1) / 2)
#define ROOM_VISIT_WORDS  ((NUM_ROOMS + 15) / 16)

#define BASE_OBJ_LOC      8
#define BASE_OBJ_FLAGS    (BASE_OBJ_LOC + OBJ_WORDS)
#define BASE_ROOM_VISITED (BASE_OBJ_FLAGS + OBJ_WORDS)
#define BASE_END          (BASE_ROOM_VISITED + ROOM_VISIT_WORDS)

/* BASE_END = 137 with 122 objects, 110 rooms. Must stay <= SAVE_BUF_WORDS (256). */

static u16 save_buf[SAVE_BUF_WORDS];

void zork_save(void) {
    u8 i;
    u8 pair;
    u16 w;

    for (w = 0; w < SAVE_BUF_WORDS; w++) save_buf[w] = 0;
    /* w is u16: a u8 index wraps 255->0 and never terminates at 256 */

    /* Library magic MUST be the first u32 or GetSavedData() will
     * report "no save" forever. 0xCAFEBABE little-endian. */
    save_buf[0] = 0xBABE;
    save_buf[1] = 0xCAFE;
    save_buf[2] = ZORK_MAGIC;
    save_buf[3] = (u16)g_player_room | ((u16)g_thief_room << 8);
    save_buf[4] = g_score;
    save_buf[5] = g_moves;
    save_buf[6] = (u16)g_lamp_fuel | ((u16)g_candle_fuel << 8);
    save_buf[7] = (u16)g_dead
                | ((u16)(g_troll_alive   ? 1 : 0) << 1)
                | ((u16)(g_thief_alive   ? 1 : 0) << 2)
                | ((u16)(g_cyclops_here  ? 1 : 0) << 3)
                | ((u16)(g_candles_dead  ? 1 : 0) << 4)
                | ((u16)(g_in_boat       ? 1 : 0) << 5)
                | ((u16)(g_bell_rung     ? 1 : 0) << 6)
                | ((u16)(g_ghosts_gone   ? 1 : 0) << 7)
                | ((u16)(g_won           ? 1 : 0) << 8)
                | ((u16)(g_knows_word    ? 1 : 0) << 9)
                | ((u16)(g_rope_tied     ? 1 : 0) << 10)
                | ((u16)(g_rainbow_solid ? 1 : 0) << 11)
                | ((u16)(g_lamp_dead     ? 1 : 0) << 12)
                | ((u16)(g_thief_wounded & 3) << 13);

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

    /* GetSavedData's "no save" path also fills MAGIC_NB, so the library
     * magic alone proves nothing - the secondary marker decides. */
    if (save_buf[0] != 0xBABE || save_buf[1] != 0xCAFE
        || save_buf[2] != ZORK_MAGIC) {
        text_println("No saved game found.");
        return;
    }

    g_player_room  = (u8)(save_buf[3] & 0xFF);
    g_thief_room   = (u8)(save_buf[3] >> 8);
    g_score        = save_buf[4];
    g_moves        = save_buf[5];
    g_lamp_fuel    = (u8)(save_buf[6] & 0xFF);
    g_candle_fuel  = (u8)(save_buf[6] >> 8);

    g_dead          = (u8)(save_buf[7] & 0x01);
    g_troll_alive   = (u8)((save_buf[7] >> 1) & 0x01);
    g_thief_alive   = (u8)((save_buf[7] >> 2) & 0x01);
    g_cyclops_here  = (u8)((save_buf[7] >> 3) & 0x01);
    g_candles_dead  = (u8)((save_buf[7] >> 4) & 0x01);
    g_in_boat       = (u8)((save_buf[7] >> 5) & 0x01);
    g_bell_rung     = (u8)((save_buf[7] >> 6) & 0x01);
    g_ghosts_gone   = (u8)((save_buf[7] >> 7) & 0x01);
    g_won           = (u8)((save_buf[7] >> 8) & 0x01);
    g_knows_word    = (u8)((save_buf[7] >> 9) & 0x01);
    g_rope_tied     = (u8)((save_buf[7] >> 10) & 0x01);
    g_rainbow_solid = (u8)((save_buf[7] >> 11) & 0x01);
    g_lamp_dead     = (u8)((save_buf[7] >> 12) & 0x01);
    g_thief_wounded = (u8)((save_buf[7] >> 13) & 0x03);

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
