/* thief.c - Zork Neo: The Thief NPC
 *
 * Implements the Thief from Zork I per the ZIL source (1actions.zil):
 *  - Wanders underground rooms each turn (RLANDBIT = outdoor, avoids those)
 *  - Steals items from rooms and player inventory
 *  - Deposits treasures in TREASURE-ROOM when alone there
 *  - Multi-round combat with stiletto
 *  - Banished only by killing him
 *
 * Called once per move from engine_post_move().
 */
#include "zork_types.h"
#include "text.h"
#include "engine.h"

/* Thief's current room (separate from g_obj_loc for NPC tracking) */
u8 g_thief_room;
u8 g_thief_visible;
u8 g_thief_wounded;  /* 0=healthy, 1=wounded, 2=badly wounded */

/* Underground rooms the thief wanders (not outdoor, not sacred maze) */
/* Based on ZIL: moves to rooms with RLANDBIT clear (underground) */
static const u8 THIEF_ROOMS[] = {
    ROOM_TROLL_ROOM,
    ROOM_EW_PASSAGE,
    ROOM_ROUND_ROOM,
    ROOM_NS_PASSAGE,
    ROOM_CHASM_ROOM,
    ROOM_EAST_OF_CHASM,
    ROOM_GALLERY,
    ROOM_STUDIO,
    ROOM_LOUD_ROOM,
    ROOM_ENGRAVINGS_CAVE,
    ROOM_NARROW_PASSAGE,
    ROOM_TREASURE_ROOM,
    ROOM_CYCLOPS_ROOM,
    ROOM_STRANGE_PASSAGE,
    255  /* terminator */
};
#define NUM_THIEF_ROOMS 14

void thief_init(void) {
    g_thief_room    = ROOM_TREASURE_ROOM;
    g_thief_visible = 0;
    g_thief_wounded = 0;
    /* Give thief his stiletto */
    g_obj_loc[OBJ_STILETTO] = (u8)(NUM_ROOMS + OBJ_THIEF);
    g_obj_loc[OBJ_LARGE_BAG] = (u8)(NUM_ROOMS + OBJ_THIEF);
}

/* Move thief to a random underground room */
static void thief_move(void) {
    u8 next;
    /* Simple pseudo-random using move counter */
    next = (u8)(g_moves % NUM_THIEF_ROOMS);
    g_thief_room = THIEF_ROOMS[next];
    g_thief_visible = 0;
}

/* Steal one item from a room */
static void thief_steal_from_room(u8 room) {
    u8 i;
    u8 candidates[8];
    u8 count;
    u8 pick;

    count = 0;
    for (i = 0; i < NUM_OBJECTS && count < 8; i++) {
        if (g_obj_loc[i] != room) continue;
        if (!(g_obj_flags[i] & OBJ_TAKEABLE)) continue;
        if (g_obj_flags[i] & OBJ_NODESC) continue;
        if (i == OBJ_STILETTO || i == OBJ_LARGE_BAG) continue;
        candidates[count++] = i;
    }
    if (count == 0) return;

    pick = candidates[g_moves % count];
    g_obj_loc[pick] = (u8)(NUM_ROOMS + OBJ_THIEF);

    if (room == g_player_room) {
        text_println("A shadowy figure darts");
        text_print("away with your ");
        text_println(g_obj_name[pick]);
    }
}

/* Steal from player inventory */
static void thief_steal_from_player(void) {
    u8 i;
    u8 candidates[8];
    u8 count;
    u8 pick;

    count = 0;
    for (i = 0; i < NUM_OBJECTS && count < 8; i++) {
        if (g_obj_loc[i] != LOC_PLAYER) continue;
        if (!(g_obj_flags[i] & OBJ_TAKEABLE)) continue;
        if (i == OBJ_LAMP || i == OBJ_SWORD || i == OBJ_KNIFE) continue;
        candidates[count++] = i;
    }
    if (count == 0) return;

    pick = candidates[g_moves % count];
    g_obj_loc[pick] = (u8)(NUM_ROOMS + OBJ_THIEF);
    text_println("You feel a light touch");
    text_print("and notice the thief");
    text_newline();
    text_print("has taken your ");
    text_println(g_obj_name[pick]);
}

/* Deposit stolen treasures in treasure room */
static void thief_deposit(void) {
    u8 i;
    for (i = 0; i < NUM_OBJECTS; i++) {
        if (g_obj_loc[i] != (u8)(NUM_ROOMS + OBJ_THIEF)) continue;
        if (i == OBJ_STILETTO || i == OBJ_LARGE_BAG) continue;
        if (g_obj_score[i] > 0) {
            g_obj_loc[i] = ROOM_TREASURE_ROOM;
        }
    }
}

/* Called every move */
void thief_tick(void) {
    if (!g_thief_alive) return;

    /* If in treasure room alone, deposit booty */
    if (g_thief_room == ROOM_TREASURE_ROOM
        && g_player_room != ROOM_TREASURE_ROOM) {
        thief_deposit();
        thief_move();
        return;
    }

    /* If in same room as player */
    if (g_thief_room == g_player_room) {
        /* Don't act in lit rooms (ONBIT in ZIL) */
        if (room_is_lit_pub(g_player_room)) {
            /* Thief visible but just lurks in lit rooms */
            if (!g_thief_visible) {
                g_thief_visible = 1;
                text_println("A seedy-looking man");
                text_println("leans against the");
                text_println("wall, watching you.");
            }
            return;
        }
        /* Dark room - steal from player (30% chance per move) */
        if ((g_moves % 10) < 3) {
            thief_steal_from_player();
        }
        /* Steal from room too */
        if ((g_moves % 7) == 0) {
            thief_steal_from_room(g_player_room);
        }
        return;
    }

    /* Steal from rooms the thief passes through */
    if ((g_moves % 5) == 0) {
        thief_steal_from_room(g_thief_room);
    }

    /* Move to next room */
    thief_move();
}

/* Combat: ATTACK thief */
void thief_attack(u8 weapon_obj) {
    u8 my_roll;
    u8 his_roll;
    u8 thief_still_up;

    if (!g_thief_alive) {
        text_println("The thief is dead.");
        return;
    }
    if (g_thief_room != g_player_room) {
        text_println("The thief isn't here.");
        return;
    }

    my_roll  = (u8)(g_moves & 7);
    his_roll = (u8)((g_moves >> 1) & 7);
    thief_still_up = 1;

    /* ---- Your swing resolves first ---- */
    if (g_thief_wounded == 0) {
        if (my_roll < 3) {
            text_println("You slash the thief!");
            text_println("He reels, wounded.");
            g_thief_wounded = 1;
        } else if (my_roll == 3) {
            text_println("The thief parries");
            text_println("your blow deftly.");
        } else {
            text_println("Your blow goes wide.");
        }
    } else if (g_thief_wounded == 1) {
        if (my_roll < 4) {
            text_println("Another hit! The");
            text_println("thief is badly hurt.");
            g_thief_wounded = 2;
        } else {
            text_println("He staggers back but");
            text_println("holds his ground.");
        }
    } else {
        /* Badly wounded - easier to finish off */
        if (my_roll < 5) {
            text_println("A final blow - the");
            text_println("thief falls dead!");
            g_thief_alive = 0;
            thief_still_up = 0;
            g_thief_room = g_player_room;
            /* Drop everything he's carrying */
            {
                u8 i;
                u8 encoded;
                encoded = (u8)(NUM_ROOMS + OBJ_THIEF);
                for (i = 0; i < NUM_OBJECTS; i++) {
                    if (g_obj_loc[i] == encoded) {
                        g_obj_loc[i] = g_player_room;
                        g_obj_flags[i] &= ~OBJ_NODESC;
                    }
                }
            }
            engine_add_score(10);
            text_println("Score +10");
        } else {
            text_println("He's barely standing");
            text_println("but shrugs off the");
            text_println("hit and fights on.");
        }
    }

    /* ---- His counter, if he's still alive to give it ---- */
    if (thief_still_up && g_thief_wounded < 2) {
        if (his_roll < 2) {
            text_println("His stiletto flashes");
            text_println("and grazes your arm.");
        } else if (his_roll == 2) {
            text_println("He stabs at you -");
            text_println("a close call!");
        }
    }

    g_moves = g_moves + 1;
}
