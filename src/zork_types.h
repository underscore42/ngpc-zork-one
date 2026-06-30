/* zork_types.h - Zork Neo for NGPC
 * Flat arrays only - no structs with pointer members (cc900 unsafe)
 * cc900 C89: no const arrays, no structs with pointers, no signed mul/div
 */
#ifndef ZORK_TYPES_H
#define ZORK_TYPES_H

#include "ngpc.h"
#include "library.h"

/* ---- Screen geometry ---- */
#define SCR_W           20
#define SCR_H           19
#define TEXT_ROWS       14
#define ROW_DIVIDER     14
#define ROW_VERB        15
#define ROW_NOUN        16
#define ROW_CMD         17
#define ROW_STATUS      18

/* ---- Palettes ---- */
#define PAL_NORMAL      0
#define PAL_DIM         1
#define PAL_HILITE      2
#define PAL_CMD         3
#define PAL_TITLE       4
#define PAL_STATUS      5
#define PAL_ERR         6
#define PAL_DIVIDER     7

/* ---- Input mode ---- */
#define MODE_VERB       0
#define MODE_NOUN       1

/* ---- Directions ---- */
#define DIR_NORTH       0
#define DIR_SOUTH       1
#define DIR_EAST        2
#define DIR_WEST        3
#define DIR_UP          4
#define DIR_DOWN        5
#define NO_EXIT         0xFF

/* ---- Object flags ---- */
#define OBJ_TAKEABLE    0x01
#define OBJ_CONTAINER   0x02
#define OBJ_OPEN        0x04
#define OBJ_LOCKED      0x08
#define OBJ_WEAPON      0x10
#define OBJ_LIGHT       0x20
#define OBJ_LIT         0x40
#define OBJ_WORN        0x80

/* ---- Room flags ---- */
#define ROOM_DARK       0x01
#define ROOM_VISITED    0x02
#define ROOM_ABOVE      0x04

/* ---- Special locations ---- */
#define LOC_PLAYER      0xFE
#define LOC_GONE        0xFF
#define NO_CONTAINER    0xFF

/* ---- Verb IDs ---- */
#define V_LOOK          0
#define V_GO            1
#define V_TAKE          2
#define V_DROP          3
#define V_EXAMINE       4
#define V_OPEN          5
#define V_CLOSE         6
#define V_READ          7
#define V_PUT           8
#define V_ATTACK        9
#define V_INVENTORY     10
#define V_TURN_ON       11
#define V_TURN_OFF      12
#define V_WAIT          13
#define V_AGAIN         14
#define V_NORTH         15
#define V_SOUTH         16
#define V_EAST          17
#define V_WEST          18
#define V_UP            19
#define V_DOWN          20
#define V_SAVE          21
#define V_RESTORE       22
#define V_QUIT          23
#define V_ENTER         24
#define V_CLIMB         25
#define V_MOVE          26
#define V_GET           27
#define V_DROP2         28
#define NUM_VERBS       29

/* ---- Object IDs ---- */
#define OBJ_MAILBOX         0
#define OBJ_LEAFLET         1
#define OBJ_MAT             2
#define OBJ_LANTERN         3
#define OBJ_SWORD           4
#define OBJ_TROPHY_CASE     5
#define OBJ_PAINTING        6
#define OBJ_SACK            7
#define OBJ_GARLIC          8
#define OBJ_BOTTLE          9
#define OBJ_KNIFE           10
#define OBJ_CANARY          11
#define OBJ_ROPE            12
#define OBJ_COAL            13
#define OBJ_TORCH           14
#define OBJ_GOLD_COIN       15
#define OBJ_PLATINUM_BAR    16
#define OBJ_POT_OF_GOLD     17
#define OBJ_JEWELS          18
#define OBJ_DIAMOND         19
#define OBJ_EMERALD         20
#define OBJ_RUBY            21
#define OBJ_SCREWDRIVER     22
#define OBJ_WINDOW          23
#define OBJ_RUG             24
#define OBJ_TRAP_DOOR       25
#define OBJ_TROLL           26
#define NUM_OBJECTS         27

/* ---- Room IDs ---- */
#define ROOM_WEST_OF_HOUSE      0
#define ROOM_NORTH_OF_HOUSE     1
#define ROOM_SOUTH_OF_HOUSE     2
#define ROOM_BEHIND_HOUSE       3
#define ROOM_KITCHEN            4
#define ROOM_LIVING_ROOM        5
#define ROOM_ATTIC              6
#define ROOM_CELLAR             7
#define ROOM_EAST_PASSAGE       8
#define ROOM_ROUND_ROOM         9
#define ROOM_NORTH_SOUTH_PASS   10
#define ROOM_EW_PASSAGE         11
#define ROOM_LOUD_ROOM          12
#define ROOM_GALLERY            13
#define ROOM_STUDIO             14
#define ROOM_CAVE               15
#define ROOM_MAZE_1             16
#define ROOM_MAZE_2             17
#define ROOM_MAZE_3             18
#define ROOM_CLEARING           19
#define ROOM_FOREST_1           20
#define ROOM_FOREST_2           21
#define ROOM_GRATING_CLEARING   22
#define ROOM_TROLL_ROOM         23
#define NUM_ROOMS               24

/* ---- Direction noun IDs (for GO verb noun list) ---- */
#define DIR_NOUN_BASE   0xF0
#define DIR_NOUN_N      0xF0
#define DIR_NOUN_S      0xF1
#define DIR_NOUN_E      0xF2
#define DIR_NOUN_W      0xF3
#define DIR_NOUN_U      0xF4
#define DIR_NOUN_D      0xF5

/* ---- Direction names (for selector display) ---- */
extern const char g_dir_names[6][6];

/* ---- Limits ---- */
#define MAX_SCROLL_LINES    64
#define MAX_LINE_LEN        20
#define MAX_NOUN_LIST       12
#define MAX_INVENTORY       12

/* ---- Save ---- */
#define SAVE_MAGIC_1        0xA5A5
#define SAVE_MAGIC_2        0x5A5A
#define SAVE_BUF_WORDS      128

/* ---- World data (flat arrays, defined in world.c) ---- */
/* Room names - short */
extern const char g_room_name[NUM_ROOMS][20];
/* Room descriptions - broken into lines of <=20 chars each,
   stored as single string with \n separators */
/* g_room_desc removed - use engine strings directly */
/* Room exits: [room*6 + dir] */
extern const u8    g_room_exits[NUM_ROOMS * 6];
/* Room base flags (ROM - never changes) */
extern const u8    g_room_base_flags[NUM_ROOMS];

/* Object names */
extern const char g_obj_name[NUM_OBJECTS][16];
/* Object examine text */
/* g_obj_desc removed */
/* Object default location */
extern const u8    g_obj_default_loc[NUM_OBJECTS];
/* Object default flags */
extern const u8    g_obj_default_flags[NUM_OBJECTS];
/* Object score value when deposited */
extern const u8    g_obj_score[NUM_OBJECTS];
/* Object target container for scoring */
extern const u8    g_obj_target[NUM_OBJECTS];

/* Verb names */
extern const char g_verb_names[NUM_VERBS][12];

/* ---- Mutable game state (RAM) ---- */
extern u8   g_player_room;
extern u16  g_score;
extern u16  g_moves;
extern u8   g_lamp_fuel;
extern u8   g_dead;
extern u8   g_troll_alive;
extern u8   g_obj_loc[NUM_OBJECTS];
extern u8   g_obj_flags[NUM_OBJECTS];
extern u8   g_room_flags[NUM_ROOMS];

/* ---- Parser state ---- */
extern u8   g_input_mode;
extern u8   g_verb_idx;
extern u8   g_noun_idx;
extern u8   g_noun_count;
extern u8   g_noun_list[MAX_NOUN_LIST];
extern u8   g_last_verb;
extern u8   g_last_noun;
extern u8   g_pad_cur;
extern u8   g_pad_prev;
extern u8   g_pad_press;

/* ---- Text buffer ---- */
extern u8   g_scroll_top;
extern u8   g_scroll_count;
extern char g_lines[MAX_SCROLL_LINES][MAX_LINE_LEN + 1];

#endif
