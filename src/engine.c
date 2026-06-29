/* engine.c - Zork Neo: Core game logic
 *
 * Verb handlers, room description, object management.
 * Darkness, lamp fuel, score tracking.
 *
 * cc900 C89: no declarations after statements, no signed mul/div,
 * no volatile u16* stores, strict C89 block scope.
 */
#include "engine.h"
#include "text.h"
#include "zork_save.h"

/* ---- Mutable game state (in work RAM) ---- */
u8  g_player_room;
u16 g_score;
u16 g_moves;
u8  g_lamp_fuel;
u8  g_dead;
u8  g_obj_loc[NUM_OBJECTS];
u8  g_obj_flags[NUM_OBJECTS];
u8  g_room_flags[NUM_ROOMS];

/* ---- Helpers ---- */

static u8 room_is_lit(u8 room_id) {
    u8 i;
    /* Outdoors or above-ground rooms are always lit */
    if (g_rooms[room_id].flags & ROOM_ABOVE) return 1;
    /* Check for lit lantern in room or player inventory */
    for (i = 0; i < NUM_OBJECTS; i++) {
        if (!(g_obj_flags[i] & OBJ_LIGHT)) continue;
        if (!(g_obj_flags[i] & OBJ_LIT))   continue;
        if (g_obj_loc[i] == room_id)        return 1;
        if (g_obj_loc[i] == LOC_PLAYER)     return 1;
    }
    return 0;
}

static u8 player_has(u8 obj_id) {
    return g_obj_loc[obj_id] == LOC_PLAYER;
}

static u8 obj_in_room(u8 obj_id, u8 room_id) {
    u8 loc;
    loc = g_obj_loc[obj_id];
    if (loc == room_id) return 1;
    /* Inside open container in room? */
    if (loc < NUM_OBJECTS) {
        if ((g_obj_flags[loc] & (OBJ_CONTAINER | OBJ_OPEN))
                == (OBJ_CONTAINER | OBJ_OPEN)) {
            if (g_obj_loc[loc] == room_id) return 1;
            if (g_obj_loc[loc] == LOC_PLAYER) return 1;
        }
    }
    return 0;
}

static u8 inventory_count(void) {
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

/* ---- Room description ---- */

void engine_describe_room(u8 room_id, u8 force_long) {
    const Room *r;
    u8 i;
    u8 shown;

    r = &g_rooms[room_id];

    if (!room_is_lit(room_id)) {
        text_println("It is pitch black. You are");
        text_println("likely to be eaten by a grue.");
        return;
    }

    /* Room name in title palette */
    text_put_row_centered(0, PAL_TITLE, r->short_name);
    text_newline();

    /* Long or short description */
    if (force_long || !(g_room_flags[room_id] & ROOM_VISITED)) {
        text_println(r->description);
        g_room_flags[room_id] |= ROOM_VISITED;
    }

    /* List objects present */
    shown = 0;
    for (i = 0; i < NUM_OBJECTS; i++) {
        u8 loc;
        loc = g_obj_loc[i];
        if (loc != room_id) continue;
        if (!shown) {
            text_println("You can see here:");
            shown = 1;
        }
        text_print("  ");
        text_println(g_objects[i].name);
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
        if (!shown) {
            text_println("You are carrying:");
            shown = 1;
        }
        text_print("  ");
        text_println(g_objects[i].name);
    }
    if (!shown) text_println("You are empty-handed.");
}

/* ---- GO ---- */
static void do_go(u8 dir) {
    u8 dest;
    dest = g_rooms[g_player_room].exits[dir];
    if (dest == NO_EXIT) {
        text_println("You can't go that way.");
        return;
    }
    g_player_room = dest;
    g_moves = g_moves + 1;

    /* Darkness check on arrival */
    if (!room_is_lit(dest) && (g_rooms[dest].flags & ROOM_DARK)) {
        /* Warn if unvisited dark room */
        if (!(g_room_flags[dest] & ROOM_VISITED)) {
            text_println("Walking into darkness...");
        }
    }
    engine_describe_room(dest, 0);
    /* Burn lamp */
    if (g_obj_flags[OBJ_LANTERN] & OBJ_LIT) {
        if (g_lamp_fuel > 0) {
            g_lamp_fuel--;
        }
        if (g_lamp_fuel == 0) {
            g_obj_flags[OBJ_LANTERN] &= ~OBJ_LIT;
            text_println("Your lamp has run out of");
            text_println("power.");
        } else if (g_lamp_fuel < 20) {
            text_println("Your lamp is getting dim.");
        }
    }
}

/* ---- TAKE ---- */
static void do_take(u8 obj) {
    if (g_obj_loc[obj] == LOC_PLAYER) {
        text_println("You already have that.");
        return;
    }
    if (!(g_obj_flags[obj] & OBJ_TAKEABLE)) {
        text_println("You can't take that.");
        return;
    }
    if (inventory_count() >= MAX_INVENTORY) {
        text_println("You are carrying too much.");
        return;
    }
    g_obj_loc[obj] = LOC_PLAYER;
    text_print("Taken. (");
    text_print(g_objects[obj].name);
    text_println(")");
    g_moves = g_moves + 1;
}

/* ---- DROP ---- */
static void do_drop(u8 obj) {
    if (!player_has(obj)) {
        text_println("You don't have that.");
        return;
    }
    g_obj_loc[obj] = g_player_room;
    text_print("Dropped. (");
    text_print(g_objects[obj].name);
    text_println(")");

    /* Depositing treasure in trophy case scores points */
    if (g_player_room == ROOM_LIVING_ROOM && obj != OBJ_TROPHY_CASE) {
        if (g_objects[obj].score_value > 0 &&
            g_objects[obj].container_id == OBJ_TROPHY_CASE) {
            /* Actually put it in the case */
            g_obj_loc[obj] = OBJ_TROPHY_CASE;
            engine_add_score(g_objects[obj].score_value);
            text_print("The ");
            text_print(g_objects[obj].name);
            text_println(" goes in the trophy case.");
            text_print("Score +");
            {
                char sc[6];
                u8 p;
                u16 sv;
                sv = g_objects[obj].score_value;
                p = 4;
                sc[5] = 0;
                sc[4] = '0' + sv % 10; sv = sv / 10;
                sc[3] = '0' + sv % 10; sv = sv / 10;
                sc[2] = '0' + sv % 10; sv = sv / 10;
                sc[1] = '0' + sv % 10;
                sc[0] = '0' + sv % 10;
                /* trim leading zeros */
                p = 0;
                while (p < 4 && sc[p] == '0') p++;
                text_println(sc + p);
            }
        }
    }
    g_moves = g_moves + 1;
}

/* ---- EXAMINE ---- */
static void do_examine(u8 obj) {
    if (!obj_in_room(obj, g_player_room) && !player_has(obj)) {
        text_println("You don't see that here.");
        return;
    }
    text_println(g_objects[obj].description);
    /* Extra detail for specific objects */
    if (obj == OBJ_MAILBOX) {
        if (g_obj_flags[OBJ_MAILBOX] & OBJ_OPEN) text_println("It is open.");
        else text_println("It is closed.");
    }
    if (obj == OBJ_LANTERN) {
        if (g_obj_flags[OBJ_LANTERN] & OBJ_LIT) text_println("on.");
        else text_println("off.");
    }
    if (obj == OBJ_TROPHY_CASE) {
        u8 i;
        u8 found;
        if (g_obj_flags[OBJ_TROPHY_CASE] & OBJ_OPEN) text_println("open.");
        else text_println("closed.");
        found = 0;
        for (i = 0; i < NUM_OBJECTS; i++) {
            if (g_obj_loc[i] == OBJ_TROPHY_CASE) {
                if (!found) { text_println("Inside:"); found = 1; }
                text_print("  ");
                text_println(g_objects[i].name);
            }
        }
        if (!found) text_println("It is empty.");
    }
    g_moves = g_moves + 1;
}

/* ---- OPEN / CLOSE ---- */
static void do_open(u8 obj) {
    if (!obj_in_room(obj, g_player_room) && !player_has(obj)) {
        text_println("You don't see that here.");
        return;
    }
    if (g_obj_flags[obj] & OBJ_LOCKED) {
        text_println("It is locked.");
        return;
    }
    if (!(g_obj_flags[obj] & OBJ_CONTAINER)) {
        text_println("You can't open that.");
        return;
    }
    if (g_obj_flags[obj] & OBJ_OPEN) {
        text_println("It is already open.");
        return;
    }
    g_obj_flags[obj] |= OBJ_OPEN;
    text_print("Opened. ");
    /* Reveal contents */
    {
        u8 i;
        u8 found;
        found = 0;
        for (i = 0; i < NUM_OBJECTS; i++) {
            if (g_obj_loc[i] == obj) {
                if (!found) {
                    text_println("Inside you find:");
                    found = 1;
                }
                text_print("  ");
                text_println(g_objects[i].name);
            }
        }
        if (!found) text_println("(empty)");
    }
    g_moves = g_moves + 1;
}

static void do_close(u8 obj) {
    if (!obj_in_room(obj, g_player_room) && !player_has(obj)) {
        text_println("You don't see that here.");
        return;
    }
    if (!(g_obj_flags[obj] & OBJ_CONTAINER)) {
        text_println("You can't close that.");
        return;
    }
    if (!(g_obj_flags[obj] & OBJ_OPEN)) {
        text_println("It is already closed.");
        return;
    }
    g_obj_flags[obj] &= ~OBJ_OPEN;
    text_println("Closed.");
    g_moves = g_moves + 1;
}

/* ---- READ ---- */
static void do_read(u8 obj) {
    if (!obj_in_room(obj, g_player_room) && !player_has(obj)) {
        text_println("You don't see that here.");
        return;
    }
    if (obj == OBJ_LEAFLET) {
        text_println(g_objects[OBJ_LEAFLET].description);
    } else if (obj == OBJ_MAT) {
        text_println("\"WELCOME TO ZORK\"");
    } else if (obj == OBJ_SWORD) {
        text_println("The blade is inscribed with");
        text_println("runes of elvish craft.");
    } else {
        text_println("There's nothing to read.");
    }
    g_moves = g_moves + 1;
}

/* ---- TURN ON / TURN OFF ---- */
static void do_turn_on(u8 obj) {
    if (!player_has(obj)) {
        text_println("You need to be holding it.");
        return;
    }
    if (!(g_obj_flags[obj] & OBJ_LIGHT)) {
        text_println("That doesn't turn on.");
        return;
    }
    if (g_obj_flags[obj] & OBJ_LIT) {
        text_println("It is already on.");
        return;
    }
    if (obj == OBJ_LANTERN && g_lamp_fuel == 0) {
        text_println("The batteries are dead.");
        return;
    }
    g_obj_flags[obj] |= OBJ_LIT;
    text_println("On.");
    g_moves = g_moves + 1;
}

static void do_turn_off(u8 obj) {
    if (!player_has(obj)) {
        text_println("You need to be holding it.");
        return;
    }
    if (!(g_obj_flags[obj] & OBJ_LIT)) {
        text_println("It is already off.");
        return;
    }
    g_obj_flags[obj] &= ~OBJ_LIT;
    text_println("Off.");
    g_moves = g_moves + 1;
}

/* ---- ATTACK ---- */
static void do_attack(u8 obj) {
    text_println("Violence isn't the answer");
    text_println("to this one.");
    g_moves = g_moves + 1;
}

/* ---- WAIT ---- */
static void do_wait(void) {
    text_println("Time passes.");
    g_moves = g_moves + 1;
    /* Burn lamp */
    if (g_obj_flags[OBJ_LANTERN] & OBJ_LIT) {
        if (g_lamp_fuel > 0) g_lamp_fuel--;
    }
}

/* ---- Main dispatch ---- */
void engine_execute(u8 verb, u8 obj) {
    if (verb == V_AGAIN) {
        verb = g_last_verb;
        obj  = g_last_noun;
    }

    switch (verb) {
        case V_LOOK:      engine_do_look();    break;
        case V_INVENTORY: engine_do_inventory(); break;
        case V_WAIT:      do_wait();            break;
        case V_NORTH:     do_go(DIR_NORTH);     break;
        case V_SOUTH:     do_go(DIR_SOUTH);     break;
        case V_EAST:      do_go(DIR_EAST);      break;
        case V_WEST:      do_go(DIR_WEST);      break;
        case V_UP:        do_go(DIR_UP);        break;
        case V_DOWN:      do_go(DIR_DOWN);      break;
        case V_GO:
            /* GO with no direction: show exits */
            text_println("Go which way?");
            break;
        case V_TAKE:      do_take(obj);         break;
        case V_DROP:      do_drop(obj);         break;
        case V_EXAMINE:   do_examine(obj);      break;
        case V_OPEN:      do_open(obj);         break;
        case V_CLOSE:     do_close(obj);        break;
        case V_READ:      do_read(obj);         break;
        case V_TURN_ON:   do_turn_on(obj);      break;
        case V_TURN_OFF:  do_turn_off(obj);     break;
        case V_ATTACK:    do_attack(obj);       break;
        case V_PUT:
            /* PUT obj IN container: simplified - drop in trophy case */
            do_drop(obj);
            break;
        case V_SAVE:
            zork_save();
            break;
        case V_RESTORE:
            zork_load();
            engine_describe_room(g_player_room, 1);
            break;
        case V_QUIT:
            text_println("Your score is:");
            {
                char sc[8];
                u16 sv;
                u8 p;
                sv = g_score;
                sc[7] = 0;
                sc[6] = '0' + sv % 10; sv = sv / 10;
                sc[5] = '0' + sv % 10; sv = sv / 10;
                sc[4] = '0' + sv % 10; sv = sv / 10;
                sc[3] = '0' + sv % 10;
                sc[2] = sc[3]; sc[1] = sc[4]; sc[0] = '0';
                p = 3;
                while (p < 6 && sc[p] == '0') p++;
                text_println(sc + p);
            }
            text_println("Thanks for playing Zork Neo!");
            break;
        default:
            text_println("I don't know how to do that.");
            break;
    }
}

/* ---- Refresh all UI rows ---- */
void engine_refresh_screen(void) {
    text_redraw();
    text_draw_divider();
    text_draw_selector();
    text_print_cmd(g_verb_names[g_verb_idx],
                   (g_input_mode == MODE_NOUN && g_noun_count > 0)
                   ? g_objects[g_noun_list[g_noun_idx]].name
                   : "");
    text_draw_status();
}

/* ---- Init ---- */
void engine_init(void) {
    u8 i;

    g_player_room = ROOM_WEST_OF_HOUSE;
    g_score       = 0;
    g_moves       = 0;
    g_lamp_fuel   = 200;    /* ~200 moves of light */
    g_dead        = 0;

    /* Copy default locations and flags from ROM data */
    for (i = 0; i < NUM_OBJECTS; i++) {
        g_obj_loc[i]   = g_objects[i].location;
        g_obj_flags[i] = g_objects[i].flags;
    }
    for (i = 0; i < NUM_ROOMS; i++) {
        g_room_flags[i] = g_rooms[i].flags & ~ROOM_VISITED;
    }
    /* Start room counts as visited after first look */
}
