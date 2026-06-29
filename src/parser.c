/* parser.c - Zork Neo: Input parser and command dispatcher
 *
 * Button mapping:
 *   Up/Down   = scroll text output window
 *   Left/Right = cycle verb (MODE_VERB) or noun (MODE_NOUN)
 *   A          = confirm verb -> move to MODE_NOUN
 *                confirm noun -> execute command
 *   B          = cancel noun selection, back to verb
 *   OPTION     = show inventory (shortcut)
 *
 * Special: verbs NORTH/SOUTH/EAST/WEST/UP/DOWN have no noun,
 * execute immediately on A from MODE_VERB.
 */
#include "parser.h"
#include "text.h"
#include "engine.h"

/* Global input state */
u8 g_pad_cur;
u8 g_pad_prev;
u8 g_pad_press;
u8 g_input_mode;
u8 g_verb_idx;
u8 g_noun_idx;
u8 g_noun_count;
u8 g_noun_list[MAX_NOUN_LIST];
u8 g_last_verb;
u8 g_last_noun;

/* Verbs that don't need a noun */
static u8 verb_needs_noun(u8 v) {
    if (v == V_LOOK)      return 0;
    if (v == V_INVENTORY) return 0;
    if (v == V_WAIT)      return 0;
    if (v == V_AGAIN)     return 0;
    if (v == V_NORTH)     return 0;
    if (v == V_SOUTH)     return 0;
    if (v == V_EAST)      return 0;
    if (v == V_WEST)      return 0;
    if (v == V_UP)        return 0;
    if (v == V_DOWN)      return 0;
    if (v == V_SAVE)      return 0;
    if (v == V_RESTORE)   return 0;
    if (v == V_QUIT)      return 0;
    return 1;
}

/* Build noun list: objects visible in room + in player inventory,
 * filtered by what makes sense for the current verb */
void parser_build_nouns(void) {
    u8 i;
    u8 v;
    u8 loc;
    u8 flags;

    g_noun_count = 0;
    v = g_verb_idx;

    for (i = 0; i < NUM_OBJECTS && g_noun_count < MAX_NOUN_LIST; i++) {
        loc   = g_obj_loc[i];
        flags = g_obj_flags[i];

        /* Skip removed objects */
        if (loc == LOC_GONE) continue;

        /* Object must be accessible: in current room, or in player inventory,
         * or inside an open container in current room */
        if (loc != g_player_room && loc != LOC_PLAYER) {
            /* Is it inside a container that's in the room or player inv? */
            u8 cont_loc;
            u8 cont_flags;
            if (loc >= NUM_OBJECTS) continue;
            cont_loc   = g_obj_loc[loc];
            cont_flags = g_obj_flags[loc];
            if (!(cont_flags & OBJ_OPEN)) continue;
            if (cont_loc != g_player_room && cont_loc != LOC_PLAYER) continue;
        }

        /* Filter by verb */
        if (v == V_TAKE) {
            if (loc == LOC_PLAYER) continue;    /* already have it */
            if (!(flags & OBJ_TAKEABLE)) continue;
        }
        if (v == V_DROP || v == V_EXAMINE || v == V_READ) {
            /* anything accessible */
        }
        if (v == V_OPEN || v == V_CLOSE) {
            if (!(flags & (OBJ_CONTAINER | OBJ_LOCKED))) continue;
        }
        if (v == V_ATTACK) {
            /* anything (player can try to attack anything) */
        }
        if (v == V_TURN_ON || v == V_TURN_OFF) {
            if (!(flags & OBJ_LIGHT)) continue;
        }
        if (v == V_PUT) {
            /* PUT needs an obj in inventory */
            if (loc != LOC_PLAYER) continue;
        }

        g_noun_list[g_noun_count] = i;
        g_noun_count++;
    }

    g_noun_idx = 0;
}

void parser_init(void) {
    g_input_mode = MODE_VERB;
    g_verb_idx   = V_LOOK;
    g_noun_idx   = 0;
    g_noun_count = 0;
    g_last_verb  = V_LOOK;
    g_last_noun  = 0;
    g_pad_cur    = 0;
    g_pad_prev   = 0;
    g_pad_press  = 0;
}

/* Repeat-press: held for N frames gives auto-repeat */
static u8 s_held_frames;
#define REPEAT_DELAY  20
#define REPEAT_RATE    6

void parser_tick(void) {
    u8 moved;
    u8 joy;

    g_pad_prev  = g_pad_cur;
    g_pad_cur   = JOYPAD & 0x7F;   /* mask power bit */
    g_pad_press = g_pad_cur & ~g_pad_prev;

    /* Auto-repeat for left/right */
    joy = 0;
    if (g_pad_cur & (J_LEFT | J_RIGHT)) {
        s_held_frames++;
        if (s_held_frames == 1) {
            joy = g_pad_cur & (J_LEFT | J_RIGHT);
        } else if (s_held_frames > REPEAT_DELAY) {
            if (((u8)(s_held_frames - REPEAT_DELAY)) % REPEAT_RATE == 0) {
                joy = g_pad_cur & (J_LEFT | J_RIGHT);
            }
        }
    } else {
        s_held_frames = 0;
    }

    /* Up/Down: scroll text window always */
    if (g_pad_press & J_UP) {
        text_scroll_up();
        text_redraw();
        text_draw_divider();
        text_draw_selector();
        text_print_cmd(g_verb_names[g_verb_idx],
                       g_noun_count ? g_objects[g_noun_list[g_noun_idx]].name : "");
        text_draw_status();
        return;
    }
    if (g_pad_press & J_DOWN) {
        text_scroll_down();
        text_redraw();
        text_draw_divider();
        text_draw_selector();
        text_print_cmd(g_verb_names[g_verb_idx],
                       g_noun_count ? g_objects[g_noun_list[g_noun_idx]].name : "");
        text_draw_status();
        return;
    }

    /* OPTION = quick inventory */
    if (g_pad_press & J_OPTION) {
        engine_do_inventory();
        engine_refresh_screen();
        return;
    }

    moved = 0;

    if (g_input_mode == MODE_VERB) {
        if (joy & J_LEFT) {
            g_verb_idx = (g_verb_idx + NUM_VERBS - 1) % NUM_VERBS;
            moved = 1;
        }
        if (joy & J_RIGHT) {
            g_verb_idx = (g_verb_idx + 1) % NUM_VERBS;
            moved = 1;
        }
        if (moved) {
            parser_build_nouns();
            text_draw_selector();
            text_print_cmd(g_verb_names[g_verb_idx], "");
            text_draw_status();
        }

        if (g_pad_press & J_A) {
            if (!verb_needs_noun(g_verb_idx)) {
                /* Execute immediately */
                g_last_verb = g_verb_idx;
                g_last_noun = 0;
                engine_execute(g_verb_idx, 0xFF);
                engine_refresh_screen();
            } else {
                /* Move to noun selection */
                parser_build_nouns();
                if (g_noun_count > 0) {
                    g_input_mode = MODE_NOUN;
                    g_noun_idx = 0;
                    text_draw_selector();
                    text_print_cmd(g_verb_names[g_verb_idx],
                                   g_objects[g_noun_list[0]].name);
                } else {
                    text_println("Nothing here to do that with.");
                    engine_refresh_screen();
                }
            }
        }

    } else if (g_input_mode == MODE_NOUN) {
        if (joy & J_LEFT) {
            g_noun_idx = (g_noun_idx + g_noun_count - 1) % g_noun_count;
            moved = 1;
        }
        if (joy & J_RIGHT) {
            g_noun_idx = (g_noun_idx + 1) % g_noun_count;
            moved = 1;
        }
        if (moved) {
            text_draw_selector();
            text_print_cmd(g_verb_names[g_verb_idx],
                           g_objects[g_noun_list[g_noun_idx]].name);
        }

        if (g_pad_press & J_A) {
            /* Execute with selected noun */
            g_last_verb = g_verb_idx;
            g_last_noun = g_noun_list[g_noun_idx];
            g_input_mode = MODE_VERB;
            engine_execute(g_verb_idx, g_noun_list[g_noun_idx]);
            engine_refresh_screen();
        }
        if (g_pad_press & J_B) {
            /* Cancel back to verb */
            g_input_mode = MODE_VERB;
            text_draw_selector();
            text_print_cmd(g_verb_names[g_verb_idx], "");
        }
    }
}
