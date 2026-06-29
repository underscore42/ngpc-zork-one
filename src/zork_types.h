/* zork_types.h - Zork Neo for NGPC
 * Core types, constants, world model
 * MIT License - game content derived from Zork I (c) Infocom, MIT-licensed Nov 2025
 * NGPC port by underscore42 / Studio So Not Kansai
 *
 * cc900 C89 rules:
 *   - no volatile u16* stores
 *   - no signed multiply/divide
 *   - no declarations after statements
 *   - no unsized arrays []
 *   - no const on arrays passed to functions expecting non-const
 *   - sprite tile IDs 0-255 only
 */
#ifndef ZORK_TYPES_H
#define ZORK_TYPES_H

#include "ngpc.h"
#include "library.h"

/* ---- Screen geometry ---- */
#define SCR_W           20      /* tiles wide */
#define SCR_H           19      /* tiles tall */
#define TEXT_ROWS       14      /* rows 0-13: scrolling output */
#define ROW_DIVIDER     14      /* row 14: --- separator */
#define ROW_VERB        15      /* row 15: verb selector */
#define ROW_NOUN        16      /* row 16: noun selector */
#define ROW_CMD         17      /* row 17: assembled command */
#define ROW_STATUS      18      /* row 18: score / moves */

/* ---- Palettes ---- */
#define PAL_NORMAL      0       /* white text */
#define PAL_DIM         1       /* grey - for selector arrows, prompts */
#define PAL_HILITE      2       /* bright - selected word */
#define PAL_CMD         3       /* yellow - command line */
#define PAL_TITLE       4       /* green - room name */
#define PAL_STATUS      5       /* dim blue - status bar */
#define PAL_ERR         6       /* red - error messages */
#define PAL_DIVIDER     7       /* dark - separator line */

/* ---- Input mode ---- */
#define MODE_VERB       0
#define MODE_NOUN       1
#define MODE_CONFIRM    2       /* brief flash before execute */

/* ---- Directions (index into exits[]) ---- */
#define DIR_NORTH       0
#define DIR_SOUTH       1
#define DIR_EAST        2
#define DIR_WEST        3
#define DIR_UP          4
#define DIR_DOWN        5
#define DIR_NONE        0xFF

/* ---- Object flags ---- */
#define OBJ_TAKEABLE    0x01    /* can TAKE */
#define OBJ_CONTAINER   0x02    /* can PUT things IN */
#define OBJ_OPEN        0x04    /* container is open */
#define OBJ_LOCKED      0x08    /* locked container/door */
#define OBJ_WEAPON      0x10    /* can ATTACK with */
#define OBJ_LIGHT       0x20    /* provides light */
#define OBJ_LIT         0x40    /* currently lit */
#define OBJ_WORN        0x80    /* currently worn */

/* ---- Room flags ---- */
#define ROOM_DARK       0x01    /* needs light source */
#define ROOM_VISITED    0x02    /* already visited (show short desc) */
#define ROOM_ABOVE      0x04    /* outdoors / above ground */
#define ROOM_SACRED     0x08    /* no combat */

/* ---- Special room IDs ---- */
#define LOC_PLAYER      0xFF    /* in player inventory */
#define LOC_CARRIED     0xFE    /* worn/held (not in room) */
#define LOC_GONE        0xFD    /* removed from game */
#define NO_EXIT         0xFF    /* no exit in that direction */

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
#define NUM_VERBS       24

/* ---- Object IDs (indices into g_objects[]) ---- */
/* Keep under 64 objects for save-state budget */
#define OBJ_MAILBOX         0
#define OBJ_LEAFLET         1
#define OBJ_DOOR_W          2
#define OBJ_MAT             3
#define OBJ_LANTERN         4
#define OBJ_SWORD           5
#define OBJ_TROPHY_CASE     6
#define OBJ_PAINTING        7
#define OBJ_SACK            8
#define OBJ_GARLIC          9
#define OBJ_BOTTLE          10
#define OBJ_WATER           11
#define OBJ_KNIFE           12
#define OBJ_CANARY          13
#define OBJ_COFFIN          14
#define OBJ_SKULL           15
#define OBJ_ROPE            16
#define OBJ_COAL            17
#define OBJ_TORCH           18
#define OBJ_GOLD_COIN       19
#define OBJ_PLATINUM_BAR    20
#define OBJ_POT_OF_GOLD     21
#define OBJ_JEWELS          22
#define OBJ_DIAMOND         23
#define OBJ_EMERALD         24
#define OBJ_RUBY            25
#define OBJ_SCREWDRIVER     26
#define OBJ_PUMP            27
#define OBJ_AIR_PUMP        28
#define OBJ_CRYSTAL_SKULL   29
#define NUM_OBJECTS         30

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
#define ROOM_GRATING_CLEARING   19
#define ROOM_FOREST_1           20
#define ROOM_FOREST_2           21
#define ROOM_FOREST_PATH        22
#define ROOM_CLEARING           23
#define NUM_ROOMS               24

/* ---- Max sizes ---- */
#define MAX_SCROLL_LINES    128     /* ring buffer for output history */
#define MAX_LINE_LEN        20      /* chars per display line */
#define MAX_NOUN_LIST       12      /* nouns shown in selector */
#define MAX_INVENTORY       12      /* player can carry this many */

/* ---- Save/Load ---- */
#define SAVE_MAGIC_1        0xA5A5
#define SAVE_MAGIC_2        0x5A5A
/* Save block: 128 u16s = 256 bytes (flash block requirement)
 * Layout:
 *  [0]  magic1
 *  [1]  magic2
 *  [2]  player_room
 *  [3]  score
 *  [4]  moves (low)
 *  [5]  moves (high)
 *  [6]  obj_location[0..7]  packed 2 per word (high/low byte)
 *  ...  continues packing all NUM_OBJECTS locations
 *  [21] obj_flags[0..7]     packed 2 per word
 *  ...  continues packing all NUM_OBJECTS flags
 *  [36] room_flags[0..3]    packed 4 per word (2 bits each)
 *  ...  room_flags for all NUM_ROOMS
 *  [48] lamp_fuel (turns remaining)
 *  [49] dead_flag
 *  [50..127] = 0 (reserved)
 */
#define SAVE_BUF_WORDS      128

/* ---- Structs ---- */

typedef struct {
    char *short_name;             /* e.g. "West of House" */
    char *description;            /* long description */
    u8          exits[6];               /* indexed by DIR_* */
    u8          flags;                  /* ROOM_* bitmask */
} Room;

typedef struct {
    char *name;                   /* e.g. "mailbox" */
    char *description;            /* examine text */
    u8          location;               /* room ID, LOC_PLAYER, LOC_GONE */
    u8          flags;                  /* OBJ_* bitmask */
    u8          score_value;            /* points when deposited in trophy case */
    u8          container_id;           /* if PUT IN: target container obj ID */
} Object;

/* ---- Global game state (all mutable, lives in work RAM) ---- */
extern u8   g_player_room;
extern u16  g_score;
extern u16  g_moves;
extern u8   g_lamp_fuel;        /* turns of light remaining */
extern u8   g_dead;
extern u8   g_input_mode;       /* MODE_VERB / MODE_NOUN / MODE_CONFIRM */
extern u8   g_verb_idx;         /* current verb selection */
extern u8   g_noun_idx;         /* current noun selection in g_noun_list */
extern u8   g_noun_count;       /* number of nouns available */
extern u8   g_noun_list[MAX_NOUN_LIST]; /* obj IDs available at this verb */
extern u8   g_last_verb;        /* for AGAIN */
extern u8   g_last_noun;
extern u8   g_pad_cur;
extern u8   g_pad_prev;
extern u8   g_pad_press;

/* Object mutable state (location + flags) stored separately
 * from const ROM data so we can flash-save just these arrays */
extern u8   g_obj_loc[NUM_OBJECTS];
extern u8   g_obj_flags[NUM_OBJECTS];
extern u8   g_room_flags[NUM_ROOMS];

/* Text output ring buffer */
extern u8   g_scroll_top;       /* first visible line in TEXT_ROWS window */
extern u8   g_scroll_count;     /* total lines in buffer */
/* Each line is MAX_LINE_LEN+1 bytes. Stored as flat array. */
extern char g_lines[MAX_SCROLL_LINES][MAX_LINE_LEN + 1];

/* ROM data */
extern Room   g_rooms[NUM_ROOMS];
extern Object g_objects[NUM_OBJECTS];
extern char *g_verb_names[NUM_VERBS];

#endif /* ZORK_TYPES_H */
