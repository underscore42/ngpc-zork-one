/* wizard.c - Zork Neo: WIZARD mode debug system
 *
 * Activated by Konami code: Up Up Down Down Left Right Left Right B A
 * Once active:
 *   - All rooms lit (lamp not needed)
 *   - OPTION+Up/Down teleports to next/prev room
 *   - OPTION+A dumps current state to transcript buffer
 *   - Status bar shows room ID and object count
 *   - WIZARD verb in selector for room jump
 */
#include "zork_types.h"
#include "text.h"
#include "engine.h"

/* ---- Konami code detector ---- */
/* Sequence: U U D D L R L R B A */
static u8 KONAMI[10] = {
    J_UP, J_UP, J_DOWN, J_DOWN,
    J_LEFT, J_RIGHT, J_LEFT, J_RIGHT,
    J_B, J_A
};

static u8 s_konami_pos = 0;
u8 g_wizard_mode = 0;

/* ---- Transcript buffer (written to flash slot 2) ---- */
/* 512 bytes = 256 u16s at flash offset 0x1FA200 */
#define TRANSCRIPT_MAX  480
static char s_transcript[TRANSCRIPT_MAX];
static u16  s_transcript_len = 0;

void wizard_log(const char *s) {
    u8 i;
    if (!g_wizard_mode) return;
    i = 0;
    while (s[i] && s_transcript_len < TRANSCRIPT_MAX - 2) {
        s_transcript[s_transcript_len++] = s[i++];
    }
    s_transcript[s_transcript_len++] = '\n';
}

void wizard_log_cmd(const char *verb, const char *noun) {
    char buf[24];
    u8 i;
    u8 p;
    if (!g_wizard_mode) return;
    buf[0] = '>';
    buf[1] = ' ';
    p = 2;
    i = 0;
    while (verb[i] && p < 22) buf[p++] = verb[i++];
    if (noun && noun[0]) {
        buf[p++] = ' ';
        i = 0;
        while (noun[i] && p < 22) buf[p++] = noun[i++];
    }
    buf[p] = 0;
    wizard_log(buf);
}

static void wizard_flush_transcript(void) {
    /* Pack transcript into u16 buffer and flash-save */
    u16 buf[128];
    u8 i;
    u16 w;
    for (i = 0; i < 128; i++) buf[i] = 0;
    buf[0] = 0xBEEF;  /* transcript magic */
    buf[1] = s_transcript_len;
    /* Pack 2 chars per word */
    for (i = 0; i < s_transcript_len && i < 252; i++) {
        w = (u16)(i >> 1) + 2;
        if (i & 1) {
            buf[w] = (buf[w] & 0xFF00u) | (u8)s_transcript[i];
        } else {
            buf[w] = ((u16)(u8)s_transcript[i] << 8) | (buf[w] & 0x00FFu);
        }
    }
    Flash((void*)buf);
    text_println("Transcript saved.");
}

/* ---- Konami detector - call each frame with pad_press ---- */
void wizard_check_konami(u8 pad_press) {
    if (pad_press == 0) return;  /* only act on actual presses */

    if (pad_press == KONAMI[s_konami_pos]) {
        s_konami_pos++;
        if (s_konami_pos >= 10) {
            s_konami_pos = 0;
            g_wizard_mode = !g_wizard_mode;
            if (g_wizard_mode) {
                text_println("*** WIZARD MODE ON ***");
                text_println("OPT+UD: teleport room");
                text_println("OPT+A:  save transcript");
                text_println("OPT+B:  run script");
            } else {
                text_println("*** WIZARD MODE OFF ***");
            }
        }
    } else if (pad_press == KONAMI[0]) {
        s_konami_pos = 1;
    } else {
        s_konami_pos = 0;
    }
}

/* ---- Wizard tick - called from parser when OPTION held ---- */
void wizard_tick(u8 pad_press, u8 pad_cur) {
    u8 next_room;
    if (!g_wizard_mode) return;

    /* OPTION+Up: teleport to prev room */
    if (pad_cur & J_OPTION) {
        if (pad_press & J_UP) {
            next_room = g_player_room;
            if (next_room > 0) next_room--;
            g_player_room = next_room;
            text_newline();
            engine_describe_room(g_player_room, 1);
        }
        if (pad_press & J_DOWN) {
            next_room = g_player_room;
            if (next_room < NUM_ROOMS - 1) next_room++;
            g_player_room = next_room;
            text_newline();
            engine_describe_room(g_player_room, 1);
        }
        if (pad_press & J_A) {
            wizard_flush_transcript();
        }
    }
}

/* ---- Wizard room_is_lit override ---- */
u8 wizard_is_lit(void) {
    return g_wizard_mode;
}

/* ---- Draw wizard status overlay on status bar ---- */
void wizard_draw_status(void) {
    char buf[MAX_LINE_LEN + 1];
    u8 i;
    u8 p;
    u16 r;

    if (!g_wizard_mode) return;

    for (i = 0; i < MAX_LINE_LEN; i++) buf[i] = ' ';
    buf[MAX_LINE_LEN] = 0;

    /* "WIZ R:00 S:000 M:000" */
    buf[0]='W'; buf[1]='I'; buf[2]='Z';
    buf[3]=' '; buf[4]='R'; buf[5]=':';
    r = g_player_room;
    buf[7] = '0' + r % 10; r = r / 10;
    buf[6] = '0' + r % 10;

    buf[8]=' '; buf[9]='S'; buf[10]=':';
    r = g_score;
    buf[13]='0'+r%10; r=r/10;
    buf[12]='0'+r%10; r=r/10;
    buf[11]='0'+r%10;

    buf[14]=' '; buf[15]='M'; buf[16]=':';
    r = g_moves;
    buf[19]='0'+r%10; r=r/10;
    buf[18]='0'+r%10; r=r/10;
    buf[17]='0'+r%10;

    PrintString(SCR_1_PLANE, PAL_ERR, 0, ROW_STATUS, buf);
}
