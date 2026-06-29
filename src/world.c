/* world.c - Zork Neo: World data
 * Room descriptions and object data derived from Zork I ZIL source
 * MIT License - (c) Infocom / Microsoft 2025 open source grant
 * Faithfully adapted for NGPC 20-char display width
 */
#include "zork_types.h"

/* ---- Verb name table ---- */
char *g_verb_names[NUM_VERBS] = {
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
    "QUIT"
};

/* ---- Room table (ROM) ---- */
/* exits[] order: N S E W UP DOWN */
/* NO_EXIT = 0xFF */

Room g_rooms[NUM_ROOMS] = {
    /* 0: WEST_OF_HOUSE */
    {
        "West of House",
        "You are standing in an open\n"
        "field west of a white house,\n"
        "with a boarded front door.\n"
        "There is a small mailbox here.",
        { ROOM_NORTH_OF_HOUSE, ROOM_SOUTH_OF_HOUSE,
          NO_EXIT, NO_EXIT, NO_EXIT, NO_EXIT },
        ROOM_ABOVE | ROOM_VISITED  /* start room, always shown long */
    },
    /* 1: NORTH_OF_HOUSE */
    {
        "North of House",
        "You are facing the north side\n"
        "of a white house. There is no\n"
        "door here, and all the windows\n"
        "are boarded up. To the north a\n"
        "forest path leads away.",
        { NO_EXIT, ROOM_WEST_OF_HOUSE,
          ROOM_BEHIND_HOUSE, NO_EXIT, NO_EXIT, NO_EXIT },
        ROOM_ABOVE
    },
    /* 2: SOUTH_OF_HOUSE */
    {
        "South of House",
        "You are facing the south side\n"
        "of a white house. There is no\n"
        "door here, and all the windows\n"
        "are boarded shut.",
        { ROOM_WEST_OF_HOUSE, NO_EXIT,
          ROOM_BEHIND_HOUSE, NO_EXIT, NO_EXIT, NO_EXIT },
        ROOM_ABOVE
    },
    /* 3: BEHIND_HOUSE */
    {
        "Behind House",
        "You are behind the white house.\n"
        "A path leads into the forest\n"
        "to the east. In one corner of\n"
        "the house there is a small\n"
        "window which is slightly open.",
        { ROOM_NORTH_OF_HOUSE, ROOM_SOUTH_OF_HOUSE,
          ROOM_CLEARING, NO_EXIT, NO_EXIT, NO_EXIT },
        ROOM_ABOVE
    },
    /* 4: KITCHEN */
    {
        "Kitchen",
        "You are in the kitchen of the\n"
        "white house. A table seems to\n"
        "have been used recently for\n"
        "the preparation of food. A\n"
        "passage leads to the west, and\n"
        "a dark staircase can be seen\n"
        "leading upward. A window looks\n"
        "out to the east.",
        { NO_EXIT, NO_EXIT,
          NO_EXIT, ROOM_LIVING_ROOM, ROOM_ATTIC, NO_EXIT },
        0
    },
    /* 5: LIVING_ROOM */
    {
        "Living Room",
        "You are in the living room.\n"
        "There is a door to the east,\n"
        "a wooden door with strange\n"
        "gothic lettering to the west,\n"
        "which appears to be nailed\n"
        "shut. A trophy case stands\n"
        "against the wall. A large\n"
        "oriental rug is in the center.",
        { NO_EXIT, NO_EXIT,
          ROOM_KITCHEN, NO_EXIT, NO_EXIT, ROOM_CELLAR },
        0
    },
    /* 6: ATTIC */
    {
        "Attic",
        "This is the attic. The only\n"
        "exit is a stairway leading\n"
        "down. A large coil of rope is\n"
        "here.",
        { NO_EXIT, NO_EXIT,
          NO_EXIT, NO_EXIT, NO_EXIT, ROOM_KITCHEN },
        ROOM_DARK
    },
    /* 7: CELLAR */
    {
        "Cellar",
        "You are in a dark and damp\n"
        "cellar with a narrow passageway\n"
        "leading north, and a crawlway\n"
        "to the south. To the west is\n"
        "the bottom of a steep metal\n"
        "ramp which is unclimbable.",
        { ROOM_EAST_PASSAGE, NO_EXIT,
          NO_EXIT, NO_EXIT, ROOM_LIVING_ROOM, NO_EXIT },
        ROOM_DARK
    },
    /* 8: EAST_PASSAGE */
    {
        "East Passage",
        "This is a narrow east-west\n"
        "passage. At the east end is a\n"
        "round room.",
        { NO_EXIT, NO_EXIT,
          ROOM_ROUND_ROOM, NO_EXIT, NO_EXIT, NO_EXIT },
        ROOM_DARK
    },
    /* 9: ROUND_ROOM */
    {
        "Round Room",
        "You are in a circular room\n"
        "with passages leading north,\n"
        "south, east, west, and\n"
        "northeast. In the center is\n"
        "a large circular pit descending\n"
        "into darkness.",
        { ROOM_NORTH_SOUTH_PASS, NO_EXIT,
          NO_EXIT, ROOM_EAST_PASSAGE, NO_EXIT, NO_EXIT },
        ROOM_DARK
    },
    /* 10: NORTH_SOUTH_PASS */
    {
        "North-South Passage",
        "This is a long north-south\n"
        "passage. There are exits to\n"
        "the east and west.",
        { NO_EXIT, ROOM_ROUND_ROOM,
          ROOM_EW_PASSAGE, NO_EXIT, NO_EXIT, NO_EXIT },
        ROOM_DARK
    },
    /* 11: EW_PASSAGE */
    {
        "East-West Passage",
        "This is a narrow east-west\n"
        "passage with a T-intersection\n"
        "to the north.",
        { ROOM_GALLERY, NO_EXIT,
          NO_EXIT, ROOM_NORTH_SOUTH_PASS, NO_EXIT, NO_EXIT },
        ROOM_DARK
    },
    /* 12: LOUD_ROOM */
    {
        "Loud Room",
        "This is a large room with a\n"
        "deafening roar from the east.\n"
        "A large echo reverberates\n"
        "around you. There is a brass\n"
        "bauble here.",
        { NO_EXIT, NO_EXIT,
          NO_EXIT, ROOM_ROUND_ROOM, NO_EXIT, NO_EXIT },
        ROOM_DARK
    },
    /* 13: GALLERY */
    {
        "Gallery",
        "This is an art gallery. Most\n"
        "of the paintings have been\n"
        "stolen by vandals with great\n"
        "taste. The vandals' most recent\n"
        "acquisition is an oil painting\n"
        "hanging in the east wall.",
        { ROOM_STUDIO, NO_EXIT,
          NO_EXIT, ROOM_EW_PASSAGE, NO_EXIT, NO_EXIT },
        ROOM_DARK
    },
    /* 14: STUDIO */
    {
        "Studio",
        "This is an artist's studio.\n"
        "Paintings are scattered about\n"
        "the room. There is an easel\n"
        "with a canvas in the center.\n"
        "A narrow door leads west.",
        { NO_EXIT, ROOM_GALLERY,
          NO_EXIT, NO_EXIT, NO_EXIT, NO_EXIT },
        ROOM_DARK
    },
    /* 15: CAVE */
    {
        "Cave",
        "You are in a small cave with\n"
        "passages leading northeast and\n"
        "south.",
        { NO_EXIT, ROOM_CELLAR,
          NO_EXIT, NO_EXIT, NO_EXIT, NO_EXIT },
        ROOM_DARK
    },
    /* 16-18: MAZE (all alike, disorienting) */
    {
        "Maze",
        "You are in a maze of twisty\n"
        "little passages, all alike.",
        { ROOM_MAZE_2, ROOM_MAZE_3,
          ROOM_MAZE_1, ROOM_MAZE_2, NO_EXIT, NO_EXIT },
        ROOM_DARK
    },
    {
        "Maze",
        "You are in a maze of twisty\n"
        "little passages, all alike.",
        { ROOM_MAZE_3, ROOM_MAZE_1,
          ROOM_MAZE_2, ROOM_MAZE_3, NO_EXIT, ROOM_CELLAR },
        ROOM_DARK
    },
    {
        "Maze",
        "You are in a maze of twisty\n"
        "little passages, all alike.",
        { ROOM_MAZE_1, ROOM_MAZE_2,
          ROOM_MAZE_3, ROOM_MAZE_1, NO_EXIT, NO_EXIT },
        ROOM_DARK
    },
    /* 19: GRATING_CLEARING */
    {
        "Grating Clearing",
        "You are in a clearing. A\n"
        "grating is set into the ground.",
        { NO_EXIT, ROOM_FOREST_1,
          NO_EXIT, ROOM_FOREST_2, NO_EXIT, NO_EXIT },
        ROOM_ABOVE
    },
    /* 20: FOREST_1 */
    {
        "Forest",
        "This is a forest with no path,\n"
        "with trees to the east.",
        { ROOM_GRATING_CLEARING, ROOM_FOREST_2,
          ROOM_FOREST_PATH, ROOM_FOREST_1, NO_EXIT, NO_EXIT },
        ROOM_ABOVE
    },
    /* 21: FOREST_2 */
    {
        "Forest",
        "This is a forest with no path,\n"
        "with trees to the east.",
        { ROOM_FOREST_1, NO_EXIT,
          ROOM_FOREST_PATH, ROOM_CLEARING, NO_EXIT, NO_EXIT },
        ROOM_ABOVE
    },
    /* 22: FOREST_PATH */
    {
        "Forest Path",
        "This is a path through the\n"
        "forest. Gaps between the trees\n"
        "to the northeast and southwest\n"
        "look like paths. A path leads\n"
        "northwest.",
        { ROOM_NORTH_OF_HOUSE, ROOM_FOREST_1,
          NO_EXIT, ROOM_FOREST_2, NO_EXIT, NO_EXIT },
        ROOM_ABOVE
    },
    /* 23: CLEARING */
    {
        "Clearing",
        "You are in a clearing, with a\n"
        "forest to the west and north,\n"
        "and a large house to the east.",
        { NO_EXIT, NO_EXIT,
          ROOM_BEHIND_HOUSE, ROOM_FOREST_2, NO_EXIT, NO_EXIT },
        ROOM_ABOVE
    }
};

/* ---- Object table (ROM) ---- */
/* Mutable fields (location, flags) are in g_obj_loc[] / g_obj_flags[]
 * which get flash-saved. Only name, description, score_value here. */

Object g_objects[NUM_OBJECTS] = {
    /* 0: MAILBOX */
    {
        "mailbox",
        "The small mailbox is\n"
        "mounted on a wooden post.\n"
        "It is",     /* completed at runtime: "open/closed" */
        ROOM_WEST_OF_HOUSE,
        OBJ_CONTAINER,          /* flags */
        0,                      /* score value */
        0
    },
    /* 1: LEAFLET */
    {
        "leaflet",
        "\"WELCOME TO ZORK!\n\n"
        "ZORK is a game of adventure,\n"
        "danger and low cunning. In it\n"
        "you will explore some of the\n"
        "most amazing territory ever\n"
        "seen by mortal man. Hardened\n"
        "adventurers have run screaming\n"
        "from the terrors within!\n\n"
        "In ZORK, the goal is to\n"
        "collect treasures.\"",
        OBJ_MAILBOX,            /* inside mailbox initially */
        OBJ_TAKEABLE,
        0,
        0
    },
    /* 2: DOOR_W (west house door) */
    {
        "door",
        "The door to the white house\n"
        "is boarded up.",
        ROOM_WEST_OF_HOUSE,
        0,
        0,
        0
    },
    /* 3: MAT */
    {
        "mat",
        "It's a mat. It says:\n"
        "\"WELCOME TO ZORK\"",
        ROOM_BEHIND_HOUSE,
        OBJ_TAKEABLE,
        0,
        0
    },
    /* 4: LANTERN */
    {
        "lantern",
        "The brass lantern is an\n"
        "ordinary battery-powered lamp.\n"
        "It is currently",   /* completed at runtime: on/off */
        ROOM_LIVING_ROOM,
        OBJ_TAKEABLE | OBJ_LIGHT,
        0,
        0
    },
    /* 5: SWORD */
    {
        "sword",
        "The elvish sword of great\n"
        "antiquity has a name inscribed\n"
        "on its hilt: \"Elvish Sword\".",
        ROOM_LIVING_ROOM,
        OBJ_TAKEABLE | OBJ_WEAPON,
        0,
        0
    },
    /* 6: TROPHY_CASE */
    {
        "trophy case",
        "The trophy case is a large\n"
        "wooden case mounted on the\n"
        "west wall. It is",
        ROOM_LIVING_ROOM,
        OBJ_CONTAINER,
        0,
        0
    },
    /* 7: PAINTING */
    {
        "painting",
        "The painting is a valuable\n"
        "oil painting. The scene\n"
        "depicts a dark forest.",
        ROOM_GALLERY,
        OBJ_TAKEABLE,
        100,
        OBJ_TROPHY_CASE
    },
    /* 8: SACK */
    {
        "brown sack",
        "The brown sack is a small\n"
        "brown bag.",
        ROOM_KITCHEN,
        OBJ_TAKEABLE | OBJ_CONTAINER | OBJ_OPEN,
        0,
        0
    },
    /* 9: GARLIC */
    {
        "clove of garlic",
        "I'm afraid the garlic smells\n"
        "absolutely foul.",
        OBJ_SACK,    /* inside sack */
        OBJ_TAKEABLE,
        0,
        0
    },
    /* 10: BOTTLE */
    {
        "glass bottle",
        "The glass bottle is half full\n"
        "of water.",
        ROOM_KITCHEN,
        OBJ_TAKEABLE | OBJ_CONTAINER | OBJ_OPEN,
        0,
        0
    },
    /* 11: WATER */
    {
        "quantity of water",
        "It's water. Wet stuff.",
        OBJ_BOTTLE,
        0,
        0,
        0
    },
    /* 12: KNIFE */
    {
        "rusty knife",
        "The knife is fairly dull and\n"
        "rusty.",
        ROOM_KITCHEN,
        OBJ_TAKEABLE | OBJ_WEAPON,
        0,
        0
    },
    /* 13: CANARY */
    {
        "golden canary",
        "A small golden canary in the\n"
        "shape of a bird. It looks\n"
        "valuable.",
        ROOM_STUDIO,
        OBJ_TAKEABLE,
        150,
        OBJ_TROPHY_CASE
    },
    /* 14: COFFIN */
    {
        "coffin",
        "The coffin is an ornate\n"
        "box made of mahogany. It is",
        ROOM_STUDIO,
        OBJ_CONTAINER | OBJ_LOCKED,
        0,
        0
    },
    /* 15: SKULL */
    {
        "crystal skull",
        "The skull appears to be\n"
        "made of crystal.",
        OBJ_COFFIN,
        OBJ_TAKEABLE,
        200,
        OBJ_TROPHY_CASE
    },
    /* 16: ROPE */
    {
        "coil of rope",
        "It's a strong rope, about\n"
        "fifty feet long.",
        ROOM_ATTIC,
        OBJ_TAKEABLE,
        0,
        0
    },
    /* 17: COAL */
    {
        "lump of coal",
        "It's a lump of coal.",
        ROOM_CAVE,
        OBJ_TAKEABLE,
        0,
        0
    },
    /* 18: TORCH */
    {
        "torch",
        "The torch is a crude club\n"
        "with a rag soaked in oil\n"
        "tied to one end. It is",
        ROOM_CAVE,
        OBJ_TAKEABLE | OBJ_LIGHT,
        0,
        0
    },
    /* 19: GOLD_COIN */
    {
        "gold coin",
        "The coin has a picture of\n"
        "the king on one side and\n"
        "a grue on the other.",
        ROOM_CELLAR,
        OBJ_TAKEABLE,
        10,
        OBJ_TROPHY_CASE
    },
    /* 20: PLATINUM_BAR */
    {
        "platinum bar",
        "The platinum bar is a bar\n"
        "of pure platinum.",
        ROOM_MAZE_1,
        OBJ_TAKEABLE,
        50,
        OBJ_TROPHY_CASE
    },
    /* 21: POT_OF_GOLD */
    {
        "pot of gold",
        "The pot is filled with\n"
        "gold coins.",
        ROOM_CAVE,
        OBJ_TAKEABLE,
        100,
        OBJ_TROPHY_CASE
    },
    /* 22: JEWELS */
    {
        "jewels",
        "A heap of coloured gems,\n"
        "including rubies, diamonds,\n"
        "and emeralds.",
        ROOM_EW_PASSAGE,
        OBJ_TAKEABLE,
        50,
        OBJ_TROPHY_CASE
    },
    /* 23: DIAMOND */
    {
        "diamond",
        "It is a huge diamond. It\n"
        "glitters!",
        ROOM_LOUD_ROOM,
        OBJ_TAKEABLE,
        100,
        OBJ_TROPHY_CASE
    },
    /* 24: EMERALD */
    {
        "emerald",
        "The emerald is the size of\n"
        "a plum and of remarkable\n"
        "clarity.",
        ROOM_STUDIO,
        OBJ_TAKEABLE,
        50,
        OBJ_TROPHY_CASE
    },
    /* 25: RUBY */
    {
        "huge ruby",
        "A giant ruby. Priceless.",
        ROOM_MAZE_3,
        OBJ_TAKEABLE,
        50,
        OBJ_TROPHY_CASE
    },
    /* 26: SCREWDRIVER */
    {
        "screwdriver",
        "It's a Zork-brand screwdriver.",
        ROOM_STUDIO,
        OBJ_TAKEABLE | OBJ_WEAPON,
        0,
        0
    },
    /* 27: PUMP */
    {
        "pump",
        "A small orange pump.",
        ROOM_CAVE,
        OBJ_TAKEABLE,
        0,
        0
    },
    /* 28: AIR_PUMP */
    {
        "air pump",
        "A foot-operated air pump.",
        ROOM_CELLAR,
        0,
        0,
        0
    },
    /* 29: CRYSTAL_SKULL */
    {
        "crystal skull",
        "The skull is made entirely\n"
        "of crystal. Looking through\n"
        "it is dizzying.",
        LOC_GONE,
        OBJ_TAKEABLE,
        300,
        OBJ_TROPHY_CASE
    }
};
