/* zork_types.h - Zork Neo for NGPC
 * AUTO-GENERATED room/object IDs from historicalsource/zork1 (MIT)
 * cc900 C89: no const char*[], no structs with pointers, no signed mul/div
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

/* ---- Directions (index into g_room_exits[room*NUM_DIRS+dir]) ---- */
#define DIR_NORTH       0
#define DIR_SOUTH       1
#define DIR_EAST        2
#define DIR_WEST        3
#define DIR_NE          4
#define DIR_NW          5
#define DIR_SE          6
#define DIR_SW          7
#define DIR_UP          8
#define DIR_DOWN        9
#define NUM_DIRS        10
#define NO_EXIT         0xFF

/* ---- Object flags ---- */
#define OBJ_TAKEABLE    0x01
#define OBJ_CONTAINER   0x02
#define OBJ_OPEN        0x04
#define OBJ_LOCKED      0x08
#define OBJ_WEAPON      0x10
#define OBJ_LIGHT       0x20
#define OBJ_LIT         0x40
#define OBJ_NODESC      0x80  /* do not list in room description */

/* ---- Room flags ---- */
#define ROOM_DARK       0x01
#define ROOM_VISITED    0x02
#define ROOM_ABOVE      0x04

/* ---- Special locations ---- */
#define LOC_PLAYER      0xFE
#define LOC_GONE        0xFF
#define NO_CONTAINER    0xFF

/* ---- Direction noun IDs (for GO verb) ---- */
#define DIR_NOUN_BASE   0xF0
extern const char g_dir_names[NUM_DIRS][6];

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
#define V_DIG           28
#define V_TIE           29
#define V_WAVE          30
#define V_RING          31
#define V_LIGHT         32
#define V_LOWER         33
#define V_RAISE         34
#define V_INFLATE       35
#define V_PRAY          36
#define V_ECHO          37
#define V_CROSS         38
#define NUM_VERBS       39

/* ---- Room IDs (auto-generated from ZIL source) ---- */
#define ROOM_WEST_OF_HOUSE                            0
#define ROOM_STONE_BARROW                             1
#define ROOM_NORTH_OF_HOUSE                           2
#define ROOM_SOUTH_OF_HOUSE                           3
#define ROOM_EAST_OF_HOUSE                            4
#define ROOM_FOREST_1                                 5
#define ROOM_FOREST_2                                 6
#define ROOM_MOUNTAINS                                7
#define ROOM_FOREST_3                                 8
#define ROOM_PATH                                     9
#define ROOM_UP_A_TREE                                10
#define ROOM_GRATING_CLEARING                         11
#define ROOM_CLEARING                                 12
#define ROOM_KITCHEN                                  13
#define ROOM_ATTIC                                    14
#define ROOM_LIVING_ROOM                              15
#define ROOM_CELLAR                                   16
#define ROOM_TROLL_ROOM                               17
#define ROOM_EAST_OF_CHASM                            18
#define ROOM_GALLERY                                  19
#define ROOM_STUDIO                                   20
#define ROOM_MAZE_1                                   21
#define ROOM_MAZE_2                                   22
#define ROOM_MAZE_3                                   23
#define ROOM_MAZE_4                                   24
#define ROOM_DEAD_END_1                               25
#define ROOM_MAZE_5                                   26
#define ROOM_DEAD_END_2                               27
#define ROOM_MAZE_6                                   28
#define ROOM_MAZE_7                                   29
#define ROOM_MAZE_8                                   30
#define ROOM_DEAD_END_3                               31
#define ROOM_MAZE_9                                   32
#define ROOM_MAZE_10                                  33
#define ROOM_MAZE_11                                  34
#define ROOM_GRATING_ROOM                             35
#define ROOM_MAZE_12                                  36
#define ROOM_DEAD_END_4                               37
#define ROOM_MAZE_13                                  38
#define ROOM_MAZE_14                                  39
#define ROOM_MAZE_15                                  40
#define ROOM_CYCLOPS_ROOM                             41
#define ROOM_STRANGE_PASSAGE                          42
#define ROOM_TREASURE_ROOM                            43
#define ROOM_RESERVOIR_SOUTH                          44
#define ROOM_RESERVOIR                                45
#define ROOM_RESERVOIR_NORTH                          46
#define ROOM_STREAM_VIEW                              47
#define ROOM_IN_STREAM                                48
#define ROOM_MIRROR_ROOM_1                            49
#define ROOM_MIRROR_ROOM_2                            50
#define ROOM_SMALL_CAVE                               51
#define ROOM_TINY_CAVE                                52
#define ROOM_COLD_PASSAGE                             53
#define ROOM_NARROW_PASSAGE                           54
#define ROOM_WINDING_PASSAGE                          55
#define ROOM_TWISTING_PASSAGE                         56
#define ROOM_ATLANTIS_ROOM                            57
#define ROOM_EW_PASSAGE                               58
#define ROOM_ROUND_ROOM                               59
#define ROOM_DEEP_CANYON                              60
#define ROOM_DAMP_CAVE                                61
#define ROOM_LOUD_ROOM                                62
#define ROOM_NS_PASSAGE                               63
#define ROOM_CHASM_ROOM                               64
#define ROOM_ENTRANCE_TO_HADES                        65
#define ROOM_LAND_OF_LIVING_DEAD                      66
#define ROOM_ENGRAVINGS_CAVE                          67
#define ROOM_EGYPT_ROOM                               68
#define ROOM_DOME_ROOM                                69
#define ROOM_TORCH_ROOM                               70
#define ROOM_NORTH_TEMPLE                             71
#define ROOM_SOUTH_TEMPLE                             72
#define ROOM_DAM_ROOM                                 73
#define ROOM_DAM_LOBBY                                74
#define ROOM_MAINTENANCE_ROOM                         75
#define ROOM_DAM_BASE                                 76
#define ROOM_RIVER_1                                  77
#define ROOM_RIVER_2                                  78
#define ROOM_RIVER_3                                  79
#define ROOM_WHITE_CLIFFS_NORTH                       80
#define ROOM_WHITE_CLIFFS_SOUTH                       81
#define ROOM_RIVER_4                                  82
#define ROOM_RIVER_5                                  83
#define ROOM_SHORE                                    84
#define ROOM_SANDY_BEACH                              85
#define ROOM_SANDY_CAVE                               86
#define ROOM_ARAGAIN_FALLS                            87
#define ROOM_ON_RAINBOW                               88
#define ROOM_END_OF_RAINBOW                           89
#define ROOM_CANYON_BOTTOM                            90
#define ROOM_CLIFF_MIDDLE                             91
#define ROOM_CANYON_VIEW                              92
#define ROOM_MINE_ENTRANCE                            93
#define ROOM_SQUEEKY_ROOM                             94
#define ROOM_BAT_ROOM                                 95
#define ROOM_SHAFT_ROOM                               96
#define ROOM_SMELLY_ROOM                              97
#define ROOM_GAS_ROOM                                 98
#define ROOM_LADDER_TOP                               99
#define ROOM_LADDER_BOTTOM                            100
#define ROOM_DEAD_END_5                               101
#define ROOM_TIMBER_ROOM                              102
#define ROOM_LOWER_SHAFT                              103
#define ROOM_MACHINE_ROOM                             104
#define ROOM_MINE_1                                   105
#define ROOM_MINE_2                                   106
#define ROOM_MINE_3                                   107
#define ROOM_MINE_4                                   108
#define ROOM_SLIDE_ROOM                               109
#define NUM_ROOMS                                     110

/* ---- Object IDs (auto-generated from ZIL source) ---- */
#define OBJ_BOARD                                     0
#define OBJ_TEETH                                     1
#define OBJ_WALL                                      2
#define OBJ_GRANITE_WALL                              3
#define OBJ_SONGBIRD                                  4
#define OBJ_WHITE_HOUSE                               5
#define OBJ_FOREST                                    6
#define OBJ_TREE                                      7
#define OBJ_MOUNTAIN_RANGE                            8
#define OBJ_GLOBAL_WATER                              9
#define OBJ_WATER                                     10
#define OBJ_KITCHEN_WINDOW                            11
#define OBJ_CHIMNEY                                   12
#define OBJ_GHOSTS                                    13
#define OBJ_SKULL                                     14
#define OBJ_LOWERED_BASKET                            15
#define OBJ_RAISED_BASKET                             16
#define OBJ_LUNCH                                     17
#define OBJ_BAT                                       18
#define OBJ_BELL                                      19
#define OBJ_HOT_BELL                                  20
#define OBJ_AXE                                       21
#define OBJ_BOLT                                      22
#define OBJ_BUBBLE                                    23
#define OBJ_ALTAR                                     24
#define OBJ_BOOK                                      25
#define OBJ_BROKEN_LAMP                               26
#define OBJ_SCEPTRE                                   27
#define OBJ_TIMBERS                                   28
#define OBJ_SLIDE                                     29
#define OBJ_KITCHEN_TABLE                             30
#define OBJ_ATTIC_TABLE                               31
#define OBJ_SANDWICH_BAG                              32
#define OBJ_TOOL_CHEST                                33
#define OBJ_YELLOW_BUTTON                             34
#define OBJ_BROWN_BUTTON                              35
#define OBJ_RED_BUTTON                                36
#define OBJ_BLUE_BUTTON                               37
#define OBJ_TROPHY_CASE                               38
#define OBJ_RUG                                       39
#define OBJ_CHALICE                                   40
#define OBJ_GARLIC                                    41
#define OBJ_TRIDENT                                   42
#define OBJ_CYCLOPS                                   43
#define OBJ_DAM                                       44
#define OBJ_TRAP_DOOR                                 45
#define OBJ_BOARDED_WINDOW                            46
#define OBJ_FRONT_DOOR                                47
#define OBJ_BARROW_DOOR                               48
#define OBJ_BARROW                                    49
#define OBJ_BOTTLE                                    50
#define OBJ_CRACK                                     51
#define OBJ_COFFIN                                    52
#define OBJ_GRATE                                     53
#define OBJ_PUMP                                      54
#define OBJ_DIAMOND                                   55
#define OBJ_JADE                                      56
#define OBJ_KNIFE                                     57
#define OBJ_BONES                                     58
#define OBJ_BURNED_OUT_LANTERN                        59
#define OBJ_BAG_OF_COINS                              60
#define OBJ_LAMP                                      61
#define OBJ_EMERALD                                   62
#define OBJ_ADVERTISEMENT                             63
#define OBJ_LEAK                                      64
#define OBJ_MACHINE                                   65
#define OBJ_INFLATED_BOAT                             66
#define OBJ_MAILBOX                                   67
#define OBJ_MATCH                                     68
#define OBJ_MIRROR_2                                  69
#define OBJ_MIRROR_1                                  70
#define OBJ_PAINTING                                  71
#define OBJ_CANDLES                                   72
#define OBJ_GUNK                                      73
#define OBJ_BODIES                                    74
#define OBJ_LEAVES                                    75
#define OBJ_PUNCTURED_BOAT                            76
#define OBJ_INFLATABLE_BOAT                           77
#define OBJ_BAR                                       78
#define OBJ_POT_OF_GOLD                               79
#define OBJ_PRAYER                                    80
#define OBJ_RAILING                                   81
#define OBJ_RAINBOW                                   82
#define OBJ_RIVER                                     83
#define OBJ_BUOY                                      84
#define OBJ_ROPE                                      85
#define OBJ_RUSTY_KNIFE                               86
#define OBJ_SAND                                      87
#define OBJ_BRACELET                                  88
#define OBJ_SCREWDRIVER                               89
#define OBJ_KEYS                                      90
#define OBJ_SHOVEL                                    91
#define OBJ_COAL                                      92
#define OBJ_LADDER                                    93
#define OBJ_SCARAB                                    94
#define OBJ_LARGE_BAG                                 95
#define OBJ_STILETTO                                  96
#define OBJ_MACHINE_SWITCH                            97
#define OBJ_WOODEN_DOOR                               98
#define OBJ_SWORD                                     99
#define OBJ_MAP                                       100
#define OBJ_BOAT_LABEL                                101
#define OBJ_THIEF                                     102
#define OBJ_PEDESTAL                                  103
#define OBJ_TORCH                                     104
#define OBJ_GUIDE                                     105
#define OBJ_TROLL                                     106
#define OBJ_TRUNK                                     107
#define OBJ_TUBE                                      108
#define OBJ_PUTTY                                     109
#define OBJ_ENGRAVINGS                                110
#define OBJ_OWNERS_MANUAL                             111
#define OBJ_CLIMBABLE_CLIFF                           112
#define OBJ_WHITE_CLIFF                               113
#define OBJ_WRENCH                                    114
#define OBJ_CONTROL_PANEL                             115
#define OBJ_NEST                                      116
#define OBJ_EGG                                       117
#define OBJ_BROKEN_EGG                                118
#define OBJ_BAUBLE                                    119
#define OBJ_CANARY                                    120
#define OBJ_BROKEN_CANARY                             121
#define NUM_OBJECTS                                   122

/* ---- Limits ---- */
#define MAX_SCROLL_LINES    64
#define MAX_LINE_LEN        20
#define MAX_NOUN_LIST       16
#define MAX_INVENTORY       16

/* ---- Save ---- */
#define SAVE_MAGIC_1        0xA5A5
#define SAVE_MAGIC_2        0x5A5A
#define SAVE_BUF_WORDS      256

/* ---- World data (ROM arrays, defined in world.c) ---- */
extern const char g_room_name[NUM_ROOMS][20];
extern const u8   g_room_exits[NUM_ROOMS * NUM_DIRS];
extern const u8   g_room_base_flags[NUM_ROOMS];
extern const char g_obj_name[NUM_OBJECTS][16];
extern const u8   g_obj_default_loc[NUM_OBJECTS];
extern const u8   g_obj_default_flags[NUM_OBJECTS];
extern const u8   g_obj_score[NUM_OBJECTS];
extern const u8   g_obj_target[NUM_OBJECTS];
extern const char g_verb_names[NUM_VERBS][12];

/* ---- Mutable game state (RAM, defined in engine.c) ---- */
extern u8   g_player_room;
extern u16  g_score;
extern u16  g_moves;
extern u8   g_lamp_fuel;
extern u8   g_dead;
extern u8   g_troll_alive;
extern u8   g_thief_alive;
extern u8   g_cyclops_here;
extern u8   g_obj_loc[NUM_OBJECTS];
extern u8   g_obj_flags[NUM_OBJECTS];
extern u8   g_room_flags[NUM_ROOMS];

/* ---- Parser state (defined in parser.c) ---- */
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

/* ---- Text buffer (defined in text.c) ---- */
extern u8   g_scroll_top;
extern u8   g_scroll_count;
extern char g_lines[MAX_SCROLL_LINES][MAX_LINE_LEN + 1];

#endif /* ZORK_TYPES_H */
