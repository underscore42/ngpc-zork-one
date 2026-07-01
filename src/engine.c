/* engine.c - Zork Neo: Game logic
 * Uses flat arrays only - no struct member access
 * Updated for full 110-room / 122-object world data
 */
#include "zork_types.h"
#include "text.h"
#include "zork_save.h"
#include "wizard.h"

/* Mutable state */
u8  g_player_room;
u16 g_score;
u16 g_moves;
u8  g_lamp_fuel;
u8  g_dead;
u8  g_troll_alive;
u8  g_thief_alive;
u8  g_cyclops_here;
u8  g_obj_loc[NUM_OBJECTS];
u8  g_obj_flags[NUM_OBJECTS];
u8  g_room_flags[NUM_ROOMS];

/* ---- Helpers ---- */

static u8 room_is_lit(u8 rid) {
    u8 i;
    if (wizard_is_lit()) return 1;
    if (g_room_flags[rid] & ROOM_ABOVE) return 1;
    for (i = 0; i < NUM_OBJECTS; i++) {
        if (!(g_obj_flags[i] & OBJ_LIGHT)) continue;
        if (!(g_obj_flags[i] & OBJ_LIT))  continue;
        if (g_obj_loc[i] == rid)           return 1;
        if (g_obj_loc[i] == LOC_PLAYER)    return 1;
    }
    return 0;
}

static u8 player_has(u8 obj) {
    return g_obj_loc[obj] == LOC_PLAYER;
}

static u8 obj_accessible(u8 obj) {
    u8 loc;
    u8 cont;
    loc = g_obj_loc[obj];
    if (loc == LOC_GONE) return 0;
    if (loc == g_player_room) return 1;
    if (loc == LOC_PLAYER) return 1;
    /* Inside open container? Check if loc is a container object */
    if (loc < NUM_OBJECTS && (g_obj_flags[loc] & OBJ_CONTAINER)) {
        cont = loc;
        if (!(g_obj_flags[cont] & OBJ_OPEN)) return 0;
        if (g_obj_loc[cont] == g_player_room) return 1;
        if (g_obj_loc[cont] == LOC_PLAYER)    return 1;
    }
    return 0;
}

static u8 inv_count(void) {
    u8 i;
    u8 n;
    n = 0;
    for (i = 0; i < NUM_OBJECTS; i++) {
        if (g_obj_loc[i] == LOC_PLAYER) n++;
    }
    return n;
}

void engine_add_score(u16 pts) {
    g_score = g_score + pts;
}

/* ---- Room descriptions ---- */
static void engine_print_room_desc(u8 rid) {
    switch(rid) {
        case ROOM_WEST_OF_HOUSE:
            text_println("Open field west of a");
            text_println("white house, boarded");
            text_println("door. Mailbox here.");
            break;
        case ROOM_STONE_BARROW:
            text_println("A massive stone");
            text_println("barrow. A stone door");
            text_println("leads inside.");
            break;
        case ROOM_NORTH_OF_HOUSE:
            text_println("North side of white");
            text_println("house. Windows are");
            text_println("boarded. Path N.");
            break;
        case ROOM_SOUTH_OF_HOUSE:
            text_println("South side of white");
            text_println("house. No door here,");
            text_println("windows boarded.");
            break;
        case ROOM_EAST_OF_HOUSE:
            text_println("Behind the white");
            text_println("house. Path leads E.");
            text_println("A window is open.");
            break;
        case ROOM_FOREST_1:
        case ROOM_FOREST_2:
        case ROOM_FOREST_3:
        case ROOM_MOUNTAINS:
            text_println("Dense forest.");
            text_println("Trees in all");
            text_println("directions.");
            break;
        case ROOM_PATH:
            text_println("Forest path. House");
            text_println("to south, clearing");
            text_println("to north. Tree here.");
            break;
        case ROOM_UP_A_TREE:
            text_println("High in a large tree.");
            text_println("You can see for");
            text_println("miles around.");
            break;
        case ROOM_GRATING_CLEARING:
            text_println("A clearing. An iron");
            text_println("grating is set into");
            text_println("the ground.");
            break;
        case ROOM_CLEARING:
            text_println("Clearing. Forest W,");
            text_println("white house to east.");
            break;
        case ROOM_KITCHEN:
            text_println("The kitchen. Passage");
            text_println("leads west. Stairs");
            text_println("lead up.");
            break;
        case ROOM_ATTIC:
            text_println("The attic. Stairway");
            text_println("leads down.");
            break;
        case ROOM_LIVING_ROOM:
            text_println("Living room. Door E.");
            text_println("Gothic door W nailed");
            text_println("shut. Trophy case");
            if (g_obj_loc[OBJ_RUG] == ROOM_LIVING_ROOM)
                text_println("and oriental rug.");
            else if (g_obj_loc[OBJ_TRAP_DOOR] != LOC_GONE)
                text_println("Trap door in floor!");
            break;
        case ROOM_CELLAR:
            text_println("Dark damp cellar.");
            text_println("Passages lead N, S.");
            text_println("Ramp W, ladder U.");
            break;
        case ROOM_TROLL_ROOM:
            text_println("A small room.");
            if (g_troll_alive) {
                text_println("A nasty troll blocks");
                text_println("the east exit,");
                text_println("waving a bloody axe.");
            } else {
                text_println("A dead troll lies");
                text_println("on the floor.");
            }
            break;
        case ROOM_EAST_OF_CHASM:
            text_println("East of a chasm.");
            text_println("Passage N to cellar,");
            text_println("gallery to east.");
            break;
        case ROOM_GALLERY:
            text_println("Art gallery. Most");
            text_println("paintings stolen.");
            text_println("One painting remains.");
            break;
        case ROOM_STUDIO:
            text_println("Artist studio.");
            text_println("Paintings scattered.");
            break;
        case ROOM_MAZE_1:
        case ROOM_MAZE_2:
        case ROOM_MAZE_3:
        case ROOM_MAZE_4:
        case ROOM_MAZE_5:
        case ROOM_MAZE_6:
        case ROOM_MAZE_7:
        case ROOM_MAZE_8:
        case ROOM_MAZE_9:
        case ROOM_MAZE_10:
        case ROOM_MAZE_11:
        case ROOM_MAZE_12:
        case ROOM_MAZE_13:
        case ROOM_MAZE_14:
        case ROOM_MAZE_15:
            text_println("Maze of twisty");
            text_println("passages, all alike.");
            break;
        case ROOM_DEAD_END_1:
        case ROOM_DEAD_END_2:
        case ROOM_DEAD_END_3:
        case ROOM_DEAD_END_4:
        case ROOM_DEAD_END_5:
            text_println("Dead end.");
            break;
        case ROOM_GRATING_ROOM:
            text_println("Small room. An iron");
            text_println("grating is in the");
            text_println("floor to the SW.");
            break;
        case ROOM_CYCLOPS_ROOM:
            text_println("A low-ceilinged room.");
            if (g_cyclops_here) {
                text_println("A giant one-eyed");
                text_println("cyclops blocks E.");
            } else {
                text_println("The cyclops is gone.");
            }
            break;
        case ROOM_STRANGE_PASSAGE:
            text_println("A winding passage.");
            text_println("East leads to the");
            text_println("living room.");
            break;
        case ROOM_TREASURE_ROOM:
            text_println("The Thief's treasure");
            text_println("room. Passages lead");
            text_println("down and around.");
            break;
        case ROOM_RESERVOIR_SOUTH:
            text_println("South shore of a");
            text_println("large reservoir.");
            break;
        case ROOM_RESERVOIR:
            text_println("On a vast reservoir.");
            text_println("Current pulls you.");
            break;
        case ROOM_RESERVOIR_NORTH:
            text_println("North shore of the");
            text_println("reservoir.");
            break;
        case ROOM_STREAM_VIEW:
            text_println("A narrow ledge above");
            text_println("a rushing stream.");
            break;
        case ROOM_IN_STREAM:
            text_println("You are in the");
            text_println("stream, swept along.");
            break;
        case ROOM_MIRROR_ROOM_1:
        case ROOM_MIRROR_ROOM_2:
            text_println("Large room with a");
            text_println("huge mirror on one");
            text_println("wall.");
            break;
        case ROOM_SMALL_CAVE:
        case ROOM_TINY_CAVE:
            text_println("A small cave.");
            text_println("Passages lead in");
            text_println("several directions.");
            break;
        case ROOM_COLD_PASSAGE:
            text_println("A cold narrow");
            text_println("passage. The air");
            text_println("is icy.");
            break;
        case ROOM_NARROW_PASSAGE:
            text_println("A narrow winding");
            text_println("passage.");
            break;
        case ROOM_WINDING_PASSAGE:
        case ROOM_TWISTING_PASSAGE:
            text_println("A winding passage.");
            break;
        case ROOM_ATLANTIS_ROOM:
            text_println("A magnificent room.");
            text_println("The walls drip with");
            text_println("moisture.");
            break;
        case ROOM_EW_PASSAGE:
            text_println("East-West passage.");
            text_println("Troll room to west,");
            text_println("round room to east.");
            break;
        case ROOM_ROUND_ROOM:
            text_println("A circular room.");
            text_println("Passages lead in");
            text_println("many directions.");
            break;
        case ROOM_DEEP_CANYON:
            text_println("A deep canyon. A");
            text_println("small passage leads");
            text_println("N and S.");
            break;
        case ROOM_DAMP_CAVE:
            text_println("A damp cave.");
            text_println("White cliffs to E,");
            text_println("loud room to W.");
            break;
        case ROOM_LOUD_ROOM:
            text_println("A loud room. The");
            text_println("roar is deafening.");
            text_println("Echo your words!");
            break;
        case ROOM_NS_PASSAGE:
            text_println("N-S passage. Round");
            text_println("room S, chasm N.");
            break;
        case ROOM_CHASM_ROOM:
            text_println("Edge of a chasm.");
            text_println("A railing here.");
            text_println("Passage N, S, E.");
            break;
        case ROOM_ENTRANCE_TO_HADES:
            text_println("Entrance to Hades.");
            text_println("Spirits bar the way");
            text_println("to the south.");
            break;
        case ROOM_LAND_OF_LIVING_DEAD:
            text_println("Land of the Dead.");
            text_println("Spirits drift past.");
            break;
        case ROOM_ENGRAVINGS_CAVE:
            text_println("A cave with ancient");
            text_println("engravings on the");
            text_println("walls.");
            break;
        case ROOM_EGYPT_ROOM:
            text_println("An Egyptian room.");
            text_println("Hieroglyphics cover");
            text_println("the walls.");
            break;
        case ROOM_DOME_ROOM:
            text_println("Atop a large dome.");
            text_println("A staircase spirals");
            text_println("down.");
            break;
        case ROOM_TORCH_ROOM:
            text_println("Large room lit by a");
            text_println("flaming torch set");
            text_println("into the wall.");
            break;
        case ROOM_NORTH_TEMPLE:
            text_println("A damp temple.");
            text_println("An altar to the");
            text_println("south.");
            break;
        case ROOM_SOUTH_TEMPLE:
            text_println("A small altar room.");
            text_println("Candles burn on the");
            text_println("altar.");
            break;
        case ROOM_DAM_ROOM:
            text_println("On a ledge above a");
            text_println("great dam. A stream");
            text_println("flows below.");
            break;
        case ROOM_DAM_LOBBY:
            text_println("Lobby of the dam.");
            text_println("Maintenance room N,");
            text_println("dam to south.");
            break;
        case ROOM_MAINTENANCE_ROOM:
            text_println("Maintenance room.");
            text_println("Tool chests line");
            text_println("the walls.");
            break;
        case ROOM_DAM_BASE:
            text_println("Base of the dam.");
            text_println("The dam towers");
            text_println("above you.");
            break;
        case ROOM_RIVER_1:
        case ROOM_RIVER_2:
        case ROOM_RIVER_3:
        case ROOM_RIVER_4:
        case ROOM_RIVER_5:
            text_println("Frigid River. You");
            text_println("are in a boat.");
            text_println("Current pulls E.");
            break;
        case ROOM_WHITE_CLIFFS_NORTH:
        case ROOM_WHITE_CLIFFS_SOUTH:
            text_println("Beach at the base");
            text_println("of white cliffs.");
            break;
        case ROOM_SHORE:
            text_println("Rocky shore. Falls");
            text_println("to south, beach");
            text_println("to east.");
            break;
        case ROOM_SANDY_BEACH:
            text_println("Sandy beach. Shovel");
            text_println("in the sand?");
            text_println("NE leads further.");
            break;
        case ROOM_SANDY_CAVE:
            text_println("A sandy cave.");
            text_println("SW leads back to");
            text_println("the beach.");
            break;
        case ROOM_ARAGAIN_FALLS:
            text_println("Aragain Falls.");
            text_println("A rainbow spans");
            text_println("the gorge.");
            break;
        case ROOM_ON_RAINBOW:
            text_println("On the rainbow!");
            text_println("The colours swirl");
            text_println("beneath you.");
            break;
        case ROOM_END_OF_RAINBOW:
            text_println("End of the rainbow.");
            text_println("A pot of gold here.");
            break;
        case ROOM_CANYON_BOTTOM:
            text_println("Canyon bottom.");
            text_println("Steep walls rise");
            text_println("on all sides.");
            break;
        case ROOM_CLIFF_MIDDLE:
            text_println("Rocky ledge on the");
            text_println("canyon wall.");
            break;
        case ROOM_CANYON_VIEW:
            text_println("Canyon view. Forest");
            text_println("to W, cliff path");
            text_println("going down.");
            break;
        case ROOM_MINE_ENTRANCE:
            text_println("Mine entrance.");
            text_println("Rickety wooden");
            text_println("supports here.");
            break;
        case ROOM_SQUEEKY_ROOM:
            text_println("A squeaky room.");
            text_println("Bats disturbed.");
            break;
        case ROOM_BAT_ROOM:
            text_println("A room full of");
            text_println("vampire bats.");
            text_println("Be careful!");
            break;
        case ROOM_SHAFT_ROOM:
            text_println("A shaft room. A");
            text_println("chain and basket");
            text_println("hang here.");
            break;
        case ROOM_SMELLY_ROOM:
            text_println("A smelly room.");
            text_println("Foul gases seep");
            text_println("from the walls.");
            break;
        case ROOM_GAS_ROOM:
            text_println("A gas-filled room.");
            text_println("Don't light any");
            text_println("flames here!");
            break;
        case ROOM_LADDER_TOP:
            text_println("Top of a ladder.");
            text_println("Mine below.");
            break;
        case ROOM_LADDER_BOTTOM:
            text_println("Bottom of a ladder.");
            text_println("Coal mine nearby.");
            break;
        case ROOM_TIMBER_ROOM:
            text_println("A room of broken");
            text_println("timber.");
            break;
        case ROOM_LOWER_SHAFT:
            text_println("A drafty room.");
            text_println("Basket hangs here");
            text_println("on a long chain.");
            break;
        case ROOM_MACHINE_ROOM:
            text_println("A machine room.");
            text_println("A large machine");
            text_println("with a lid.");
            break;
        case ROOM_MINE_1:
        case ROOM_MINE_2:
        case ROOM_MINE_3:
        case ROOM_MINE_4:
            text_println("Coal mine. Tunnels");
            text_println("lead in several");
            text_println("directions.");
            break;
        case ROOM_SLIDE_ROOM:
            text_println("A twisting metal");
            text_println("slide leads down.");
            break;
        default:
            text_println("You are somewhere.");
            break;
    }
}

/* ---- Object descriptions ---- */
static void engine_print_obj_desc(u8 obj) {
    switch(obj) {
        case OBJ_MAILBOX:
            text_println("A small mailbox.");
            break;
        case OBJ_ADVERTISEMENT:
            text_println("WELCOME TO ZORK!");
            text_println("Collect treasures");
            text_println("for the trophy case.");
            break;
        case OBJ_LAMP:
            text_println("A brass lantern,");
            text_println("battery-powered.");
            break;
        case OBJ_SWORD:
            text_println("An elvish sword of");
            text_println("great antiquity.");
            break;
        case OBJ_TROPHY_CASE:
            text_println("A large wooden");
            text_println("trophy case.");
            break;
        case OBJ_PAINTING:
            text_println("A painting of");
            text_println("unparalleled beauty.");
            break;
        case OBJ_SANDWICH_BAG:
            text_println("Brown sack, smells");
            text_println("of hot peppers.");
            break;
        case OBJ_GARLIC:
            text_println("A clove of garlic.");
            text_println("It smells terrible.");
            break;
        case OBJ_BOTTLE:
            text_println("A glass bottle.");
            break;
        case OBJ_WATER:
            text_println("A quantity of water.");
            break;
        case OBJ_KNIFE:
            text_println("A nasty-looking");
            text_println("knife.");
            break;
        case OBJ_CANARY:
            text_println("A golden clockwork");
            text_println("canary.");
            break;
        case OBJ_ROPE:
            text_println("A coil of rope.");
            break;
        case OBJ_COAL:
            text_println("A lump of coal.");
            break;
        case OBJ_TORCH:
            text_println("A burning torch.");
            break;
        case OBJ_BAG_OF_COINS:
            text_println("An old leather bag");
            text_println("bulging with coins.");
            break;
        case OBJ_BAR:
            text_println("A platinum bar.");
            break;
        case OBJ_POT_OF_GOLD:
            text_println("A pot of gold!");
            break;
        case OBJ_DIAMOND:
            text_println("An enormous diamond,");
            text_println("perfectly cut.");
            break;
        case OBJ_EMERALD:
            text_println("A large emerald.");
            break;
        case OBJ_BRACELET:
            text_println("A sapphire-encrusted");
            text_println("bracelet.");
            break;
        case OBJ_SCREWDRIVER:
            text_println("A screwdriver.");
            break;
        case OBJ_KITCHEN_WINDOW:
            text_println("A small window,");
            text_println("slightly open.");
            break;
        case OBJ_RUG:
            text_println("A large oriental");
            text_println("rug.");
            break;
        case OBJ_TRAP_DOOR:
            text_println("A trap door in");
            text_println("the floor.");
            break;
        case OBJ_TROLL:
            text_println("A nasty troll with");
            text_println("a bloody axe.");
            break;
        case OBJ_COFFIN:
            text_println("A solid gold coffin");
            text_println("of Ramses II.");
            break;
        case OBJ_SCEPTRE:
            text_println("An ornamented");
            text_println("sceptre, enameled.");
            break;
        case OBJ_SKULL:
            text_println("A crystal skull,");
            text_println("grinning nastily.");
            break;
        case OBJ_CHALICE:
            text_println("A silver chalice,");
            text_println("intricately engraved.");
            break;
        case OBJ_JADE:
            text_println("An exquisite jade");
            text_println("figurine.");
            break;
        case OBJ_TRIDENT:
            text_println("Poseidon's own");
            text_println("crystal trident.");
            break;
        case OBJ_SCARAB:
            text_println("A jeweled scarab.");
            break;
        case OBJ_TRUNK:
            text_println("A trunk of jewels.");
            break;
        case OBJ_EGG:
            text_println("A jewel-encrusted");
            text_println("egg.");
            break;
        case OBJ_BAUBLE:
            text_println("A beautiful brass");
            text_println("bauble.");
            break;
        case OBJ_BELL:
            text_println("A small brass bell.");
            break;
        case OBJ_BOOK:
            text_println("A large black book,");
            text_println("open to page 569.");
            break;
        case OBJ_CANDLES:
            text_println("A pair of candles.");
            break;
        case OBJ_MATCH:
            text_println("A matchbook. Cover");
            text_println("says FCD#3.");
            break;
        case OBJ_SHOVEL:
            text_println("A shovel.");
            break;
        case OBJ_KEYS:
            text_println("A set of keys.");
            break;
        case OBJ_PUMP:
            text_println("A hand-held");
            text_println("air pump.");
            break;
        case OBJ_WRENCH:
            text_println("A wrench.");
            break;
        case OBJ_GRATE:
            text_println("An iron grating.");
            break;
        case OBJ_BUOY:
            text_println("A navigational");
            text_println("buoy.");
            break;
        case OBJ_STILETTO:
            text_println("A stiletto.");
            break;
        default:
            text_println("Nothing special.");
            break;
    }
}

/* ---- Room description ---- */
void engine_describe_room(u8 rid, u8 force_long) {
    u8 i;
    u8 shown;
    u8 visited;

    if (!room_is_lit(rid)) {
        text_println("It is pitch black.");
        text_println("You are likely to be");
        text_println("eaten by a grue.");
        return;
    }

    visited = g_room_flags[rid] & ROOM_VISITED;

    /* Room name */
    text_println(g_room_name[rid]);

    if (force_long || !visited) {
        engine_print_room_desc(rid);
        g_room_flags[rid] |= ROOM_VISITED;
    }

    /* List objects in room - skip scenery, but include items in open NODESC containers */
    shown = 0;
    for (i = 0; i < NUM_OBJECTS; i++) {
        u8 iloc;
        u8 icont;
        iloc = g_obj_loc[i];
        /* Directly in room */
        if (iloc == rid) {
            if (g_obj_flags[i] & OBJ_NODESC) continue;
            if (g_obj_flags[i] == 0 && g_obj_score[i] == 0) continue;
            if (!shown) { text_println("You can see:"); shown = 1; }
            text_print("  ");
            text_println(g_obj_name[i]);
        }
        /* In an open NODESC container in room (e.g. kitchen table) */
        else if (iloc < NUM_OBJECTS && (g_obj_flags[iloc] & OBJ_CONTAINER)) {
            icont = iloc;
            if (!(g_obj_flags[icont] & OBJ_NODESC)) continue;
            if (!(g_obj_flags[icont] & OBJ_OPEN)) continue;
            if (g_obj_loc[icont] != rid) continue;
            if (!shown) { text_println("You can see:"); shown = 1; }
            text_print("  ");
            text_println(g_obj_name[i]);
        }
    }
}

/* ---- LOOK ---- */
void engine_do_look(void) {
    text_newline();
    engine_describe_room(g_player_room, 1);
}

/* ---- INVENTORY ---- */
void engine_do_inventory(void) {
    u8 i;
    u8 shown;
    shown = 0;
    text_newline();
    for (i = 0; i < NUM_OBJECTS; i++) {
        if (g_obj_loc[i] != LOC_PLAYER) continue;
        if (!shown) { text_println("You carry:"); shown = 1; }
        text_print("  ");
        text_println(g_obj_name[i]);
    }
    if (!shown) text_println("Empty-handed.");
}

/* ---- GO ---- */
static void do_go(u8 dir) {
    u8 dest;
    /* stride is now NUM_DIRS=10 */
    dest = g_room_exits[(u16)g_player_room * NUM_DIRS + dir];
    /* Troll blocks east exit from troll room */
    if (g_player_room == ROOM_TROLL_ROOM && dir == DIR_EAST && g_troll_alive) {
        text_println("The troll blocks your");
        text_println("way, waving his axe!");
        return;
    }
    /* Trap door: D from living room only works when trap door is open */
    if (g_player_room == ROOM_LIVING_ROOM && dir == DIR_DOWN) {
        if (g_obj_loc[OBJ_TRAP_DOOR] == ROOM_LIVING_ROOM
            && (g_obj_flags[OBJ_TRAP_DOOR] & OBJ_OPEN)) {
            dest = ROOM_CELLAR;
        } else {
            text_println("There is no way");
            text_println("down from here.");
            return;
        }
    }
    /* Cyclops blocks east exit from cyclops room */
    if (g_player_room == ROOM_CYCLOPS_ROOM && dir == DIR_EAST && g_cyclops_here) {
        text_println("The cyclops blocks");
        text_println("your way!");
        return;
    }
    if (dest == NO_EXIT) {
        text_println("You can't go that");
        text_println("way.");
        return;
    }
    g_player_room = dest;
    g_moves = g_moves + 1;
    engine_describe_room(dest, 0);
    /* Burn lamp */
    if (g_obj_flags[OBJ_LAMP] & OBJ_LIT) {
        if (g_lamp_fuel > 0) g_lamp_fuel--;
        if (g_lamp_fuel == 0) {
            g_obj_flags[OBJ_LAMP] &= ~OBJ_LIT;
            text_println("Your lamp dies.");
        } else if (g_lamp_fuel < 20) {
            text_println("The lamp flickers.");
        }
    }
}

/* ---- TAKE ---- */
static void do_take(u8 obj) {
    if (g_obj_loc[obj] == LOC_PLAYER) {
        text_println("Already carrying it.");
        return;
    }
    if (!(g_obj_flags[obj] & OBJ_TAKEABLE)) {
        text_println("You can't take that.");
        return;
    }
    if (inv_count() >= MAX_INVENTORY) {
        text_println("You're carrying too");
        text_println("much.");
        return;
    }
    g_obj_loc[obj] = LOC_PLAYER;
    text_print("Taken: ");
    text_println(g_obj_name[obj]);
    g_moves = g_moves + 1;
}

/* ---- DROP ---- */
static void do_drop(u8 obj) {
    u8 sv;
    if (!player_has(obj)) {
        text_println("Not carrying that.");
        return;
    }
    g_obj_loc[obj] = g_player_room;
    text_print("Dropped: ");
    text_println(g_obj_name[obj]);
    /* Score if treasure dropped in living room */
    sv = g_obj_score[obj];
    if (sv > 0 && g_player_room == ROOM_LIVING_ROOM
               && g_obj_target[obj] == OBJ_TROPHY_CASE) {
        g_obj_loc[obj] = OBJ_TROPHY_CASE;
        engine_add_score(sv);
        text_print("In the trophy case!");
        text_newline();
        {
            char sc[6];
            u16 s;
            s = sv;
            sc[0] = '+';
            sc[5] = 0;
            sc[4] = '0' + s % 10; s = s / 10;
            sc[3] = '0' + s % 10; s = s / 10;
            sc[2] = '0' + s % 10; s = s / 10;
            sc[1] = '0' + s % 10;
            text_println(sc);
        }
    }
    g_moves = g_moves + 1;
}

/* ---- EXAMINE ---- */
static void do_examine(u8 obj) {
    if (!obj_accessible(obj)) {
        text_println("Not here.");
        return;
    }
    engine_print_obj_desc(obj);
    if (obj == OBJ_MAILBOX) {
        if (g_obj_flags[OBJ_MAILBOX] & OBJ_OPEN)
            text_println("It is open.");
        else
            text_println("It is closed.");
    }
    if (obj == OBJ_LAMP) {
        if (g_obj_flags[OBJ_LAMP] & OBJ_LIT)
            text_println("It is on.");
        else
            text_println("It is off.");
    }
    if (obj == OBJ_TROPHY_CASE) {
        u8 i;
        u8 f;
        f = 0;
        for (i = 0; i < NUM_OBJECTS; i++) {
            if (OBJ_TROPHY_CASE < NUM_ROOMS) break;  /* safety */
            if (g_obj_loc[i] != OBJ_TROPHY_CASE) continue;
            if (!f) { text_println("Inside:"); f = 1; }
            text_print("  ");
            text_println(g_obj_name[i]);
        }
        if (!f) text_println("It is empty.");
    }
    g_moves = g_moves + 1;
}

/* ---- OPEN / CLOSE ---- */
static void do_open(u8 obj) {
    if (!obj_accessible(obj)) { text_println("Not here."); return; }
    if (g_obj_flags[obj] & OBJ_LOCKED) { text_println("It is locked."); return; }
    if (!(g_obj_flags[obj] & OBJ_CONTAINER)) { text_println("Can't open that."); return; }
    if (g_obj_flags[obj] & OBJ_OPEN) { text_println("Already open."); return; }
    g_obj_flags[obj] |= OBJ_OPEN;
    /* Trap door: enable D exit from living room to cellar */
    if (obj == OBJ_TRAP_DOOR) {
        /* Patch the exit table in RAM - Living Room D = Cellar (16) */
        /* g_room_exits is const ROM - can't patch directly */
        /* Instead handle in do_go: check trap door state */
    }
    {
        u8 i;
        u8 f;
        f = 0;
        text_println("Opened.");
        /* Only list contents if obj has OBJ_CONTAINER flag */
        if (g_obj_flags[obj] & OBJ_CONTAINER) {
            for (i = 0; i < NUM_OBJECTS; i++) {
                if (g_obj_loc[i] != obj) continue;
                if (!f) { text_println("Inside:"); f = 1; }
                text_print("  ");
                text_println(g_obj_name[i]);
            }
        }
        if (!f) text_println("(empty)");
    }
    g_moves = g_moves + 1;
}

static void do_close(u8 obj) {
    if (!obj_accessible(obj)) { text_println("Not here."); return; }
    if (!(g_obj_flags[obj] & OBJ_CONTAINER)) { text_println("Can't close that."); return; }
    if (!(g_obj_flags[obj] & OBJ_OPEN)) { text_println("Already closed."); return; }
    g_obj_flags[obj] &= ~OBJ_OPEN;
    text_println("Closed.");
    g_moves = g_moves + 1;
}

/* ---- READ ---- */
static void do_read(u8 obj) {
    if (!obj_accessible(obj)) { text_println("Not here."); return; }
    if (obj == OBJ_ADVERTISEMENT) {
        engine_print_obj_desc(obj);
    } else if (obj == OBJ_BOOK) {
        text_println("Commandment #12592:");
        text_println("Oh ye who go about");
        text_println("saying 'Hello sailor'");
        text_println("dost thou know thy");
        text_println("sin before the gods?");
    } else if (obj == OBJ_MATCH) {
        text_println("Visit Beautiful");
        text_println("FCD#3!");
    } else if (obj == OBJ_BOAT_LABEL) {
        text_println("PLEASE READ BEFORE");
        text_println("OPERATING. Magic word");
        text_println("is ZORK.");
    } else {
        text_println("Nothing to read.");
    }
    g_moves = g_moves + 1;
}

/* ---- TURN ON / OFF ---- */
static void do_turn_on(u8 obj) {
    if (!player_has(obj)) { text_println("Not carrying it."); return; }
    if (!(g_obj_flags[obj] & OBJ_LIGHT)) { text_println("Doesn't turn on."); return; }
    if (g_obj_flags[obj] & OBJ_LIT) { text_println("Already on."); return; }
    if (obj == OBJ_LAMP && g_lamp_fuel == 0) { text_println("Batteries dead."); return; }
    g_obj_flags[obj] |= OBJ_LIT;
    text_println("On.");
    g_moves = g_moves + 1;
}

static void do_turn_off(u8 obj) {
    if (!player_has(obj)) { text_println("Not carrying it."); return; }
    if (!(g_obj_flags[obj] & OBJ_LIT)) { text_println("Already off."); return; }
    g_obj_flags[obj] &= ~OBJ_LIT;
    text_println("Off.");
    g_moves = g_moves + 1;
}

/* ---- Main dispatch ---- */
void engine_execute(u8 verb, u8 obj) {
    char cmd_buf[22];
    u8 ci;
    u8 vi;
    u8 ni;

    if (verb == V_AGAIN) {
        verb = g_last_verb;
        obj  = g_last_noun;
    }

    /* Echo command to scroll buffer */
    cmd_buf[0] = '>';
    cmd_buf[1] = ' ';
    ci = 2;
    vi = 0;
    while (g_verb_names[verb][vi] && ci < 20) {
        cmd_buf[ci++] = g_verb_names[verb][vi++];
    }
    if (obj != 0xFF && obj < NUM_OBJECTS) {
        if (ci < 20) cmd_buf[ci++] = ' ';
        ni = 0;
        while (g_obj_name[obj][ni] && ci < 20) {
            cmd_buf[ci++] = g_obj_name[obj][ni++];
        }
    } else if (obj >= 0xF0 && obj <= (u8)(0xF0 + NUM_DIRS - 1)) {
        if (ci < 20) cmd_buf[ci++] = ' ';
        ni = 0;
        while (g_dir_names[obj - 0xF0][ni] && ci < 20) {
            cmd_buf[ci++] = g_dir_names[obj - 0xF0][ni++];
        }
    }
    cmd_buf[ci] = 0;
    text_newline();
    text_println(cmd_buf);
    wizard_log(cmd_buf);

    switch (verb) {
        case V_LOOK:      engine_do_look();      break;
        case V_INVENTORY: engine_do_inventory(); break;
        case V_WAIT:
            text_println("Time passes.");
            g_moves = g_moves + 1;
            break;
        case V_NORTH: do_go(DIR_NORTH); break;
        case V_SOUTH: do_go(DIR_SOUTH); break;
        case V_EAST:  do_go(DIR_EAST);  break;
        case V_WEST:  do_go(DIR_WEST);  break;
        case V_UP:    do_go(DIR_UP);    break;
        case V_DOWN:  do_go(DIR_DOWN);  break;
        case V_TAKE:    do_take(obj);    break;
        case V_GET:     do_take(obj);    break;
        case V_DROP:    do_drop(obj);    break;
        case V_EXAMINE: do_examine(obj); break;
        case V_OPEN:    do_open(obj);    break;
        case V_CLOSE:   do_close(obj);   break;
        case V_READ:    do_read(obj);    break;
        case V_TURN_ON:  do_turn_on(obj); break;
        case V_TURN_OFF: do_turn_off(obj);break;
        case V_ATTACK:
            if (obj == OBJ_TROLL) {
                if (!player_has(OBJ_SWORD) && !player_has(OBJ_KNIFE)
                    && !player_has(OBJ_SCREWDRIVER)) {
                    text_println("You need a weapon!");
                } else if (g_troll_alive) {
                    u8 hit;
                    hit = (u8)(g_moves & 3);
                    if (hit < 2) {
                        text_println("You slash the troll.");
                        text_println("He is wounded!");
                    } else if (hit == 2) {
                        text_println("The troll is dead!");
                        g_troll_alive = 0;
                        g_obj_loc[OBJ_TROLL] = LOC_GONE;
                        engine_add_score(25);
                        text_println("Score +25");
                    } else {
                        text_println("The troll parries!");
                    }
                } else {
                    text_println("The troll is dead.");
                }
            } else {
                text_println("Violence isn't the");
                text_println("answer here.");
            }
            g_moves = g_moves + 1;
            break;
        case V_PUT:
            do_drop(obj);
            break;
        case V_GO:
            if (obj >= DIR_NOUN_BASE) {
                do_go(obj - DIR_NOUN_BASE);
            } else {
                text_println("Go which way?");
            }
            break;
        case V_SAVE:
            zork_save();
            break;
        case V_RESTORE:
            zork_load();
            engine_describe_room(g_player_room, 1);
            break;
        case V_ENTER:
            if (g_player_room == ROOM_EAST_OF_HOUSE) {
                if (g_obj_flags[OBJ_KITCHEN_WINDOW] & OBJ_OPEN) {
                    g_player_room = ROOM_KITCHEN;
                    g_moves = g_moves + 1;
                    engine_describe_room(ROOM_KITCHEN, 0);
                } else {
                    text_println("The window is");
                    text_println("closed.");
                }
            } else {
                text_println("There's nothing to");
                text_println("enter here.");
            }
            break;
        case V_CLIMB:
            if (g_player_room == ROOM_PATH) {
                g_player_room = ROOM_UP_A_TREE;
                g_moves = g_moves + 1;
                engine_describe_room(ROOM_UP_A_TREE, 0);
            } else {
                text_println("You can't climb");
                text_println("that.");
            }
            break;
        case V_MOVE:
            if (obj == OBJ_RUG && g_player_room == ROOM_LIVING_ROOM) {
                g_obj_loc[OBJ_RUG] = LOC_GONE;
                g_obj_loc[OBJ_TRAP_DOOR] = ROOM_LIVING_ROOM;
                g_obj_flags[OBJ_TRAP_DOOR] = OBJ_CONTAINER;
                text_println("You move the rug,");
                text_println("revealing a trap");
                text_println("door in the floor!");
            } else {
                text_println("You can't move");
                text_println("that.");
            }
            g_moves = g_moves + 1;
            break;
        case V_DIG:
            if (obj == OBJ_SAND && g_player_room == ROOM_SANDY_BEACH
                && player_has(OBJ_SHOVEL)) {
                text_println("You dig in the sand.");
                text_println("You find a scarab!");
                g_obj_loc[OBJ_SCARAB] = ROOM_SANDY_BEACH;
            } else if (!player_has(OBJ_SHOVEL)) {
                text_println("You need a shovel.");
            } else {
                text_println("Digging here reveals");
                text_println("nothing useful.");
            }
            g_moves = g_moves + 1;
            break;
        case V_TIE:
            if (obj == OBJ_ROPE && g_player_room == ROOM_CHASM_ROOM
                && player_has(OBJ_ROPE)) {
                g_obj_loc[OBJ_ROPE] = ROOM_CHASM_ROOM;
                text_println("Rope tied to the");
                text_println("railing. You can");
                text_println("climb down now.");
            } else {
                text_println("You can't tie that");
                text_println("there.");
            }
            g_moves = g_moves + 1;
            break;
        case V_WAVE:
            if (obj == OBJ_SCEPTRE && g_player_room == ROOM_END_OF_RAINBOW) {
                text_println("You wave the");
                text_println("sceptre. A rainbow");
                text_println("bridge appears!");
                g_room_flags[ROOM_ON_RAINBOW] |= ROOM_ABOVE;
            } else {
                text_println("Nothing happens.");
            }
            g_moves = g_moves + 1;
            break;
        case V_RING:
            if (obj == OBJ_BELL && player_has(OBJ_BELL)) {
                text_println("You ring the bell.");
                text_println("The spirits wail");
                text_println("and the candles");
                text_println("go out!");
                g_obj_flags[OBJ_CANDLES] &= ~OBJ_LIT;
            } else {
                text_println("Nothing to ring.");
            }
            g_moves = g_moves + 1;
            break;
        case V_LIGHT:
            if (obj == OBJ_MATCH && player_has(OBJ_MATCH)) {
                text_println("You light a match.");
            } else if (obj == OBJ_CANDLES && player_has(OBJ_CANDLES)) {
                text_println("You light the");
                text_println("candles.");
                g_obj_flags[OBJ_CANDLES] |= OBJ_LIT | OBJ_LIGHT;
            } else {
                text_println("You can't light");
                text_println("that.");
            }
            g_moves = g_moves + 1;
            break;
        case V_PRAY:
            if (g_player_room == ROOM_SOUTH_TEMPLE) {
                text_println("You pray at the");
                text_println("altar. The spirits");
                text_println("are appeased.");
                g_obj_loc[OBJ_GHOSTS] = LOC_GONE;
            } else {
                text_println("Nothing happens.");
            }
            g_moves = g_moves + 1;
            break;
        case V_ECHO:
            if (g_player_room == ROOM_LOUD_ROOM) {
                text_println("ECHO!");
                text_println("The bar falls from");
                text_println("the ledge!");
                g_obj_loc[OBJ_BAR] = ROOM_LOUD_ROOM;
            } else {
                text_println("Your voice echoes.");
            }
            g_moves = g_moves + 1;
            break;
        case V_CROSS:
            if (g_player_room == ROOM_ARAGAIN_FALLS) {
                if (g_room_flags[ROOM_ON_RAINBOW] & ROOM_ABOVE) {
                    g_player_room = ROOM_ON_RAINBOW;
                    g_moves = g_moves + 1;
                    engine_describe_room(ROOM_ON_RAINBOW, 0);
                } else {
                    text_println("The rainbow isn't");
                    text_println("solid enough.");
                }
            } else {
                text_println("Nothing to cross.");
            }
            break;
        case V_INFLATE:
            if (obj == OBJ_INFLATABLE_BOAT && player_has(OBJ_PUMP)
                && player_has(OBJ_INFLATABLE_BOAT)) {
                text_println("You inflate the");
                text_println("boat. It's ready!");
                g_obj_loc[OBJ_INFLATED_BOAT] = LOC_PLAYER;
                g_obj_loc[OBJ_INFLATABLE_BOAT] = LOC_GONE;
            } else {
                text_println("You can't inflate");
                text_println("that.");
            }
            g_moves = g_moves + 1;
            break;
        case V_LOWER:
            if (obj == OBJ_RAISED_BASKET
                && g_player_room == ROOM_SHAFT_ROOM) {
                text_println("You lower the");
                text_println("basket.");
                g_obj_loc[OBJ_RAISED_BASKET] = LOC_GONE;
                g_obj_loc[OBJ_LOWERED_BASKET] = ROOM_LOWER_SHAFT;
            } else {
                text_println("Nothing to lower.");
            }
            g_moves = g_moves + 1;
            break;
        case V_RAISE:
            if (obj == OBJ_LOWERED_BASKET
                && g_player_room == ROOM_LOWER_SHAFT) {
                text_println("You raise the");
                text_println("basket.");
                g_obj_loc[OBJ_LOWERED_BASKET] = LOC_GONE;
                g_obj_loc[OBJ_RAISED_BASKET] = ROOM_SHAFT_ROOM;
            } else {
                text_println("Nothing to raise.");
            }
            g_moves = g_moves + 1;
            break;
        case V_QUIT:
            text_println("Thanks for playing!");
            text_print("Final score: ");
            {
                char sc[8];
                u16 s;
                u8 p;
                s = g_score;
                sc[7] = 0;
                sc[6] = '0' + s % 10; s = s / 10;
                sc[5] = '0' + s % 10; s = s / 10;
                sc[4] = '0' + s % 10; s = s / 10;
                sc[3] = '0' + s % 10;
                p = 3;
                while (p < 6 && sc[p] == '0') p++;
                text_println(sc + p);
            }
            break;
        default:
            text_println("I can't do that.");
            break;
    }
}

static char s_empty[1] = {0};
void engine_refresh_screen(void) {
    text_redraw();
    text_draw_divider();
    text_draw_selector();
    text_print_cmd(g_verb_names[g_verb_idx],
                   (g_input_mode == MODE_NOUN && g_noun_count > 0)
                   ? ((g_noun_list[g_noun_idx] >= 0xF0)
                      ? (char*)g_dir_names[g_noun_list[g_noun_idx] - 0xF0]
                      : (char*)g_obj_name[g_noun_list[g_noun_idx]])
                   : s_empty);
    if (g_wizard_mode) wizard_draw_status();
    else text_draw_status();
}

void engine_init(void) {
    u8 i;
    g_player_room  = ROOM_WEST_OF_HOUSE;
    g_troll_alive  = 1;
    g_thief_alive  = 1;
    g_cyclops_here = 1;
    g_score        = 0;
    g_moves        = 0;
    g_lamp_fuel    = 200;
    g_dead         = 0;
    for (i = 0; i < NUM_OBJECTS; i++) {
        g_obj_loc[i]   = g_obj_default_loc[i];
        g_obj_flags[i] = g_obj_default_flags[i];
    }
    for (i = 0; i < NUM_ROOMS; i++) {
        g_room_flags[i] = g_room_base_flags[i];
    }
    /* Kitchen and Living Room lit (windows) */
    g_room_flags[ROOM_KITCHEN]      |= ROOM_ABOVE;
    g_room_flags[ROOM_LIVING_ROOM]  |= ROOM_ABOVE;
    /* Torch room always lit */
    g_room_flags[ROOM_TORCH_ROOM]   |= ROOM_ABOVE;
    /* Trap door starts hidden */
    g_obj_loc[OBJ_TRAP_DOOR] = LOC_GONE;
}
