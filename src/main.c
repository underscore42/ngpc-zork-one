/* main.c - Zork Neo for NGPC
 * Studio So Not Kansai / underscore42
 */

#define CARTHDR_IMPL
#include "carthdr.h"
#include "ngpc.h"
#include "library.h"
#include "zork_types.h"
#include "text.h"
#include "parser.h"
#include "engine.h"
#include "wizard.h"
#include "script.h"

static void setup_palettes(void) {
    SetBackgroundColour(RGB(0, 0, 0));
    SetPalette(SCR_1_PLANE, 0, 0, RGB(15,15,15), RGB(15,15,15), RGB(15,15,15));
    SetPalette(SCR_1_PLANE, 1, 0, RGB(6,6,6),   RGB(6,6,6),   RGB(6,6,6));
    SetPalette(SCR_1_PLANE, 2, 0, RGB(15,10,0), RGB(15,10,0), RGB(15,10,0));
    SetPalette(SCR_1_PLANE, 3, 0, RGB(4,15,5),  RGB(4,15,5),  RGB(4,15,5));
    SetPalette(SCR_1_PLANE, 4, 0, RGB(4,14,15), RGB(4,14,15), RGB(4,14,15));
    SetPalette(SCR_1_PLANE, 5, 0, RGB(5,8,15),  RGB(5,8,15),  RGB(5,8,15));
    SetPalette(SCR_1_PLANE, 6, 0, RGB(15,3,3),  RGB(15,3,3),  RGB(15,3,3));
    SetPalette(SCR_1_PLANE, 7, 0, RGB(4,4,4),   RGB(4,4,4),   RGB(4,4,4));
}

static void draw_title(void) {
    ClearScreen(SCR_1_PLANE);
    setup_palettes();
    SysSetSystemFont();

    PrintString(SCR_1_PLANE, PAL_TITLE,  3, 0,  "ZORK I: THE GREAT");
    PrintString(SCR_1_PLANE, PAL_TITLE,  2, 1,  "UNDERGROUND EMPIRE");
    PrintString(SCR_1_PLANE, PAL_DIM,    0, 2,  "--------------------");
    PrintString(SCR_1_PLANE, PAL_DIM,    0, 3,  "(c) 1981 Infocom Inc.");
    PrintString(SCR_1_PLANE, PAL_DIM,    0, 4,  "MIT License 2025");
    PrintString(SCR_1_PLANE, PAL_NORMAL, 0, 6,  "Controls:");
    PrintString(SCR_1_PLANE, PAL_DIM,    0, 7,  "LR   pick verb/noun");
    PrintString(SCR_1_PLANE, PAL_DIM,    0, 8,  "A    confirm");
    PrintString(SCR_1_PLANE, PAL_DIM,    0, 9,  "B    cancel noun");
    PrintString(SCR_1_PLANE, PAL_DIM,    0, 10, "UD   scroll text");
    PrintString(SCR_1_PLANE, PAL_DIM,    0, 11, "OPT  inventory");
    PrintString(SCR_1_PLANE, PAL_DIM,    0, 13, "SAVE/RESTORE via");
    PrintString(SCR_1_PLANE, PAL_DIM,    0, 14, "verb selector");
    PrintString(SCR_1_PLANE, PAL_HILITE, 2, 16, "Press A to begin");
    PrintString(SCR_1_PLANE, PAL_DIM,    0, 18, "STUDIO SO NOT KANSAI");
}

static void wait_for_a(void) {
    u8 prev;
    u8 cur;
    prev = 0;
    for (;;) {
        WaitVsync();
        cur = JOYPAD & 0x7F;
        if ((cur & J_A) && !(prev & J_A)) return;
        prev = cur;
    }
}

void main(void) {
    InitNGPC();

    draw_title();
    wait_for_a();

    /* Init all systems */
    text_init();
    engine_init();
    parser_init();

    /* Build initial noun list so selector is populated on first frame */
    parser_build_nouns();

    /* Game screen */
    ClearScreen(SCR_1_PLANE);
    setup_palettes();
    SysSetSystemFont();

    engine_describe_room(ROOM_WEST_OF_HOUSE, 1);
    engine_refresh_screen();

    for (;;) {
        WaitVsync();
        WATCHDOG = WATCHDOG_CLEAR;
        parser_tick();
        script_tick();
    }
}
