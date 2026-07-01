/* world.c - Zork I Neo: Complete world data (110 rooms, 122 objects)
 * Auto-generated from historicalsource/zork1 MIT-licensed ZIL source
 */
#include "zork_types.h"

const char g_verb_names[NUM_VERBS][12] = {
    "LOOK",
    "GO",
    "TAKE",
    "DROP",
    "EXAMINE",
    "OPEN",
    "CLOSE",
    "READ",
    "PUT",
    "ATTACK",
    "INVENTORY",
    "TURN ON",
    "TURN OFF",
    "WAIT",
    "AGAIN",
    "NORTH",
    "SOUTH",
    "EAST",
    "WEST",
    "UP",
    "DOWN",
    "SAVE",
    "RESTORE",
    "QUIT",
    "ENTER",
    "CLIMB",
    "MOVE",
    "GET",
    "DIG",
    "TIE",
    "WAVE",
    "RING",
    "LIGHT",
    "LOWER",
    "RAISE",
    "INFLATE",
    "PRAY",
    "ECHO",
    "CROSS",
};

const char g_dir_names[NUM_DIRS][6] = {
    "NORTH",
    "SOUTH",
    "EAST",
    "WEST",
    "NE",
    "NW",
    "SE",
    "SW",
    "UP",
    "DOWN",
};

const char g_room_name[NUM_ROOMS][20] = {
    "West of House",  /*   0 */
    "Stone Barrow",  /*   1 */
    "North of House",  /*   2 */
    "South of House",  /*   3 */
    "Behind House",  /*   4 */
    "Forest",  /*   5 */
    "Forest",  /*   6 */
    "Forest",  /*   7 */
    "Forest",  /*   8 */
    "Forest Path",  /*   9 */
    "Up a Tree",  /*  10 */
    "Clearing",  /*  11 */
    "Clearing",  /*  12 */
    "Kitchen",  /*  13 */
    "Attic",  /*  14 */
    "Living Room",  /*  15 */
    "Cellar",  /*  16 */
    "The Troll Room",  /*  17 */
    "East of Chasm",  /*  18 */
    "Gallery",  /*  19 */
    "Studio",  /*  20 */
    "Maze",  /*  21 */
    "Maze",  /*  22 */
    "Maze",  /*  23 */
    "Maze",  /*  24 */
    "Dead End",  /*  25 */
    "Maze",  /*  26 */
    "Dead End",  /*  27 */
    "Maze",  /*  28 */
    "Maze",  /*  29 */
    "Maze",  /*  30 */
    "Dead End",  /*  31 */
    "Maze",  /*  32 */
    "Maze",  /*  33 */
    "Maze",  /*  34 */
    "Grating Room",  /*  35 */
    "Maze",  /*  36 */
    "Dead End",  /*  37 */
    "Maze",  /*  38 */
    "Maze",  /*  39 */
    "Maze",  /*  40 */
    "Cyclops Room",  /*  41 */
    "Strange Passage",  /*  42 */
    "Treasure Room",  /*  43 */
    "Reservoir South",  /*  44 */
    "Reservoir",  /*  45 */
    "Reservoir North",  /*  46 */
    "Stream View",  /*  47 */
    "Stream",  /*  48 */
    "Mirror Room",  /*  49 */
    "Mirror Room",  /*  50 */
    "Cave",  /*  51 */
    "Cave",  /*  52 */
    "Cold Passage",  /*  53 */
    "Narrow Passage",  /*  54 */
    "Winding Passage",  /*  55 */
    "Twisting Passage",  /*  56 */
    "Atlantis Room",  /*  57 */
    "East-West Passage",  /*  58 */
    "Round Room",  /*  59 */
    "Deep Canyon",  /*  60 */
    "Damp Cave",  /*  61 */
    "Loud Room",  /*  62 */
    "North-South Passage",  /*  63 */
    "Chasm",  /*  64 */
    "Entrance to Hades",  /*  65 */
    "Land of the Dead",  /*  66 */
    "Engravings Cave",  /*  67 */
    "Egyptian Room",  /*  68 */
    "Dome Room",  /*  69 */
    "Torch Room",  /*  70 */
    "Temple",  /*  71 */
    "Altar",  /*  72 */
    "Dam",  /*  73 */
    "Dam Lobby",  /*  74 */
    "Maintenance Room",  /*  75 */
    "Dam Base",  /*  76 */
    "Frigid River",  /*  77 */
    "Frigid River",  /*  78 */
    "Frigid River",  /*  79 */
    "White Cliffs Beach",  /*  80 */
    "White Cliffs Beach",  /*  81 */
    "Frigid River",  /*  82 */
    "Frigid River",  /*  83 */
    "Shore",  /*  84 */
    "Sandy Beach",  /*  85 */
    "Sandy Cave",  /*  86 */
    "Aragain Falls",  /*  87 */
    "On the Rainbow",  /*  88 */
    "End of Rainbow",  /*  89 */
    "Canyon Bottom",  /*  90 */
    "Rocky Ledge",  /*  91 */
    "Canyon View",  /*  92 */
    "Mine Entrance",  /*  93 */
    "Squeaky Room",  /*  94 */
    "Bat Room",  /*  95 */
    "Shaft Room",  /*  96 */
    "Smelly Room",  /*  97 */
    "Gas Room",  /*  98 */
    "Ladder Top",  /*  99 */
    "Ladder Bottom",  /* 100 */
    "Dead End",  /* 101 */
    "Timber Room",  /* 102 */
    "Drafty Room",  /* 103 */
    "Machine Room",  /* 104 */
    "Coal Mine",  /* 105 */
    "Coal Mine",  /* 106 */
    "Coal Mine",  /* 107 */
    "Coal Mine",  /* 108 */
    "Slide Room",  /* 109 */
};

const u8 g_room_exits[NUM_ROOMS * NUM_DIRS] = {
    2, 3, 255, 5, 2, 255, 3, 1, 255, 255,  /*   0 WEST-OF-HOUSE */
    255, 255, 255, 255, 0, 255, 255, 255, 255, 255,  /*   1 STONE-BARROW */
    9, 255, 4, 0, 255, 255, 4, 0, 255, 255,  /*   2 NORTH-OF-HOUSE */
    255, 8, 4, 0, 4, 0, 255, 255, 255, 255,  /*   3 SOUTH-OF-HOUSE */
    2, 3, 12, 13, 255, 2, 255, 3, 255, 255,  /*   4 EAST-OF-HOUSE */
    11, 8, 9, 255, 255, 255, 255, 255, 255, 255,  /*   5 FOREST-1 */
    255, 12, 7, 9, 255, 255, 255, 255, 255, 255,  /*   6 FOREST-2 */
    6, 6, 255, 6, 255, 255, 255, 255, 255, 255,  /*   7 MOUNTAINS */
    12, 255, 255, 5, 255, 3, 255, 255, 255, 255,  /*   8 FOREST-3 */
    11, 2, 6, 5, 255, 255, 255, 255, 10, 255,  /*   9 PATH */
    255, 255, 255, 255, 255, 255, 255, 255, 255, 9,  /*  10 UP-A-TREE */
    255, 9, 6, 5, 255, 255, 255, 255, 255, 255,  /*  11 GRATING-CLEARING */
    6, 8, 92, 4, 255, 255, 255, 255, 255, 255,  /*  12 CLEARING */
    255, 255, 4, 15, 255, 255, 255, 255, 14, 20,  /*  13 KITCHEN */
    255, 255, 255, 255, 255, 255, 255, 255, 255, 13,  /*  14 ATTIC */
    255, 255, 13, 42, 255, 255, 255, 255, 255, 255,  /*  15 LIVING-ROOM */
    17, 18, 255, 255, 255, 255, 255, 255, 15, 255,  /*  16 CELLAR */
    255, 16, 58, 21, 255, 255, 255, 255, 255, 255,  /*  17 TROLL-ROOM */
    16, 255, 19, 255, 255, 255, 255, 255, 255, 255,  /*  18 EAST-OF-CHASM */
    20, 255, 255, 18, 255, 255, 255, 255, 255, 255,  /*  19 GALLERY */
    255, 19, 255, 255, 255, 255, 255, 255, 255, 255,  /*  20 STUDIO */
    21, 22, 17, 24, 255, 255, 255, 255, 255, 255,  /*  21 MAZE-1 */
    255, 21, 23, 255, 255, 255, 255, 255, 255, 255,  /*  22 MAZE-2 */
    24, 255, 255, 22, 255, 255, 255, 255, 26, 255,  /*  23 MAZE-3 */
    21, 255, 25, 23, 255, 255, 255, 255, 255, 255,  /*  24 MAZE-4 */
    255, 24, 255, 255, 255, 255, 255, 255, 255, 255,  /*  25 DEAD-END-1 */
    23, 255, 27, 255, 255, 255, 255, 28, 255, 255,  /*  26 MAZE-5 */
    255, 255, 255, 26, 255, 255, 255, 255, 255, 255,  /*  27 DEAD-END-2 */
    255, 255, 29, 28, 255, 255, 255, 255, 32, 26,  /*  28 MAZE-6 */
    255, 40, 30, 28, 255, 255, 255, 255, 39, 255,  /*  29 MAZE-7 */
    255, 255, 255, 30, 29, 255, 31, 255, 255, 255,  /*  30 MAZE-8 */
    30, 255, 255, 255, 255, 255, 255, 255, 255, 255,  /*  31 DEAD-END-3 */
    28, 38, 33, 36, 255, 32, 255, 255, 255, 255,  /*  32 MAZE-9 */
    255, 255, 32, 38, 255, 255, 255, 255, 34, 255,  /*  33 MAZE-10 */
    255, 255, 255, 255, 35, 38, 255, 36, 255, 33,  /*  34 MAZE-11 */
    255, 255, 255, 255, 255, 255, 255, 34, 11, 255,  /*  35 GRATING-ROOM */
    37, 255, 38, 255, 255, 255, 255, 34, 32, 255,  /*  36 MAZE-12 */
    255, 36, 255, 255, 255, 255, 255, 255, 255, 255,  /*  37 DEAD-END-4 */
    255, 33, 32, 34, 255, 255, 255, 255, 255, 36,  /*  38 MAZE-13 */
    255, 29, 255, 40, 29, 39, 255, 255, 255, 255,  /*  39 MAZE-14 */
    255, 29, 255, 39, 255, 255, 41, 255, 255, 255,  /*  40 MAZE-15 */
    255, 255, 42, 255, 255, 40, 255, 255, 43, 255,  /*  41 CYCLOPS-ROOM */
    255, 255, 15, 41, 255, 255, 255, 255, 255, 255,  /*  42 STRANGE-PASSAGE */
    255, 255, 255, 255, 255, 255, 255, 255, 255, 41,  /*  43 TREASURE-ROOM */
    45, 255, 73, 47, 255, 255, 60, 64, 255, 255,  /*  44 RESERVOIR-SOUTH */
    46, 44, 255, 48, 255, 255, 255, 255, 48, 255,  /*  45 RESERVOIR */
    57, 45, 255, 255, 255, 255, 255, 255, 255, 255,  /*  46 RESERVOIR-NORTH */
    255, 255, 44, 255, 255, 255, 255, 255, 255, 255,  /*  47 STREAM-VIEW */
    255, 255, 45, 255, 255, 255, 255, 255, 255, 45,  /*  48 IN-STREAM */
    53, 255, 51, 56, 255, 255, 255, 255, 255, 255,  /*  49 MIRROR-ROOM-1 */
    54, 255, 52, 55, 255, 255, 255, 255, 255, 255,  /*  50 MIRROR-ROOM-2 */
    49, 57, 255, 56, 255, 255, 255, 255, 255, 57,  /*  51 SMALL-CAVE */
    50, 255, 255, 55, 255, 255, 255, 255, 255, 65,  /*  52 TINY-CAVE */
    255, 49, 255, 109, 255, 255, 255, 255, 255, 255,  /*  53 COLD-PASSAGE */
    59, 50, 255, 255, 255, 255, 255, 255, 255, 255,  /*  54 NARROW-PASSAGE */
    50, 255, 52, 255, 255, 255, 255, 255, 255, 255,  /*  55 WINDING-PASSAGE */
    49, 255, 51, 255, 255, 255, 255, 255, 255, 255,  /*  56 TWISTING-PASSAGE */
    255, 46, 255, 255, 255, 255, 255, 255, 51, 255,  /*  57 ATLANTIS-ROOM */
    64, 255, 59, 17, 255, 255, 255, 255, 255, 64,  /*  58 EW-PASSAGE */
    63, 54, 62, 58, 255, 255, 67, 255, 255, 255,  /*  59 ROUND-ROOM */
    255, 255, 73, 255, 255, 44, 255, 63, 255, 62,  /*  60 DEEP-CANYON */
    255, 255, 80, 62, 255, 255, 255, 255, 255, 255,  /*  61 DAMP-CAVE */
    255, 255, 61, 59, 255, 255, 255, 255, 60, 255,  /*  62 LOUD-ROOM */
    64, 59, 255, 255, 60, 255, 255, 255, 255, 255,  /*  63 NS-PASSAGE */
    255, 63, 255, 255, 44, 255, 255, 58, 58, 255,  /*  64 CHASM-ROOM */
    255, 66, 255, 255, 255, 255, 255, 255, 52, 255,  /*  65 ENTRANCE-TO-HADES */
    65, 255, 255, 255, 255, 255, 255, 255, 255, 255,  /*  66 LAND-OF-LIVING-DEAD */
    255, 255, 69, 255, 255, 59, 255, 255, 255, 255,  /*  67 ENGRAVINGS-CAVE */
    255, 255, 255, 71, 255, 255, 255, 255, 71, 255,  /*  68 EGYPT-ROOM */
    255, 255, 255, 67, 255, 255, 255, 255, 255, 70,  /*  69 DOME-ROOM */
    255, 71, 255, 255, 255, 255, 255, 255, 255, 71,  /*  70 TORCH-ROOM */
    70, 72, 68, 255, 255, 255, 255, 255, 70, 68,  /*  71 NORTH-TEMPLE */
    71, 255, 255, 255, 255, 255, 255, 255, 255, 52,  /*  72 SOUTH-TEMPLE */
    74, 60, 76, 44, 255, 255, 255, 255, 255, 76,  /*  73 DAM-ROOM */
    75, 73, 75, 255, 255, 255, 255, 255, 255, 255,  /*  74 DAM-LOBBY */
    255, 74, 255, 74, 255, 255, 255, 255, 255, 255,  /*  75 MAINTENANCE-ROOM */
    73, 255, 255, 255, 255, 255, 255, 255, 73, 255,  /*  76 DAM-BASE */
    255, 255, 255, 76, 255, 255, 255, 255, 255, 78,  /*  77 RIVER-1 */
    255, 255, 255, 255, 255, 255, 255, 255, 255, 79,  /*  78 RIVER-2 */
    255, 255, 255, 80, 255, 255, 255, 255, 255, 82,  /*  79 RIVER-3 */
    255, 81, 255, 61, 255, 255, 255, 255, 255, 255,  /*  80 WHITE-CLIFFS-NORTH */
    80, 255, 255, 255, 255, 255, 255, 255, 255, 255,  /*  81 WHITE-CLIFFS-SOUTH */
    255, 255, 85, 81, 255, 255, 255, 255, 255, 83,  /*  82 RIVER-4 */
    255, 255, 84, 255, 255, 255, 255, 255, 255, 255,  /*  83 RIVER-5 */
    85, 87, 255, 255, 255, 255, 255, 255, 255, 255,  /*  84 SHORE */
    255, 84, 255, 255, 86, 255, 255, 255, 255, 255,  /*  85 SANDY-BEACH */
    255, 255, 255, 255, 255, 255, 255, 85, 255, 255,  /*  86 SANDY-CAVE */
    84, 255, 255, 88, 255, 255, 255, 255, 88, 255,  /*  87 ARAGAIN-FALLS */
    255, 255, 87, 89, 255, 255, 255, 255, 255, 255,  /*  88 ON-RAINBOW */
    255, 255, 88, 255, 88, 255, 255, 90, 88, 255,  /*  89 END-OF-RAINBOW */
    89, 255, 255, 255, 255, 255, 255, 255, 91, 255,  /*  90 CANYON-BOTTOM */
    255, 255, 255, 255, 255, 255, 255, 255, 92, 90,  /*  91 CLIFF-MIDDLE */
    255, 255, 91, 8, 255, 12, 255, 255, 255, 91,  /*  92 CANYON-VIEW */
    255, 109, 255, 94, 255, 255, 255, 255, 255, 255,  /*  93 MINE-ENTRANCE */
    95, 255, 93, 255, 255, 255, 255, 255, 255, 255,  /*  94 SQUEEKY-ROOM */
    255, 94, 96, 255, 255, 255, 255, 255, 255, 255,  /*  95 BAT-ROOM */
    97, 255, 255, 95, 255, 255, 255, 255, 255, 255,  /*  96 SHAFT-ROOM */
    255, 96, 255, 255, 255, 255, 255, 255, 255, 98,  /*  97 SMELLY-ROOM */
    255, 255, 105, 255, 255, 255, 255, 255, 97, 255,  /*  98 GAS-ROOM */
    255, 255, 255, 255, 255, 255, 255, 255, 108, 100,  /*  99 LADDER-TOP */
    255, 101, 255, 102, 255, 255, 255, 255, 99, 255,  /* 100 LADDER-BOTTOM */
    100, 255, 255, 255, 255, 255, 255, 255, 255, 255,  /* 101 DEAD-END-5 */
    255, 255, 100, 103, 255, 255, 255, 255, 255, 255,  /* 102 TIMBER-ROOM */
    255, 104, 102, 255, 255, 255, 255, 255, 255, 255,  /* 103 LOWER-SHAFT */
    103, 255, 255, 255, 255, 255, 255, 255, 255, 255,  /* 104 MACHINE-ROOM */
    98, 255, 105, 255, 106, 255, 255, 255, 255, 255,  /* 105 MINE-1 */
    106, 105, 255, 255, 255, 255, 107, 255, 255, 255,  /* 106 MINE-2 */
    255, 107, 106, 255, 255, 255, 255, 108, 255, 255,  /* 107 MINE-3 */
    107, 255, 255, 108, 255, 255, 255, 255, 255, 99,  /* 108 MINE-4 */
    93, 255, 53, 255, 255, 255, 255, 255, 255, 16,  /* 109 SLIDE-ROOM */
};

const u8 g_room_base_flags[NUM_ROOMS] = {
    ROOM_ABOVE,  /*   0 WEST-OF-HOUSE */
    ROOM_ABOVE,  /*   1 STONE-BARROW */
    ROOM_ABOVE,  /*   2 NORTH-OF-HOUSE */
    ROOM_ABOVE,  /*   3 SOUTH-OF-HOUSE */
    ROOM_ABOVE,  /*   4 EAST-OF-HOUSE */
    ROOM_ABOVE,  /*   5 FOREST-1 */
    ROOM_ABOVE,  /*   6 FOREST-2 */
    ROOM_ABOVE,  /*   7 MOUNTAINS */
    ROOM_ABOVE,  /*   8 FOREST-3 */
    ROOM_ABOVE,  /*   9 PATH */
    ROOM_ABOVE,  /*  10 UP-A-TREE */
    ROOM_ABOVE,  /*  11 GRATING-CLEARING */
    ROOM_ABOVE,  /*  12 CLEARING */
    ROOM_DARK,  /*  13 KITCHEN */
    ROOM_DARK,  /*  14 ATTIC */
    ROOM_DARK,  /*  15 LIVING-ROOM */
    ROOM_DARK,  /*  16 CELLAR */
    ROOM_DARK,  /*  17 TROLL-ROOM */
    ROOM_DARK,  /*  18 EAST-OF-CHASM */
    ROOM_DARK,  /*  19 GALLERY */
    ROOM_DARK,  /*  20 STUDIO */
    ROOM_DARK,  /*  21 MAZE-1 */
    ROOM_DARK,  /*  22 MAZE-2 */
    ROOM_DARK,  /*  23 MAZE-3 */
    ROOM_DARK,  /*  24 MAZE-4 */
    ROOM_DARK,  /*  25 DEAD-END-1 */
    ROOM_DARK,  /*  26 MAZE-5 */
    ROOM_DARK,  /*  27 DEAD-END-2 */
    ROOM_DARK,  /*  28 MAZE-6 */
    ROOM_DARK,  /*  29 MAZE-7 */
    ROOM_DARK,  /*  30 MAZE-8 */
    ROOM_DARK,  /*  31 DEAD-END-3 */
    ROOM_DARK,  /*  32 MAZE-9 */
    ROOM_DARK,  /*  33 MAZE-10 */
    ROOM_DARK,  /*  34 MAZE-11 */
    ROOM_DARK,  /*  35 GRATING-ROOM */
    ROOM_DARK,  /*  36 MAZE-12 */
    ROOM_DARK,  /*  37 DEAD-END-4 */
    ROOM_DARK,  /*  38 MAZE-13 */
    ROOM_DARK,  /*  39 MAZE-14 */
    ROOM_DARK,  /*  40 MAZE-15 */
    ROOM_DARK,  /*  41 CYCLOPS-ROOM */
    ROOM_DARK,  /*  42 STRANGE-PASSAGE */
    ROOM_DARK,  /*  43 TREASURE-ROOM */
    ROOM_DARK,  /*  44 RESERVOIR-SOUTH */
    ROOM_DARK,  /*  45 RESERVOIR */
    ROOM_DARK,  /*  46 RESERVOIR-NORTH */
    ROOM_DARK,  /*  47 STREAM-VIEW */
    ROOM_DARK,  /*  48 IN-STREAM */
    ROOM_DARK,  /*  49 MIRROR-ROOM-1 */
    ROOM_DARK,  /*  50 MIRROR-ROOM-2 */
    ROOM_DARK,  /*  51 SMALL-CAVE */
    ROOM_DARK,  /*  52 TINY-CAVE */
    ROOM_DARK,  /*  53 COLD-PASSAGE */
    ROOM_DARK,  /*  54 NARROW-PASSAGE */
    ROOM_DARK,  /*  55 WINDING-PASSAGE */
    ROOM_DARK,  /*  56 TWISTING-PASSAGE */
    ROOM_DARK,  /*  57 ATLANTIS-ROOM */
    ROOM_DARK,  /*  58 EW-PASSAGE */
    ROOM_DARK,  /*  59 ROUND-ROOM */
    ROOM_DARK,  /*  60 DEEP-CANYON */
    ROOM_DARK,  /*  61 DAMP-CAVE */
    ROOM_DARK,  /*  62 LOUD-ROOM */
    ROOM_DARK,  /*  63 NS-PASSAGE */
    ROOM_DARK,  /*  64 CHASM-ROOM */
    ROOM_DARK,  /*  65 ENTRANCE-TO-HADES */
    ROOM_DARK,  /*  66 LAND-OF-LIVING-DEAD */
    ROOM_DARK,  /*  67 ENGRAVINGS-CAVE */
    ROOM_DARK,  /*  68 EGYPT-ROOM */
    ROOM_DARK,  /*  69 DOME-ROOM */
    ROOM_DARK,  /*  70 TORCH-ROOM */
    ROOM_DARK,  /*  71 NORTH-TEMPLE */
    ROOM_DARK,  /*  72 SOUTH-TEMPLE */
    ROOM_DARK,  /*  73 DAM-ROOM */
    ROOM_DARK,  /*  74 DAM-LOBBY */
    ROOM_DARK,  /*  75 MAINTENANCE-ROOM */
    ROOM_DARK,  /*  76 DAM-BASE */
    ROOM_DARK,  /*  77 RIVER-1 */
    ROOM_DARK,  /*  78 RIVER-2 */
    ROOM_DARK,  /*  79 RIVER-3 */
    ROOM_ABOVE,  /*  80 WHITE-CLIFFS-NORTH */
    ROOM_ABOVE,  /*  81 WHITE-CLIFFS-SOUTH */
    ROOM_DARK,  /*  82 RIVER-4 */
    ROOM_DARK,  /*  83 RIVER-5 */
    ROOM_ABOVE,  /*  84 SHORE */
    ROOM_ABOVE,  /*  85 SANDY-BEACH */
    ROOM_DARK,  /*  86 SANDY-CAVE */
    ROOM_ABOVE,  /*  87 ARAGAIN-FALLS */
    ROOM_ABOVE,  /*  88 ON-RAINBOW */
    ROOM_ABOVE,  /*  89 END-OF-RAINBOW */
    ROOM_ABOVE,  /*  90 CANYON-BOTTOM */
    ROOM_ABOVE,  /*  91 CLIFF-MIDDLE */
    ROOM_ABOVE,  /*  92 CANYON-VIEW */
    ROOM_DARK,  /*  93 MINE-ENTRANCE */
    ROOM_DARK,  /*  94 SQUEEKY-ROOM */
    ROOM_DARK,  /*  95 BAT-ROOM */
    ROOM_DARK,  /*  96 SHAFT-ROOM */
    ROOM_DARK,  /*  97 SMELLY-ROOM */
    ROOM_DARK,  /*  98 GAS-ROOM */
    ROOM_DARK,  /*  99 LADDER-TOP */
    ROOM_DARK,  /* 100 LADDER-BOTTOM */
    ROOM_DARK,  /* 101 DEAD-END-5 */
    ROOM_DARK,  /* 102 TIMBER-ROOM */
    ROOM_DARK,  /* 103 LOWER-SHAFT */
    ROOM_DARK,  /* 104 MACHINE-ROOM */
    ROOM_DARK,  /* 105 MINE-1 */
    ROOM_DARK,  /* 106 MINE-2 */
    ROOM_DARK,  /* 107 MINE-3 */
    ROOM_DARK,  /* 108 MINE-4 */
    ROOM_DARK,  /* 109 SLIDE-ROOM */
};

const char g_obj_name[NUM_OBJECTS][16] = {
    "board",  /*   0 BOARD */
    "set of teeth",  /*   1 TEETH */
    "surrounding wal",  /*   2 WALL */
    "granite wall",  /*   3 GRANITE-WALL */
    "songbird",  /*   4 SONGBIRD */
    "white house",  /*   5 WHITE-HOUSE */
    "forest",  /*   6 FOREST */
    "tree",  /*   7 TREE */
    "mountain range",  /*   8 MOUNTAIN-RANGE */
    "water",  /*   9 GLOBAL-WATER */
    "quantity of wat",  /*  10 WATER */
    "kitchen window",  /*  11 KITCHEN-WINDOW */
    "chimney",  /*  12 CHIMNEY */
    "number of ghost",  /*  13 GHOSTS */
    "crystal skull",  /*  14 SKULL */
    "basket",  /*  15 LOWERED-BASKET */
    "basket",  /*  16 RAISED-BASKET */
    "lunch",  /*  17 LUNCH */
    "bat",  /*  18 BAT */
    "brass bell",  /*  19 BELL */
    "red hot brass b",  /*  20 HOT-BELL */
    "bloody axe",  /*  21 AXE */
    "bolt",  /*  22 BOLT */
    "green bubble",  /*  23 BUBBLE */
    "altar",  /*  24 ALTAR */
    "black book",  /*  25 BOOK */
    "broken lantern",  /*  26 BROKEN-LAMP */
    "sceptre",  /*  27 SCEPTRE */
    "broken timber",  /*  28 TIMBERS */
    "chute",  /*  29 SLIDE */
    "kitchen table",  /*  30 KITCHEN-TABLE */
    "table",  /*  31 ATTIC-TABLE */
    "brown sack",  /*  32 SANDWICH-BAG */
    "group of tool c",  /*  33 TOOL-CHEST */
    "yellow button",  /*  34 YELLOW-BUTTON */
    "brown button",  /*  35 BROWN-BUTTON */
    "red button",  /*  36 RED-BUTTON */
    "blue button",  /*  37 BLUE-BUTTON */
    "trophy case",  /*  38 TROPHY-CASE */
    "carpet",  /*  39 RUG */
    "chalice",  /*  40 CHALICE */
    "clove of garlic",  /*  41 GARLIC */
    "crystal trident",  /*  42 TRIDENT */
    "cyclops",  /*  43 CYCLOPS */
    "dam",  /*  44 DAM */
    "trap door",  /*  45 TRAP-DOOR */
    "boarded window",  /*  46 BOARDED-WINDOW */
    "door",  /*  47 FRONT-DOOR */
    "stone door",  /*  48 BARROW-DOOR */
    "stone barrow",  /*  49 BARROW */
    "glass bottle",  /*  50 BOTTLE */
    "crack",  /*  51 CRACK */
    "gold coffin",  /*  52 COFFIN */
    "grating",  /*  53 GRATE */
    "hand-held air p",  /*  54 PUMP */
    "huge diamond",  /*  55 DIAMOND */
    "jade figurine",  /*  56 JADE */
    "nasty knife",  /*  57 KNIFE */
    "skeleton",  /*  58 BONES */
    "burned-out lant",  /*  59 BURNED-OUT-LANTERN */
    "leather bag of ",  /*  60 BAG-OF-COINS */
    "brass lantern",  /*  61 LAMP */
    "large emerald",  /*  62 EMERALD */
    "leaflet",  /*  63 ADVERTISEMENT */
    "leak",  /*  64 LEAK */
    "machine",  /*  65 MACHINE */
    "magic boat",  /*  66 INFLATED-BOAT */
    "small mailbox",  /*  67 MAILBOX */
    "matchbook",  /*  68 MATCH */
    "mirror",  /*  69 MIRROR-2 */
    "mirror",  /*  70 MIRROR-1 */
    "painting",  /*  71 PAINTING */
    "pair of candles",  /*  72 CANDLES */
    "small piece of ",  /*  73 GUNK */
    "pile of bodies",  /*  74 BODIES */
    "pile of leaves",  /*  75 LEAVES */
    "punctured boat",  /*  76 PUNCTURED-BOAT */
    "pile of plastic",  /*  77 INFLATABLE-BOAT */
    "platinum bar",  /*  78 BAR */
    "pot of gold",  /*  79 POT-OF-GOLD */
    "prayer",  /*  80 PRAYER */
    "wooden railing",  /*  81 RAILING */
    "rainbow",  /*  82 RAINBOW */
    "river",  /*  83 RIVER */
    "red buoy",  /*  84 BUOY */
    "rope",  /*  85 ROPE */
    "rusty knife",  /*  86 RUSTY-KNIFE */
    "sand",  /*  87 SAND */
    "sapphire-encrus",  /*  88 BRACELET */
    "screwdriver",  /*  89 SCREWDRIVER */
    "skeleton key",  /*  90 KEYS */
    "shovel",  /*  91 SHOVEL */
    "small pile of c",  /*  92 COAL */
    "wooden ladder",  /*  93 LADDER */
    "beautiful jewel",  /*  94 SCARAB */
    "large bag",  /*  95 LARGE-BAG */
    "stiletto",  /*  96 STILETTO */
    "switch",  /*  97 MACHINE-SWITCH */
    "wooden door",  /*  98 WOODEN-DOOR */
    "sword",  /*  99 SWORD */
    "ancient map",  /* 100 MAP */
    "tan label",  /* 101 BOAT-LABEL */
    "thief",  /* 102 THIEF */
    "pedestal",  /* 103 PEDESTAL */
    "torch",  /* 104 TORCH */
    "tour guidebook",  /* 105 GUIDE */
    "troll",  /* 106 TROLL */
    "trunk of jewels",  /* 107 TRUNK */
    "tube",  /* 108 TUBE */
    "viscous materia",  /* 109 PUTTY */
    "wall with engra",  /* 110 ENGRAVINGS */
    "ZORK owner's ma",  /* 111 OWNERS-MANUAL */
    "cliff",  /* 112 CLIMBABLE-CLIFF */
    "white cliffs",  /* 113 WHITE-CLIFF */
    "wrench",  /* 114 WRENCH */
    "control panel",  /* 115 CONTROL-PANEL */
    "bird's nest",  /* 116 NEST */
    "jewel-encrusted",  /* 117 EGG */
    "broken jewel-en",  /* 118 BROKEN-EGG */
    "beautiful brass",  /* 119 BAUBLE */
    "golden clockwor",  /* 120 CANARY */
    "broken clockwor",  /* 121 BROKEN-CANARY */
};

const u8 g_obj_default_loc[NUM_OBJECTS] = {
    LOC_GONE,  /*   0 BOARD */
    LOC_GONE,  /*   1 TEETH */
    LOC_GONE,  /*   2 WALL */
    LOC_GONE,  /*   3 GRANITE-WALL */
    LOC_GONE,  /*   4 SONGBIRD */
    LOC_GONE,  /*   5 WHITE-HOUSE */
    LOC_GONE,  /*   6 FOREST */
    LOC_GONE,  /*   7 TREE */
    7,  /*   8 MOUNTAIN-RANGE */
    LOC_GONE,  /*   9 GLOBAL-WATER */
    50,  /*  10 WATER */
    4,  /*  11 KITCHEN-WINDOW = ROOM_EAST_OF_HOUSE */
    LOC_GONE,  /*  12 CHIMNEY */
    65,  /*  13 GHOSTS */
    66,  /*  14 SKULL */
    103,  /*  15 LOWERED-BASKET */
    96,  /*  16 RAISED-BASKET */
    32,  /*  17 LUNCH */
    95,  /*  18 BAT */
    71,  /*  19 BELL */
    LOC_GONE,  /*  20 HOT-BELL */
    106,  /*  21 AXE */
    73,  /*  22 BOLT */
    73,  /*  23 BUBBLE */
    72,  /*  24 ALTAR */
    24,  /*  25 BOOK */
    LOC_GONE,  /*  26 BROKEN-LAMP */
    52,  /*  27 SCEPTRE */
    102,  /*  28 TIMBERS */
    LOC_GONE,  /*  29 SLIDE */
    13,  /*  30 KITCHEN-TABLE */
    14,  /*  31 ATTIC-TABLE */
    30,  /*  32 SANDWICH-BAG */
    75,  /*  33 TOOL-CHEST */
    75,  /*  34 YELLOW-BUTTON */
    75,  /*  35 BROWN-BUTTON */
    75,  /*  36 RED-BUTTON */
    75,  /*  37 BLUE-BUTTON */
    15,  /*  38 TROPHY-CASE */
    15,  /*  39 RUG */
    43,  /*  40 CHALICE */
    32,  /*  41 GARLIC */
    57,  /*  42 TRIDENT */
    41,  /*  43 CYCLOPS */
    73,  /*  44 DAM */
    15,  /*  45 TRAP-DOOR */
    LOC_GONE,  /*  46 BOARDED-WINDOW */
    OBJ_NODESC,  /*  47 FRONT-DOOR */
    1,  /*  48 BARROW-DOOR */
    1,  /*  49 BARROW */
    30,  /*  50 BOTTLE */
    LOC_GONE,  /*  51 CRACK */
    68,  /*  52 COFFIN */
    LOC_GONE,  /*  53 GRATE */
    46,  /*  54 PUMP */
    LOC_GONE,  /*  55 DIAMOND */
    95,  /*  56 JADE */
    31,  /*  57 KNIFE */
    26,  /*  58 BONES */
    26,  /*  59 BURNED-OUT-LANTERN */
    26,  /*  60 BAG-OF-COINS */
    15,  /*  61 LAMP */
    84,  /*  62 EMERALD */
    67,  /*  63 ADVERTISEMENT */
    75,  /*  64 LEAK */
    104,  /*  65 MACHINE */
    LOC_GONE,  /*  66 INFLATED-BOAT */
    0,  /*  67 MAILBOX */
    74,  /*  68 MATCH */
    50,  /*  69 MIRROR-2 */
    49,  /*  70 MIRROR-1 */
    19,  /*  71 PAINTING */
    72,  /*  72 CANDLES */
    LOC_GONE,  /*  73 GUNK */
    LOC_GONE,  /*  74 BODIES */
    11,  /*  75 LEAVES */
    LOC_GONE,  /*  76 PUNCTURED-BOAT */
    76,  /*  77 INFLATABLE-BOAT */
    62,  /*  78 BAR */
    89,  /*  79 POT-OF-GOLD */
    71,  /*  80 PRAYER */
    69,  /*  81 RAILING */
    LOC_GONE,  /*  82 RAINBOW */
    LOC_GONE,  /*  83 RIVER */
    82,  /*  84 BUOY */
    14,  /*  85 ROPE */
    26,  /*  86 RUSTY-KNIFE */
    86,  /*  87 SAND */
    98,  /*  88 BRACELET */
    75,  /*  89 SCREWDRIVER */
    26,  /*  90 KEYS */
    85,  /*  91 SHOVEL */
    101,  /*  92 COAL */
    LOC_GONE,  /*  93 LADDER */
    86,  /*  94 SCARAB */
    102,  /*  95 LARGE-BAG */
    102,  /*  96 STILETTO */
    104,  /*  97 MACHINE-SWITCH */
    15,  /*  98 WOODEN-DOOR */
    15,  /*  99 SWORD */
    38,  /* 100 MAP */
    66,  /* 101 BOAT-LABEL */
    59,  /* 102 THIEF */
    70,  /* 103 PEDESTAL */
    103,  /* 104 TORCH */
    74,  /* 105 GUIDE */
    17,  /* 106 TROLL */
    45,  /* 107 TRUNK */
    75,  /* 108 TUBE */
    108,  /* 109 PUTTY */
    67,  /* 110 ENGRAVINGS */
    20,  /* 111 OWNERS-MANUAL */
    LOC_GONE,  /* 112 CLIMBABLE-CLIFF */
    LOC_GONE,  /* 113 WHITE-CLIFF */
    75,  /* 114 WRENCH */
    73,  /* 115 CONTROL-PANEL */
    10,  /* 116 NEST */
    116,  /* 117 EGG */
    LOC_GONE,  /* 118 BROKEN-EGG */
    LOC_GONE,  /* 119 BAUBLE */
    117,  /* 120 CANARY */
    118,  /* 121 BROKEN-CANARY */
};

const u8 g_obj_default_flags[NUM_OBJECTS] = {
    0,  /*   0 BOARD */
    0,  /*   1 TEETH */
    0,  /*   2 WALL */
    0,  /*   3 GRANITE-WALL */
    0,  /*   4 SONGBIRD */
    0,  /*   5 WHITE-HOUSE */
    0,  /*   6 FOREST */
    0,  /*   7 TREE */
    0,  /*   8 MOUNTAIN-RANGE */
    0,  /*   9 GLOBAL-WATER */
    OBJ_TAKEABLE,  /*  10 WATER */
    OBJ_OPEN | OBJ_CONTAINER | OBJ_NODESC,  /*  11 KITCHEN-WINDOW */
    0,  /*  12 CHIMNEY */
    0,  /*  13 GHOSTS */
    OBJ_TAKEABLE,  /*  14 SKULL */
    OBJ_TAKEABLE,  /*  15 LOWERED-BASKET */
    OBJ_TAKEABLE | OBJ_CONTAINER | OBJ_OPEN,  /*  16 RAISED-BASKET */
    OBJ_TAKEABLE,  /*  17 LUNCH */
    OBJ_TAKEABLE,  /*  18 BAT */
    OBJ_TAKEABLE,  /*  19 BELL */
    OBJ_TAKEABLE,  /*  20 HOT-BELL */
    OBJ_TAKEABLE | OBJ_WEAPON,  /*  21 AXE */
    OBJ_TAKEABLE,  /*  22 BOLT */
    OBJ_TAKEABLE,  /*  23 BUBBLE */
    OBJ_CONTAINER | OBJ_OPEN,  /*  24 ALTAR */
    OBJ_TAKEABLE | OBJ_CONTAINER,  /*  25 BOOK */
    OBJ_TAKEABLE,  /*  26 BROKEN-LAMP */
    OBJ_TAKEABLE | OBJ_WEAPON,  /*  27 SCEPTRE */
    OBJ_TAKEABLE,  /*  28 TIMBERS */
    0,  /*  29 SLIDE */
    OBJ_CONTAINER | OBJ_OPEN,  /*  30 KITCHEN-TABLE */
    OBJ_CONTAINER | OBJ_OPEN,  /*  31 ATTIC-TABLE */
    OBJ_TAKEABLE | OBJ_CONTAINER,  /*  32 SANDWICH-BAG */
    OBJ_TAKEABLE | OBJ_CONTAINER | OBJ_OPEN,  /*  33 TOOL-CHEST */
    0,  /*  34 YELLOW-BUTTON */
    0,  /*  35 BROWN-BUTTON */
    0,  /*  36 RED-BUTTON */
    0,  /*  37 BLUE-BUTTON */
    OBJ_TAKEABLE | OBJ_CONTAINER,  /*  38 TROPHY-CASE */
    OBJ_TAKEABLE,  /*  39 RUG */
    OBJ_TAKEABLE | OBJ_CONTAINER,  /*  40 CHALICE */
    OBJ_TAKEABLE,  /*  41 GARLIC */
    OBJ_TAKEABLE,  /*  42 TRIDENT */
    OBJ_TAKEABLE,  /*  43 CYCLOPS */
    OBJ_TAKEABLE,  /*  44 DAM */
    0,  /*  45 TRAP-DOOR */
    OBJ_NODESC,  /*  46 BOARDED-WINDOW */
    OBJ_NODESC,  /*  47 FRONT-DOOR */
    OBJ_OPEN | OBJ_NODESC,  /*  48 BARROW-DOOR */
    0,  /*  49 BARROW */
    OBJ_TAKEABLE | OBJ_CONTAINER,  /*  50 BOTTLE */
    0,  /*  51 CRACK */
    OBJ_TAKEABLE | OBJ_CONTAINER,  /*  52 COFFIN */
    0,  /*  53 GRATE */
    OBJ_TAKEABLE,  /*  54 PUMP */
    OBJ_TAKEABLE,  /*  55 DIAMOND */
    OBJ_TAKEABLE,  /*  56 JADE */
    OBJ_TAKEABLE | OBJ_WEAPON,  /*  57 KNIFE */
    OBJ_TAKEABLE,  /*  58 BONES */
    OBJ_TAKEABLE,  /*  59 BURNED-OUT-LANTERN */
    OBJ_TAKEABLE,  /*  60 BAG-OF-COINS */
    OBJ_TAKEABLE | OBJ_LIGHT,  /*  61 LAMP */
    OBJ_TAKEABLE,  /*  62 EMERALD */
    OBJ_TAKEABLE,  /*  63 ADVERTISEMENT */
    0,  /*  64 LEAK */
    OBJ_TAKEABLE | OBJ_CONTAINER,  /*  65 MACHINE */
    OBJ_TAKEABLE | OBJ_OPEN,  /*  66 INFLATED-BOAT */
    OBJ_TAKEABLE | OBJ_CONTAINER,  /*  67 MAILBOX */
    OBJ_TAKEABLE,  /*  68 MATCH */
    OBJ_TAKEABLE,  /*  69 MIRROR-2 */
    OBJ_TAKEABLE,  /*  70 MIRROR-1 */
    OBJ_TAKEABLE,  /*  71 PAINTING */
    OBJ_TAKEABLE | OBJ_LIGHT,  /*  72 CANDLES */
    OBJ_TAKEABLE,  /*  73 GUNK */
    OBJ_TAKEABLE,  /*  74 BODIES */
    OBJ_TAKEABLE,  /*  75 LEAVES */
    OBJ_TAKEABLE,  /*  76 PUNCTURED-BOAT */
    OBJ_TAKEABLE,  /*  77 INFLATABLE-BOAT */
    OBJ_TAKEABLE,  /*  78 BAR */
    OBJ_TAKEABLE,  /*  79 POT-OF-GOLD */
    0,  /*  80 PRAYER */
    0,  /*  81 RAILING */
    0,  /*  82 RAINBOW */
    0,  /*  83 RIVER */
    OBJ_TAKEABLE | OBJ_CONTAINER,  /*  84 BUOY */
    OBJ_TAKEABLE,  /*  85 ROPE */
    OBJ_TAKEABLE | OBJ_WEAPON,  /*  86 RUSTY-KNIFE */
    0,  /*  87 SAND */
    OBJ_TAKEABLE,  /*  88 BRACELET */
    OBJ_TAKEABLE,  /*  89 SCREWDRIVER */
    OBJ_TAKEABLE,  /*  90 KEYS */
    OBJ_TAKEABLE,  /*  91 SHOVEL */
    OBJ_TAKEABLE,  /*  92 COAL */
    0,  /*  93 LADDER */
    OBJ_TAKEABLE,  /*  94 SCARAB */
    OBJ_TAKEABLE,  /*  95 LARGE-BAG */
    OBJ_TAKEABLE | OBJ_WEAPON,  /*  96 STILETTO */
    0,  /*  97 MACHINE-SWITCH */
    0,  /*  98 WOODEN-DOOR */
    OBJ_TAKEABLE | OBJ_WEAPON,  /*  99 SWORD */
    OBJ_TAKEABLE,  /* 100 MAP */
    OBJ_TAKEABLE,  /* 101 BOAT-LABEL */
    OBJ_TAKEABLE | OBJ_CONTAINER | OBJ_OPEN,  /* 102 THIEF */
    OBJ_CONTAINER | OBJ_OPEN,  /* 103 PEDESTAL */
    OBJ_TAKEABLE | OBJ_LIGHT,  /* 104 TORCH */
    OBJ_TAKEABLE,  /* 105 GUIDE */
    OBJ_TAKEABLE | OBJ_OPEN,  /* 106 TROLL */
    OBJ_TAKEABLE,  /* 107 TRUNK */
    OBJ_TAKEABLE | OBJ_CONTAINER,  /* 108 TUBE */
    OBJ_TAKEABLE,  /* 109 PUTTY */
    0,  /* 110 ENGRAVINGS */
    OBJ_TAKEABLE,  /* 111 OWNERS-MANUAL */
    0,  /* 112 CLIMBABLE-CLIFF */
    0,  /* 113 WHITE-CLIFF */
    OBJ_TAKEABLE,  /* 114 WRENCH */
    0,  /* 115 CONTROL-PANEL */
    OBJ_TAKEABLE | OBJ_CONTAINER | OBJ_OPEN,  /* 116 NEST */
    OBJ_TAKEABLE | OBJ_CONTAINER,  /* 117 EGG */
    OBJ_TAKEABLE | OBJ_CONTAINER | OBJ_OPEN,  /* 118 BROKEN-EGG */
    OBJ_TAKEABLE,  /* 119 BAUBLE */
    OBJ_TAKEABLE,  /* 120 CANARY */
    OBJ_TAKEABLE,  /* 121 BROKEN-CANARY */
};

const u8 g_obj_score[NUM_OBJECTS] = {
    0,  /*   0 BOARD */
    0,  /*   1 TEETH */
    0,  /*   2 WALL */
    0,  /*   3 GRANITE-WALL */
    0,  /*   4 SONGBIRD */
    0,  /*   5 WHITE-HOUSE */
    0,  /*   6 FOREST */
    0,  /*   7 TREE */
    0,  /*   8 MOUNTAIN-RANGE */
    0,  /*   9 GLOBAL-WATER */
    0,  /*  10 WATER */
    OBJ_OPEN | OBJ_CONTAINER | OBJ_NODESC,  /*  11 KITCHEN-WINDOW */
    0,  /*  12 CHIMNEY */
    0,  /*  13 GHOSTS */
    20,  /*  14 SKULL */
    0,  /*  15 LOWERED-BASKET */
    0,  /*  16 RAISED-BASKET */
    0,  /*  17 LUNCH */
    0,  /*  18 BAT */
    0,  /*  19 BELL */
    0,  /*  20 HOT-BELL */
    0,  /*  21 AXE */
    0,  /*  22 BOLT */
    0,  /*  23 BUBBLE */
    0,  /*  24 ALTAR */
    0,  /*  25 BOOK */
    0,  /*  26 BROKEN-LAMP */
    10,  /*  27 SCEPTRE */
    0,  /*  28 TIMBERS */
    0,  /*  29 SLIDE */
    0,  /*  30 KITCHEN-TABLE */
    0,  /*  31 ATTIC-TABLE */
    0,  /*  32 SANDWICH-BAG */
    0,  /*  33 TOOL-CHEST */
    0,  /*  34 YELLOW-BUTTON */
    0,  /*  35 BROWN-BUTTON */
    0,  /*  36 RED-BUTTON */
    0,  /*  37 BLUE-BUTTON */
    0,  /*  38 TROPHY-CASE */
    0,  /*  39 RUG */
    15,  /*  40 CHALICE */
    0,  /*  41 GARLIC */
    15,  /*  42 TRIDENT */
    0,  /*  43 CYCLOPS */
    0,  /*  44 DAM */
    0,  /*  45 TRAP-DOOR */
    OBJ_NODESC,  /*  46 BOARDED-WINDOW */
    OBJ_NODESC,  /*  47 FRONT-DOOR */
    0,  /*  48 BARROW-DOOR */
    0,  /*  49 BARROW */
    0,  /*  50 BOTTLE */
    0,  /*  51 CRACK */
    25,  /*  52 COFFIN */
    0,  /*  53 GRATE */
    0,  /*  54 PUMP */
    20,  /*  55 DIAMOND */
    10,  /*  56 JADE */
    0,  /*  57 KNIFE */
    0,  /*  58 BONES */
    0,  /*  59 BURNED-OUT-LANTERN */
    15,  /*  60 BAG-OF-COINS */
    0,  /*  61 LAMP */
    15,  /*  62 EMERALD */
    0,  /*  63 ADVERTISEMENT */
    0,  /*  64 LEAK */
    0,  /*  65 MACHINE */
    0,  /*  66 INFLATED-BOAT */
    0,  /*  67 MAILBOX */
    0,  /*  68 MATCH */
    0,  /*  69 MIRROR-2 */
    0,  /*  70 MIRROR-1 */
    10,  /*  71 PAINTING */
    0,  /*  72 CANDLES */
    0,  /*  73 GUNK */
    0,  /*  74 BODIES */
    0,  /*  75 LEAVES */
    0,  /*  76 PUNCTURED-BOAT */
    0,  /*  77 INFLATABLE-BOAT */
    15,  /*  78 BAR */
    20,  /*  79 POT-OF-GOLD */
    0,  /*  80 PRAYER */
    0,  /*  81 RAILING */
    0,  /*  82 RAINBOW */
    0,  /*  83 RIVER */
    0,  /*  84 BUOY */
    0,  /*  85 ROPE */
    0,  /*  86 RUSTY-KNIFE */
    0,  /*  87 SAND */
    10,  /*  88 BRACELET */
    0,  /*  89 SCREWDRIVER */
    0,  /*  90 KEYS */
    0,  /*  91 SHOVEL */
    0,  /*  92 COAL */
    0,  /*  93 LADDER */
    10,  /*  94 SCARAB */
    0,  /*  95 LARGE-BAG */
    0,  /*  96 STILETTO */
    0,  /*  97 MACHINE-SWITCH */
    0,  /*  98 WOODEN-DOOR */
    0,  /*  99 SWORD */
    0,  /* 100 MAP */
    0,  /* 101 BOAT-LABEL */
    0,  /* 102 THIEF */
    0,  /* 103 PEDESTAL */
    20,  /* 104 TORCH */
    0,  /* 105 GUIDE */
    0,  /* 106 TROLL */
    20,  /* 107 TRUNK */
    0,  /* 108 TUBE */
    0,  /* 109 PUTTY */
    0,  /* 110 ENGRAVINGS */
    0,  /* 111 OWNERS-MANUAL */
    0,  /* 112 CLIMBABLE-CLIFF */
    0,  /* 113 WHITE-CLIFF */
    0,  /* 114 WRENCH */
    0,  /* 115 CONTROL-PANEL */
    0,  /* 116 NEST */
    10,  /* 117 EGG */
    2,  /* 118 BROKEN-EGG */
    2,  /* 119 BAUBLE */
    10,  /* 120 CANARY */
    1,  /* 121 BROKEN-CANARY */
};

const u8 g_obj_target[NUM_OBJECTS] = {
    NO_CONTAINER,  /*   0 BOARD */
    NO_CONTAINER,  /*   1 TEETH */
    NO_CONTAINER,  /*   2 WALL */
    NO_CONTAINER,  /*   3 GRANITE-WALL */
    NO_CONTAINER,  /*   4 SONGBIRD */
    NO_CONTAINER,  /*   5 WHITE-HOUSE */
    NO_CONTAINER,  /*   6 FOREST */
    NO_CONTAINER,  /*   7 TREE */
    NO_CONTAINER,  /*   8 MOUNTAIN-RANGE */
    NO_CONTAINER,  /*   9 GLOBAL-WATER */
    NO_CONTAINER,  /*  10 WATER */
    NO_CONTAINER,  /*  11 KITCHEN-WINDOW */
    NO_CONTAINER,  /*  12 CHIMNEY */
    NO_CONTAINER,  /*  13 GHOSTS */
    OBJ_TROPHY_CASE,  /*  14 SKULL */
    NO_CONTAINER,  /*  15 LOWERED-BASKET */
    NO_CONTAINER,  /*  16 RAISED-BASKET */
    NO_CONTAINER,  /*  17 LUNCH */
    NO_CONTAINER,  /*  18 BAT */
    NO_CONTAINER,  /*  19 BELL */
    NO_CONTAINER,  /*  20 HOT-BELL */
    NO_CONTAINER,  /*  21 AXE */
    NO_CONTAINER,  /*  22 BOLT */
    NO_CONTAINER,  /*  23 BUBBLE */
    NO_CONTAINER,  /*  24 ALTAR */
    NO_CONTAINER,  /*  25 BOOK */
    NO_CONTAINER,  /*  26 BROKEN-LAMP */
    OBJ_TROPHY_CASE,  /*  27 SCEPTRE */
    NO_CONTAINER,  /*  28 TIMBERS */
    NO_CONTAINER,  /*  29 SLIDE */
    NO_CONTAINER,  /*  30 KITCHEN-TABLE */
    NO_CONTAINER,  /*  31 ATTIC-TABLE */
    NO_CONTAINER,  /*  32 SANDWICH-BAG */
    NO_CONTAINER,  /*  33 TOOL-CHEST */
    NO_CONTAINER,  /*  34 YELLOW-BUTTON */
    NO_CONTAINER,  /*  35 BROWN-BUTTON */
    NO_CONTAINER,  /*  36 RED-BUTTON */
    NO_CONTAINER,  /*  37 BLUE-BUTTON */
    NO_CONTAINER,  /*  38 TROPHY-CASE */
    NO_CONTAINER,  /*  39 RUG */
    OBJ_TROPHY_CASE,  /*  40 CHALICE */
    NO_CONTAINER,  /*  41 GARLIC */
    OBJ_TROPHY_CASE,  /*  42 TRIDENT */
    NO_CONTAINER,  /*  43 CYCLOPS */
    NO_CONTAINER,  /*  44 DAM */
    NO_CONTAINER,  /*  45 TRAP-DOOR */
    NO_CONTAINER,  /*  46 BOARDED-WINDOW */
    NO_CONTAINER,  /*  47 FRONT-DOOR */
    NO_CONTAINER,  /*  48 BARROW-DOOR */
    NO_CONTAINER,  /*  49 BARROW */
    NO_CONTAINER,  /*  50 BOTTLE */
    NO_CONTAINER,  /*  51 CRACK */
    OBJ_TROPHY_CASE,  /*  52 COFFIN */
    NO_CONTAINER,  /*  53 GRATE */
    NO_CONTAINER,  /*  54 PUMP */
    OBJ_TROPHY_CASE,  /*  55 DIAMOND */
    OBJ_TROPHY_CASE,  /*  56 JADE */
    NO_CONTAINER,  /*  57 KNIFE */
    NO_CONTAINER,  /*  58 BONES */
    NO_CONTAINER,  /*  59 BURNED-OUT-LANTERN */
    OBJ_TROPHY_CASE,  /*  60 BAG-OF-COINS */
    NO_CONTAINER,  /*  61 LAMP */
    OBJ_TROPHY_CASE,  /*  62 EMERALD */
    NO_CONTAINER,  /*  63 ADVERTISEMENT */
    NO_CONTAINER,  /*  64 LEAK */
    NO_CONTAINER,  /*  65 MACHINE */
    NO_CONTAINER,  /*  66 INFLATED-BOAT */
    NO_CONTAINER,  /*  67 MAILBOX */
    NO_CONTAINER,  /*  68 MATCH */
    NO_CONTAINER,  /*  69 MIRROR-2 */
    NO_CONTAINER,  /*  70 MIRROR-1 */
    OBJ_TROPHY_CASE,  /*  71 PAINTING */
    NO_CONTAINER,  /*  72 CANDLES */
    NO_CONTAINER,  /*  73 GUNK */
    NO_CONTAINER,  /*  74 BODIES */
    NO_CONTAINER,  /*  75 LEAVES */
    NO_CONTAINER,  /*  76 PUNCTURED-BOAT */
    NO_CONTAINER,  /*  77 INFLATABLE-BOAT */
    OBJ_TROPHY_CASE,  /*  78 BAR */
    OBJ_TROPHY_CASE,  /*  79 POT-OF-GOLD */
    NO_CONTAINER,  /*  80 PRAYER */
    NO_CONTAINER,  /*  81 RAILING */
    NO_CONTAINER,  /*  82 RAINBOW */
    NO_CONTAINER,  /*  83 RIVER */
    NO_CONTAINER,  /*  84 BUOY */
    NO_CONTAINER,  /*  85 ROPE */
    NO_CONTAINER,  /*  86 RUSTY-KNIFE */
    NO_CONTAINER,  /*  87 SAND */
    OBJ_TROPHY_CASE,  /*  88 BRACELET */
    NO_CONTAINER,  /*  89 SCREWDRIVER */
    NO_CONTAINER,  /*  90 KEYS */
    NO_CONTAINER,  /*  91 SHOVEL */
    NO_CONTAINER,  /*  92 COAL */
    NO_CONTAINER,  /*  93 LADDER */
    OBJ_TROPHY_CASE,  /*  94 SCARAB */
    NO_CONTAINER,  /*  95 LARGE-BAG */
    NO_CONTAINER,  /*  96 STILETTO */
    NO_CONTAINER,  /*  97 MACHINE-SWITCH */
    NO_CONTAINER,  /*  98 WOODEN-DOOR */
    NO_CONTAINER,  /*  99 SWORD */
    NO_CONTAINER,  /* 100 MAP */
    NO_CONTAINER,  /* 101 BOAT-LABEL */
    NO_CONTAINER,  /* 102 THIEF */
    NO_CONTAINER,  /* 103 PEDESTAL */
    OBJ_TROPHY_CASE,  /* 104 TORCH */
    NO_CONTAINER,  /* 105 GUIDE */
    NO_CONTAINER,  /* 106 TROLL */
    OBJ_TROPHY_CASE,  /* 107 TRUNK */
    NO_CONTAINER,  /* 108 TUBE */
    NO_CONTAINER,  /* 109 PUTTY */
    NO_CONTAINER,  /* 110 ENGRAVINGS */
    NO_CONTAINER,  /* 111 OWNERS-MANUAL */
    NO_CONTAINER,  /* 112 CLIMBABLE-CLIFF */
    NO_CONTAINER,  /* 113 WHITE-CLIFF */
    NO_CONTAINER,  /* 114 WRENCH */
    NO_CONTAINER,  /* 115 CONTROL-PANEL */
    NO_CONTAINER,  /* 116 NEST */
    OBJ_TROPHY_CASE,  /* 117 EGG */
    OBJ_TROPHY_CASE,  /* 118 BROKEN-EGG */
    OBJ_TROPHY_CASE,  /* 119 BAUBLE */
    OBJ_TROPHY_CASE,  /* 120 CANARY */
    OBJ_TROPHY_CASE,  /* 121 BROKEN-CANARY */
};
