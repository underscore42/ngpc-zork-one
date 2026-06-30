/* script.c - Zork Neo: Scripted playback for testing
 *
 * A ROM array of (verb_id, obj_id) pairs.
 * Activated by OPTION+B in wizard mode.
 * Auto-executes one command per second, logs results.
 *
 * Script IDs match checkpoint sections in the walkthrough.
 */
#include "zork_types.h"
#include "text.h"
#include "engine.h"
#include "wizard.h"

#define SCRIPT_END  0xFF
#define NO_OBJ      0xFF

/* ---- Checkpoint 1: West of House -> Cellar with painting ---- */
/* S, E, ENTER, W, TAKE lamp, MOVE rug, OPEN trap door,
   TURN ON lamp, D, S, E, TAKE painting, N, U, U,
   TAKE knife, TAKE rope, D, W, OPEN case,
   DROP painting (scores), DROP knife, TAKE sword,
   OPEN trap door, D */
static const u8 SCRIPT_CP1[] = {
    V_SOUTH,    NO_OBJ,          /* -> South of House */
    V_EAST,     NO_OBJ,          /* -> Behind House */
    V_ENTER,    NO_OBJ,          /* -> Kitchen */
    V_WEST,     NO_OBJ,          /* -> Living Room */
    V_TAKE,     OBJ_LANTERN,     /* take lamp */
    V_TAKE,     OBJ_SWORD,       /* take sword */
    V_MOVE,     OBJ_RUG,         /* reveal trap door */
    V_OPEN,     OBJ_TRAP_DOOR,   /* open trap door */
    V_TURN_ON,  OBJ_LANTERN,     /* lamp on */
    V_DOWN,     NO_OBJ,          /* -> Cellar */
    V_SOUTH,    NO_OBJ,          /* -> Cave */
    V_ATTACK,   OBJ_TROLL,       /* attack troll */
    V_ATTACK,   OBJ_TROLL,       /* attack troll */
    V_ATTACK,   OBJ_TROLL,       /* attack troll */
    V_EAST,     NO_OBJ,          /* -> East Passage */
    SCRIPT_END, SCRIPT_END
};

/* ---- Script state ---- */
static u8  s_script_active  = 0;
static u8  s_script_pos     = 0;
static u8  s_script_timer   = 0;
static const u8 *s_script   = 0;

#define SCRIPT_FRAME_DELAY  60  /* 1 second at 60fps */

void script_start(u8 checkpoint) {
    if (!g_wizard_mode) {
        text_println("Wizard mode required.");
        return;
    }
    s_script_pos   = 0;
    s_script_timer = SCRIPT_FRAME_DELAY;
    if (checkpoint == 1) {
        s_script = SCRIPT_CP1;
        text_println("--- SCRIPT CP1 START ---");
    } else {
        text_println("Unknown checkpoint.");
        return;
    }
    s_script_active = 1;
}

void script_stop(void) {
    s_script_active = 0;
    text_println("--- SCRIPT STOPPED ---");
}

u8 script_is_active(void) {
    return s_script_active;
}

/* Call once per frame from main loop */
void script_tick(void) {
    u8 verb;
    u8 obj;

    if (!s_script_active) return;
    if (s_script == 0) return;

    if (s_script_timer > 0) {
        s_script_timer--;
        return;
    }

    verb = s_script[s_script_pos];
    obj  = s_script[s_script_pos + 1];

    if (verb == SCRIPT_END) {
        s_script_active = 0;
        text_println("--- SCRIPT COMPLETE ---");
        return;
    }

    /* Execute command */
    engine_execute(verb, obj);
    engine_refresh_screen();

    s_script_pos  += 2;
    s_script_timer = SCRIPT_FRAME_DELAY;
}
