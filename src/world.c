/* world.c - Zork Neo: World data
 * Fixed-width char arrays only - no char* pointer arrays (cc900 unsafe)
 * MIT License - Zork I (c) Infocom, open sourced by Microsoft 2025
 */
#include "zork_types.h"

/* Verb names: 12 chars max, fixed width */
const char g_verb_names[NUM_VERBS][12] = {
    "LOOK",      "GO",        "TAKE",      "DROP",
    "EXAMINE",   "OPEN",      "CLOSE",     "READ",
    "PUT",       "ATTACK",    "INVENTORY", "TURN ON",
    "TURN OFF",  "WAIT",      "AGAIN",     "NORTH",
    "SOUTH",     "EAST",      "WEST",      "UP",
    "DOWN",      "SAVE",      "RESTORE",   "QUIT",
    "ENTER",     "CLIMB",     "MOVE"
};

/* Room short names: 20 chars max */
const char g_room_name[NUM_ROOMS][20] = {
    "West of House",
    "North of House",
    "South of House",
    "Behind House",
    "Kitchen",
    "Living Room",
    "Attic",
    "Cellar",
    "East Passage",
    "Round Room",
    "N/S Passage",
    "E/W Passage",
    "Loud Room",
    "Gallery",
    "Studio",
    "Cave",
    "Maze",
    "Maze",
    "Maze",
    "Clearing",
    "Forest",
    "Forest",
    "Grating Clearing"
};

/* Direction names for GO noun selector */
const char g_dir_names[6][6] = {
    "NORTH", "SOUTH", "EAST", "WEST", "UP", "DOWN"
};

/* Object names: 16 chars max */
const char g_obj_name[NUM_OBJECTS][16] = {
    "mailbox",
    "leaflet",
    "mat",
    "brass lantern",
    "elvish sword",
    "trophy case",
    "painting",
    "brown sack",
    "garlic",
    "glass bottle",
    "rusty knife",
    "golden canary",
    "coil of rope",
    "lump of coal",
    "torch",
    "gold coin",
    "platinum bar",
    "pot of gold",
    "jewels",
    "diamond",
    "emerald",
    "huge ruby",
    "screwdriver",
    "window",
    "rug",
    "trap door"
};

/* Room exits: [room_id*6 + dir], N/S/E/W/U/D */
const u8 g_room_exits[NUM_ROOMS * 6] = {
    1, 2, 255, 255, 255, 255,  /* 0  WEST_OF_HOUSE */
    255, 0, 3, 255, 255, 255,  /* 1  NORTH_OF_HOUSE */
    0, 255, 3, 255, 255, 255,  /* 2  SOUTH_OF_HOUSE */
    1, 2, 19, 255, 255, 255,   /* 3  BEHIND_HOUSE */
    255, 255, 255, 5, 6, 255,  /* 4  KITCHEN */
    255, 255, 4, 255, 255, 7,  /* 5  LIVING_ROOM */
    255, 255, 255, 255, 255, 4,/* 6  ATTIC */
    8, 255, 255, 255, 5, 255,  /* 7  CELLAR */
    255, 255, 9, 255, 255, 255,/* 8  EAST_PASSAGE */
    10, 255, 255, 8, 255, 255, /* 9  ROUND_ROOM */
    255, 9, 11, 255, 255, 255, /* 10 NORTH_SOUTH_PASS */
    13, 255, 255, 10, 255, 255,/* 11 EW_PASSAGE */
    255, 255, 255, 9, 255, 255,/* 12 LOUD_ROOM */
    14, 255, 255, 11, 255, 255,/* 13 GALLERY */
    255, 13, 255, 255, 255, 255,/* 14 STUDIO */
    255, 7, 255, 255, 255, 255,/* 15 CAVE */
    17, 18, 16, 17, 255, 255,  /* 16 MAZE_1 */
    18, 16, 17, 18, 255, 7,    /* 17 MAZE_2 */
    16, 17, 18, 16, 255, 255,  /* 18 MAZE_3 */
    20, 255, 255, 3, 255, 255, /* 19 CLEARING: N=Forest1 W=Behind */
    22, 19, 21, 20, 255, 255,  /* 20 FOREST_1: N=Grating S=Clearing E=Forest2 */
    20, 255, 255, 19, 255, 255,/* 21 FOREST_2: N=Forest1 W=Clearing */
    255, 20, 255, 21, 255, 255 /* 22 GRATING_CLEARING: S=Forest1 W=Forest2 */
};

/* Default object locations */
const u8 g_obj_default_loc[NUM_OBJECTS] = {
    ROOM_WEST_OF_HOUSE,
    OBJ_MAILBOX,
    ROOM_BEHIND_HOUSE,
    ROOM_LIVING_ROOM,
    ROOM_LIVING_ROOM,
    ROOM_LIVING_ROOM,
    ROOM_GALLERY,
    ROOM_KITCHEN,
    OBJ_SACK,
    ROOM_KITCHEN,
    ROOM_KITCHEN,
    ROOM_STUDIO,
    ROOM_ATTIC,
    ROOM_CAVE,
    ROOM_CAVE,
    ROOM_CELLAR,
    ROOM_MAZE_1,
    ROOM_CAVE,
    ROOM_EW_PASSAGE,
    ROOM_LOUD_ROOM,
    ROOM_STUDIO,
    ROOM_MAZE_3,
    ROOM_STUDIO,         /* 22 screwdriver */
    3,                   /* 23 window = ROOM_BEHIND_HOUSE */
    5,                   /* 24 rug = ROOM_LIVING_ROOM */
    255                  /* 25 trap door starts hidden (LOC_GONE) */
};

/* Default object flags */
const u8 g_obj_default_flags[NUM_OBJECTS] = {
    OBJ_CONTAINER,
    OBJ_TAKEABLE,
    OBJ_TAKEABLE,
    OBJ_TAKEABLE | OBJ_LIGHT,
    OBJ_TAKEABLE | OBJ_WEAPON,
    OBJ_CONTAINER | OBJ_OPEN,
    OBJ_TAKEABLE,
    OBJ_TAKEABLE | OBJ_CONTAINER | OBJ_OPEN,
    OBJ_TAKEABLE,
    OBJ_TAKEABLE | OBJ_CONTAINER | OBJ_OPEN,
    OBJ_TAKEABLE | OBJ_WEAPON,
    OBJ_TAKEABLE,
    OBJ_TAKEABLE,
    OBJ_TAKEABLE,
    OBJ_TAKEABLE | OBJ_LIGHT,
    OBJ_TAKEABLE,
    OBJ_TAKEABLE,
    OBJ_TAKEABLE,
    OBJ_TAKEABLE,
    OBJ_TAKEABLE,
    OBJ_TAKEABLE,
    OBJ_TAKEABLE,
    OBJ_TAKEABLE | OBJ_WEAPON, /* 22 screwdriver */
    OBJ_CONTAINER | OBJ_OPEN,  /* 23 window - already open */
    0,                         /* 24 rug */
    OBJ_CONTAINER              /* 25 trap door - closed */
};

/* Score values */
const u8 g_obj_score[NUM_OBJECTS] = {
    0, 0, 0, 0, 0, 0,
    100, 0, 0, 0, 0, 150,
    0, 0, 0, 10, 50, 100,
    50, 100, 50, 50, 0,
    0, 0, 0
};

/* Score target container */
const u8 g_obj_target[NUM_OBJECTS] = {
    NO_CONTAINER, NO_CONTAINER, NO_CONTAINER, NO_CONTAINER,
    NO_CONTAINER, NO_CONTAINER, OBJ_TROPHY_CASE, NO_CONTAINER,
    NO_CONTAINER, NO_CONTAINER, NO_CONTAINER, OBJ_TROPHY_CASE,
    NO_CONTAINER, NO_CONTAINER, NO_CONTAINER, OBJ_TROPHY_CASE,
    OBJ_TROPHY_CASE, OBJ_TROPHY_CASE, OBJ_TROPHY_CASE, OBJ_TROPHY_CASE,
    OBJ_TROPHY_CASE, OBJ_TROPHY_CASE, NO_CONTAINER
};
