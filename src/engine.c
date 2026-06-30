/* engine.c - Zork Neo: Game logic
 * Uses flat arrays only - no struct member access
 */
#include "zork_types.h"
#include "text.h"
#include "zork_save.h"

/* Mutable state */
u8  g_player_room;
u16 g_score;
u16 g_moves;
u8  g_lamp_fuel;
u8  g_dead;
u8  g_obj_loc[NUM_OBJECTS];
u8  g_obj_flags[NUM_OBJECTS];
u8  g_room_flags[NUM_ROOMS];

/* ---- Helpers ---- */

static u8 room_is_lit(u8 rid) {
    u8 i;
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
    /* Inside open container? loc must be an object ID (>= NUM_ROOMS) */
    if (loc >= NUM_ROOMS && loc < NUM_OBJECTS) {
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


/* ---- Room descriptions (inline to avoid char* array issues) ---- */
static void engine_print_room_desc(u8 rid) {
    switch(rid) {
        case ROOM_WEST_OF_HOUSE:
            text_println("Open field west of a");
            text_println("white house, boarded");
            text_println("door. Mailbox here.");
            break;
        case ROOM_NORTH_OF_HOUSE:
            text_println("North side of white");
            text_println("house. No door here,");
            text_println("windows boarded.");
            text_println("Forest path goes N.");
            break;
        case ROOM_SOUTH_OF_HOUSE:
            text_println("South side of white");
            text_println("house. No door here,");
            text_println("windows boarded.");
            break;
        case ROOM_BEHIND_HOUSE:
            text_println("Behind the white");
            text_println("house. Path leads E.");
            text_println("A window is open.");
            break;
        case ROOM_KITCHEN:
            text_println("The kitchen. Passage");
            text_println("leads west. Stairs");
            text_println("lead up.");
            break;
        case ROOM_LIVING_ROOM:
            text_println("Living room. Door E.");
            text_println("Gothic door W nailed");
            text_println("shut. Trophy case");
            if (g_obj_loc[OBJ_RUG] == ROOM_LIVING_ROOM)
                text_println("and oriental rug.");
            else if (g_obj_loc[OBJ_TRAP_DOOR] == ROOM_LIVING_ROOM)
                text_println("Trap door in floor!");
            break;
        case ROOM_ATTIC:
            text_println("The attic. Stairway");
            text_println("leads down.");
            break;
        case ROOM_CELLAR:
            text_println("Dark damp cellar.");
            text_println("Passages lead N, S.");
            text_println("Ramp leads W.");
            break;
        case ROOM_EAST_PASSAGE:
            text_println("Narrow E-W passage.");
            text_println("Round room to east.");
            break;
        case ROOM_ROUND_ROOM:
            text_println("Circular room with");
            text_println("passages all around.");
            text_println("A pit leads down.");
            break;
        case ROOM_NORTH_SOUTH_PASS:
            text_println("Long N-S passage.");
            text_println("Exits east and west.");
            break;
        case ROOM_EW_PASSAGE:
            text_println("Narrow E-W passage.");
            text_println("T-junction to north.");
            break;
        case ROOM_LOUD_ROOM:
            text_println("Large room. A loud");
            text_println("roar from the east.");
            break;
        case ROOM_GALLERY:
            text_println("Art gallery. Most");
            text_println("paintings stolen.");
            text_println("One oil painting.");
            break;
        case ROOM_STUDIO:
            text_println("Artist studio.");
            text_println("Paintings scattered.");
            text_println("An easel here.");
            break;
        case ROOM_CAVE:
            text_println("Small cave. Passages");
            text_println("NE and south.");
            break;
        case ROOM_MAZE_1:
        case ROOM_MAZE_2:
        case ROOM_MAZE_3:
            text_println("Maze of twisty");
            text_println("passages, all alike.");
            break;
        case ROOM_CLEARING:
            text_println("Clearing. Forest W,");
            text_println("white house to east.");
            break;
        case ROOM_FOREST_1:
        case ROOM_FOREST_2:
            text_println("Dense forest.");
            text_println("No clear path.");
            break;
        case ROOM_GRATING_CLEARING:
            text_println("A clearing. Iron");
            text_println("grating in ground.");
            break;
        default:
            text_println("You are somewhere.");
            break;
    }
}

/* ---- Object descriptions ---- */
static void engine_print_obj_desc(u8 obj) {
    switch(obj) {
        case OBJ_MAILBOX:     text_println("A small mailbox."); break;
        case OBJ_LEAFLET:
            text_println("WELCOME TO ZORK!");
            text_println("Collect treasures");
            text_println("for the trophy case.");
            break;
        case OBJ_MAT:         text_println("Mat reads: WELCOME"); break;
        case OBJ_LANTERN:     text_println("A brass lantern."); break;
        case OBJ_SWORD:       text_println("An elvish sword."); break;
        case OBJ_TROPHY_CASE: text_println("A large wooden case."); break;
        case OBJ_PAINTING:    text_println("A valuable painting."); break;
        case OBJ_SACK:        text_println("A small brown bag."); break;
        case OBJ_GARLIC:      text_println("It smells terrible."); break;
        case OBJ_BOTTLE:      text_println("Half full of water."); break;
        case OBJ_KNIFE:       text_println("A dull rusty knife."); break;
        case OBJ_CANARY:      text_println("A golden bird."); break;
        case OBJ_ROPE:        text_println("50 feet of rope."); break;
        case OBJ_COAL:        text_println("A lump of coal."); break;
        case OBJ_TORCH:       text_println("A crude torch."); break;
        case OBJ_GOLD_COIN:   text_println("A gold coin."); break;
        case OBJ_PLATINUM_BAR:text_println("A platinum bar."); break;
        case OBJ_POT_OF_GOLD: text_println("A pot of gold!"); break;
        case OBJ_JEWELS:      text_println("A heap of gems."); break;
        case OBJ_DIAMOND:     text_println("A huge diamond!"); break;
        case OBJ_EMERALD:     text_println("A fine emerald."); break;
        case OBJ_RUBY:        text_println("A giant ruby."); break;
        case OBJ_SCREWDRIVER: text_println("A screwdriver."); break;
        case OBJ_WINDOW:    text_println("A small window,"); text_println("slightly open."); break;
        case OBJ_RUG:       text_println("A large oriental"); text_println("rug."); break;
        case OBJ_TRAP_DOOR: text_println("A trap door in"); text_println("the floor."); break;
        default:              text_println("Nothing special."); break;
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

    /* List objects in room */
    shown = 0;
    for (i = 0; i < NUM_OBJECTS; i++) {
        if (g_obj_loc[i] != rid) continue;
        if (!shown) {
            text_println("You can see:");
            shown = 1;
        }
        text_print("  ");
        text_println(g_obj_name[i]);
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
    dest = g_room_exits[g_player_room * 6 + dir];
    if (dest == NO_EXIT) {
        text_println("You can't go that way.");
        return;
    }
    g_player_room = dest;
    g_moves = g_moves + 1;
    engine_describe_room(dest, 0);
    /* Burn lamp */
    if (g_obj_flags[OBJ_LANTERN] & OBJ_LIT) {
        if (g_lamp_fuel > 0) g_lamp_fuel--;
        if (g_lamp_fuel == 0) {
            g_obj_flags[OBJ_LANTERN] &= ~OBJ_LIT;
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
        text_println("You're carrying too\nmuch.");
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
    if (obj == OBJ_LANTERN) {
        if (g_obj_flags[OBJ_LANTERN] & OBJ_LIT)
            text_println("It is on.");
        else
            text_println("It is off.");
    }
    if (obj == OBJ_TROPHY_CASE) {
        u8 i;
        u8 f;
        f = 0;
        for (i = 0; i < NUM_OBJECTS; i++) {
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
    {
        u8 i;
        u8 f;
        f = 0;
        text_println("Opened.");
        for (i = 0; i < NUM_OBJECTS; i++) {
            if (g_obj_loc[i] != obj) continue;
            if (!f) { text_println("Inside:"); f = 1; }
            text_print("  ");
            text_println(g_obj_name[i]);
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
    if (obj == OBJ_LEAFLET || obj == OBJ_MAT) {
        engine_print_obj_desc(obj);
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
    if (obj == OBJ_LANTERN && g_lamp_fuel == 0) { text_println("Batteries dead."); return; }
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

    /* Echo command to scroll buffer: "> VERB NOUN" */
    cmd_buf[0] = '>';
    cmd_buf[1] = ' ';
    ci = 2;
    /* verb name */
    vi = 0;
    while (g_verb_names[verb][vi] && ci < 20) {
        cmd_buf[ci++] = g_verb_names[verb][vi++];
    }
    /* noun name */
    if (obj != 0xFF && obj < NUM_OBJECTS) {
        if (ci < 20) cmd_buf[ci++] = ' ';
        ni = 0;
        while (g_obj_name[obj][ni] && ci < 20) {
            cmd_buf[ci++] = g_obj_name[obj][ni++];
        }
    } else if (obj >= 0xF0 && obj <= 0xF5) {
        if (ci < 20) cmd_buf[ci++] = ' ';
        ni = 0;
        while (g_dir_names[obj - 0xF0][ni] && ci < 20) {
            cmd_buf[ci++] = g_dir_names[obj - 0xF0][ni++];
        }
    }
    cmd_buf[ci] = 0;
    text_newline();
    text_println(cmd_buf);

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
        case V_TAKE:     do_take(obj);    break;
        case V_DROP:     do_drop(obj);    break;
        case V_EXAMINE:  do_examine(obj); break;
        case V_OPEN:     do_open(obj);    break;
        case V_CLOSE:    do_close(obj);   break;
        case V_READ:     do_read(obj);    break;
        case V_TURN_ON:  do_turn_on(obj); break;
        case V_TURN_OFF: do_turn_off(obj);break;
        case V_ATTACK:
            text_println("Violence isn't the\nanswer here.");
            g_moves = g_moves + 1;
            break;
        case V_PUT:  do_drop(obj); break;
        case V_GO:
            if (obj >= DIR_NOUN_BASE && obj <= DIR_NOUN_BASE + 5) {
                do_go(obj - DIR_NOUN_BASE);
            } else {
                text_println("Go which way?");
            }
            break;
        case V_SAVE:    zork_save(); break;
        case V_RESTORE:
            zork_load();
            engine_describe_room(g_player_room, 1);
            break;
        case V_ENTER:
            /* Context-sensitive ENTER based on current room */
            if (g_player_room == ROOM_BEHIND_HOUSE) {
                if (g_obj_flags[OBJ_WINDOW] & OBJ_OPEN) {
                    g_player_room = ROOM_KITCHEN;
                    g_moves = g_moves + 1;
                    engine_describe_room(ROOM_KITCHEN, 0);
                } else {
                    text_println("The window is closed.");
                }
            } else {
                text_println("There's nothing to");
                text_println("enter here.");
            }
            break;
        case V_CLIMB:
            text_println("You can't climb that.");
            g_moves = g_moves + 1;
            break;
        case V_MOVE:
            if (obj == OBJ_RUG && g_player_room == ROOM_LIVING_ROOM) {
                /* Moving rug reveals trap door */
                g_obj_loc[OBJ_RUG] = LOC_GONE;
                g_obj_loc[OBJ_TRAP_DOOR] = ROOM_LIVING_ROOM;
                text_println("You move the rug,");
                text_println("revealing a trap door!");
            } else {
                text_println("You can't move that.");
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
    text_draw_status();
}

void engine_init(void) {
    u8 i;
    g_player_room = ROOM_WEST_OF_HOUSE;
    g_score       = 0;
    g_moves       = 0;
    g_lamp_fuel   = 200;
    g_dead        = 0;
    for (i = 0; i < NUM_OBJECTS; i++) {
        g_obj_loc[i]   = g_obj_default_loc[i];
        g_obj_flags[i] = g_obj_default_flags[i];
    }
    /* Set room flags explicitly - above ground rooms get ROOM_ABOVE */
    for (i = 0; i < NUM_ROOMS; i++) {
        g_room_flags[i] = 0;
    }
    /* Above-ground rooms: 0-3, 19-22 */
    g_room_flags[ROOM_WEST_OF_HOUSE]    = ROOM_ABOVE;
    g_room_flags[ROOM_NORTH_OF_HOUSE]   = ROOM_ABOVE;
    g_room_flags[ROOM_SOUTH_OF_HOUSE]   = ROOM_ABOVE;
    g_room_flags[ROOM_BEHIND_HOUSE]     = ROOM_ABOVE;
    g_room_flags[ROOM_CLEARING]         = ROOM_ABOVE;
    g_room_flags[ROOM_FOREST_1]         = ROOM_ABOVE;
    g_room_flags[ROOM_FOREST_2]         = ROOM_ABOVE;
    g_room_flags[ROOM_GRATING_CLEARING] = ROOM_ABOVE;
    /* Kitchen and Living Room have windows - always lit */
    g_room_flags[ROOM_KITCHEN]      |= ROOM_ABOVE;
    g_room_flags[ROOM_LIVING_ROOM]  |= ROOM_ABOVE;
    /* All underground rooms need lantern */
}
