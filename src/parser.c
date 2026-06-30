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
#include "wizard.h"
#include "script.h"

/* Global input state */
static char s_empty[1] = {0};
static u8 s_action_delay = 0;

static char *noun_name(u8 nid) {
    if (nid >= 0xF0) return (char*)g_dir_names[nid - 0xF0];
    return (char*)g_obj_name[nid];
}
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
    if (v == V_ENTER)     return 0;
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

    /* GO verb: add available exit directions as nouns */
    if (v == V_GO) {
        u8 dir;
        for (dir = 0; dir < 6; dir++) {
            if (g_room_exits[g_player_room * 6 + dir] != NO_EXIT) {
                g_noun_list[g_noun_count] = 0xF0 + dir;
                g_noun_count++;
            }
        }
        g_noun_idx = 0;
        return;
    }

    for (i = 0; i < NUM_OBJECTS && g_noun_count < MAX_NOUN_LIST; i++) {
        loc   = g_obj_loc[i];
        flags = g_obj_flags[i];

        /* Skip removed objects */
        if (loc == LOC_GONE) continue;

        /* Object must be accessible: in current room, or in player inventory,
         * or inside an open container in current room */
        if (loc != g_player_room && loc != LOC_PLAYER) {
            /* Inside open container? loc must be object ID (>= NUM_ROOMS) */
            u8 cont_loc;
            u8 cont_flags;
            if (loc < NUM_ROOMS || loc >= NUM_OBJECTS) continue;
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
        if (v == V_ENTER) {
            /* ENTER - only show window when at Behind House */
            if (i != OBJ_WINDOW) continue;
            if (g_player_room != ROOM_BEHIND_HOUSE) continue;
        }
        if (v == V_MOVE) {
            if (i != OBJ_RUG) continue;
        }
        if (v == V_ATTACK) {
            /* Only attack troll when in troll room */
            if (i == OBJ_TROLL && g_player_room != ROOM_TROLL_ROOM) continue;
            if (i == OBJ_TROLL && !g_troll_alive) continue;
        }
        if (v == V_CLIMB) {
            if (flags & OBJ_TAKEABLE) continue;
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

    /* Konami code check */
    wizard_check_konami(g_pad_press);

    /* Wizard teleport (OPTION+UD) */
    wizard_tick(g_pad_press, g_pad_cur);

    /* Post-action delay - prevents A from firing repeatedly */
    if (s_action_delay > 0) {
        s_action_delay--;
        return;
    }

    /* Left/Right: single tap only (edge detect via pad_press) */
    joy = g_pad_press & (J_LEFT | J_RIGHT);

    /* Up/Down: scroll text with auto-repeat */
    if (g_pad_cur & (J_UP | J_DOWN)) {
        s_held_frames++;
    } else {
        s_held_frames = 0;
    }

    /* Auto-repeat fire for up/down scroll */
    {
        u8 do_ud;
        do_ud = 0;
        if (g_pad_press & (J_UP | J_DOWN)) do_ud = 1;
        else if (s_held_frames > REPEAT_DELAY) {
            if (((u8)(s_held_frames - REPEAT_DELAY)) % REPEAT_RATE == 0) do_ud = 1;
        }
        if (do_ud) {
            if (g_pad_cur & J_UP)   text_scroll_up();
            if (g_pad_cur & J_DOWN) text_scroll_down();
            text_redraw();
            text_draw_divider();
            text_draw_selector();
            text_print_cmd(g_verb_names[g_verb_idx],
                           g_noun_count ? noun_name(g_noun_list[g_noun_idx]) : s_empty);
            text_draw_status();
            return;
        }
    }

    /* OPTION combos */
    if (g_pad_press & J_OPTION) {
        if (g_wizard_mode && (g_pad_cur & J_B)) {
            /* OPTION+B in wizard mode: run checkpoint 1 script */
            script_start(1);
            engine_refresh_screen();
        } else {
            /* Normal OPTION: quick inventory */
            engine_do_inventory();
            engine_refresh_screen();
        }
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
            text_print_cmd(g_verb_names[g_verb_idx], s_empty);
            text_draw_status();
        }

        if (g_pad_press & J_A) {
            if (!verb_needs_noun(g_verb_idx)) {
                /* Execute immediately */
                g_last_verb = g_verb_idx;
                g_last_noun = 0;
                engine_execute(g_verb_idx, 0xFF);
                engine_refresh_screen();
                s_action_delay = 20;
            } else {
                /* Move to noun selection */
                parser_build_nouns();
                if (g_noun_count > 0) {
                    g_input_mode = MODE_NOUN;
                    g_noun_idx = 0;
                    text_draw_selector();
                    text_print_cmd(g_verb_names[g_verb_idx],
                                   noun_name(g_noun_list[0]));
                } else {
                    text_println("Nothing here to do that with.");
                    engine_refresh_screen();
                    s_action_delay = 20;
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
                           noun_name(g_noun_list[g_noun_idx]));
        }

        if (g_pad_press & J_A) {
            /* Execute with selected noun */
            g_last_verb = g_verb_idx;
            g_last_noun = g_noun_list[g_noun_idx];
            g_input_mode = MODE_VERB;
            engine_execute(g_verb_idx, g_noun_list[g_noun_idx]);
            engine_refresh_screen();
            s_action_delay = 20;
        }
        if (g_pad_press & J_B) {
            /* Cancel back to verb */
            g_input_mode = MODE_VERB;
            parser_build_nouns();
            text_draw_selector();
            text_print_cmd(g_verb_names[g_verb_idx], s_empty);
            text_draw_status();
        }
    }
}
