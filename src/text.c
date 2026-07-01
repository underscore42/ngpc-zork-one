/* text.c - Zork Neo: Text output engine
 *
 * Manages a ring buffer of MAX_SCROLL_LINES lines, each MAX_LINE_LEN chars.
 * Displays TEXT_ROWS lines at a time. Up/Down scrolls the window.
 * Word-wraps at MAX_LINE_LEN automatically.
 *
 * NGPC constraints obeyed:
 *   - SysSetSystemFont() called once at init, never per-frame
 *   - FillScreen not ClearScreen (tile 0 is font glyph)
 *   - PrintString for row writes
 *   - No volatile u16* stores
 *   - No signed multiply/divide
 *   - No declarations after statements
 */
#include "text.h"

/* ---- Global state (defined here, declared extern in zork_types.h) ---- */
u8   g_scroll_top;
u8   g_scroll_count;
char g_lines[MAX_SCROLL_LINES][MAX_LINE_LEN + 1];

/* ---- Internal: current write position within active line ---- */
static u8  s_col;       /* column cursor (0..MAX_LINE_LEN-1) */
static u8  s_line;      /* ring buffer write index */

/* ---- Helpers ---- */

/* Advance to next line in ring buffer */
static void next_line(void) {
    u8 i;
    s_col = 0;
    s_line = (s_line + 1) & (MAX_SCROLL_LINES - 1);
    /* Clear the new line */
    for (i = 0; i <= MAX_LINE_LEN; i++) g_lines[s_line][i] = 0;

    if (g_scroll_count < MAX_SCROLL_LINES) {
        g_scroll_count++;
    } else {
        /* Ring wrapped: push top forward */
        g_scroll_top = (g_scroll_top + 1) & (MAX_SCROLL_LINES - 1);
    }

    /* Auto-scroll visible window to show newest content */
    if (g_scroll_count >= TEXT_ROWS) {
        g_scroll_top = (s_line + 1 + MAX_SCROLL_LINES - TEXT_ROWS)
                       & (MAX_SCROLL_LINES - 1);
    }
}

/* Emit one character into the current line */
static void emit_char(char c) {
    if (s_col >= MAX_LINE_LEN) {
        next_line();
    }
    g_lines[s_line][s_col] = c;
    s_col++;
}

void text_init(void) {
    u8 i;
    u8 j;
    s_col = 0;
    s_line = 0;
    g_scroll_top = 0;
    g_scroll_count = 1;
    for (i = 0; i < MAX_SCROLL_LINES; i++) {
        for (j = 0; j <= MAX_LINE_LEN; j++) {
            g_lines[i][j] = 0;
        }
    }
}

/* Word-wrap print: handles \n, wraps at word boundaries */
void text_print(const char *s) {
    const char *p;
    const char *word_start;
    u8 word_len;
    u8 k;

    p = s;
    while (*p) {
        if (*p == '\n') {
            next_line();
            p++;
            continue;
        }

        /* Find word length */
        word_start = p;
        word_len = 0;
        while (p[word_len] && p[word_len] != ' ' && p[word_len] != '\n') {
            word_len++;
        }

        /* Will this word fit on current line? */
        if (s_col > 0 && (u8)(s_col + word_len) > MAX_LINE_LEN) {
            next_line();
        }

        /* Emit the word */
        for (k = 0; k < word_len; k++) {
            emit_char(word_start[k]);
        }
        p += word_len;

        /* Emit space if not at end/newline */
        if (*p == ' ') {
            if (s_col < MAX_LINE_LEN) {
                emit_char(' ');
            }
            p++;
        }
    }
}

void text_newline(void) {
    next_line();
}

void text_println(const char *s) {
    text_print(s);
    next_line();
}

void text_clear(void) {
    text_init();
    FillScreen(SCR_1_PLANE, ' ', PAL_NORMAL);
}

/* ---- Redraw visible TEXT_ROWS from scroll buffer ---- */
void text_redraw(void) {
    u8 row;
    u8 idx;
    u8 c;
    char buf[MAX_LINE_LEN + 1];

    for (row = 0; row < TEXT_ROWS; row++) {
        idx = (g_scroll_top + row) & (MAX_SCROLL_LINES - 1);

        /* Copy line to local buf, pad with spaces */
        for (c = 0; c < MAX_LINE_LEN; c++) {
            buf[c] = g_lines[idx][c] ? g_lines[idx][c] : ' ';
        }
        buf[MAX_LINE_LEN] = 0;

        PrintString(SCR_1_PLANE, PAL_NORMAL, 0, row, buf);
    }
}

void text_scroll_up(void) {
    u8 oldest;
    if (g_scroll_count <= TEXT_ROWS) return;
    oldest = (u8)((s_line + 1 + MAX_SCROLL_LINES - g_scroll_count)
                  & (MAX_SCROLL_LINES - 1));
    if (g_scroll_top != oldest) {
        g_scroll_top = (u8)((g_scroll_top + MAX_SCROLL_LINES - 1)
                            & (MAX_SCROLL_LINES - 1));
        text_redraw();
    }
}

void text_scroll_down(void) {
    u8 bottom_visible;
    bottom_visible = (g_scroll_top + TEXT_ROWS - 1) & (MAX_SCROLL_LINES - 1);
    if (bottom_visible != s_line) {
        g_scroll_top = (g_scroll_top + 1) & (MAX_SCROLL_LINES - 1);
        text_redraw();
    }
}

/* ---- Row writers ---- */

static void pad_to_20(char *buf, char *s) {
    u8 i;
    u8 len;
    len = 0;
    while (s[len] && len < MAX_LINE_LEN) { buf[len] = s[len]; len++; }
    for (i = len; i < MAX_LINE_LEN; i++) buf[i] = ' ';
    buf[MAX_LINE_LEN] = 0;
}

void text_put_row(u8 row, u8 pal, char *s) {
    char buf[MAX_LINE_LEN + 1];
    pad_to_20(buf, s);
    PrintString(SCR_1_PLANE, pal, 0, row, buf);
}

void text_put_row_centered(u8 row, u8 pal, char *s) {
    char buf[MAX_LINE_LEN + 1];
    u8 len;
    u8 pad;
    u8 i;
    len = 0;
    while (s[len]) len++;
    if (len >= MAX_LINE_LEN) {
        text_put_row(row, pal, s);
        return;
    }
    pad = (MAX_LINE_LEN - len) >> 1;
    for (i = 0; i < MAX_LINE_LEN; i++) buf[i] = ' ';
    for (i = 0; i < len; i++) buf[pad + i] = s[i];
    buf[MAX_LINE_LEN] = 0;
    PrintString(SCR_1_PLANE, pal, 0, row, buf);
}

void text_draw_divider(void) {
    PrintString(SCR_1_PLANE, PAL_DIVIDER, 0, ROW_DIVIDER,
                "--------------------");
}

/* ---- Verb/Noun selector rows ---- */
/* Verb list: g_verb_names[g_verb_idx] is selected
 * prev = (g_verb_idx-1+NUM_VERBS)%NUM_VERBS
 * next = (g_verb_idx+1)%NUM_VERBS
 * Format: "< PREVVERB [VERB] NEXTV>"  truncated to 20
 *
 * Row format (20 chars):
 *   "< PREV  [CURR]  NXT>"
 * We build it manually to fit exactly 20 chars.
 */

static void build_selector_row(char *buf, const char *prev_s, const char *cur_s, const char *next_s,
                                u8 is_active) {
    /* Layout: "< " + prev(5) + " [" + cur(6) + "] " + next(3) + ">" */
    /* Total: 2+5+2+6+2+3 = 20 -- adjust cur width dynamically */
    u8 i;
    u8 pos;
    u8 len;

    for (i = 0; i < MAX_LINE_LEN; i++) buf[i] = ' ';
    buf[MAX_LINE_LEN] = 0;

    /* Arrow left */
    buf[0] = is_active ? '\x11' : ' ';  /* char 17 = left arrow in font */
    buf[1] = ' ';

    /* Prev word (up to 5 chars, right-aligned at col 7) */
    len = 0; while (prev_s[len] && len < 5) len++;
    pos = 7 - len;
    for (i = 0; i < len; i++) buf[pos + i] = prev_s[i];

    /* Bracket + current (up to 7 chars) */
    buf[8] = '[';
    len = 0; while (cur_s[len] && len < 7) len++;
    for (i = 0; i < len; i++) buf[9 + i] = cur_s[i];
    buf[9 + len] = ']';

    /* Next word (up to 4 chars starting at col 17) */
    buf[17] = ' ';
    len = 0; while (next_s[len] && len < 3) len++;
    for (i = 0; i < len; i++) buf[17 + i] = next_s[i];

    /* Arrow right */
    buf[19] = is_active ? '\x10' : ' '; /* char 16 = right arrow */
}

void text_draw_selector(void) {
    char buf[MAX_LINE_LEN + 1];
    u8 prev_v;
    u8 next_v;
    u8 prev_n;
    u8 next_n;
    u8 i;
    char *cur_noun;
    char *prev_noun;
    char *next_noun;

    prev_v = (g_verb_idx + NUM_VERBS - 1) % NUM_VERBS;
    next_v = (g_verb_idx + 1) % NUM_VERBS;

    build_selector_row(buf,
        g_verb_names[prev_v],
        g_verb_names[g_verb_idx],
        g_verb_names[next_v],
        g_input_mode == MODE_VERB);

    PrintString(SCR_1_PLANE,
                g_input_mode == MODE_VERB ? PAL_HILITE : PAL_DIM,
                0, ROW_VERB, buf);

    /* Noun row */
    if (g_noun_count > 0) {
        prev_n = (g_noun_idx + MAX_NOUN_LIST - 1) % g_noun_count;
        next_n = (g_noun_idx + 1) % g_noun_count;

        /* Direction nouns (for GO) use g_dir_names, objects use g_obj_name */
        cur_noun  = (g_noun_list[g_noun_idx] >= 0xF0)
                    ? (char*)g_dir_names[g_noun_list[g_noun_idx] - 0xF0]
                    : (char*)g_obj_name[g_noun_list[g_noun_idx]];
        prev_noun = (g_noun_list[prev_n] >= 0xF0)
                    ? (char*)g_dir_names[g_noun_list[prev_n] - 0xF0]
                    : (char*)g_obj_name[g_noun_list[prev_n]];
        next_noun = (g_noun_list[next_n] >= 0xF0)
                    ? (char*)g_dir_names[g_noun_list[next_n] - 0xF0]
                    : (char*)g_obj_name[g_noun_list[next_n]];

        build_selector_row(buf,
            prev_noun, cur_noun, next_noun,
            g_input_mode == MODE_NOUN);

        PrintString(SCR_1_PLANE,
                    g_input_mode == MODE_NOUN ? PAL_CMD : PAL_DIM,
                    0, ROW_NOUN, buf);
    } else {
        PrintString(SCR_1_PLANE, PAL_DIM, 0, ROW_NOUN,
                    "    [ press A ]     ");
    }
}

/* ---- Command line row ---- */
void text_print_cmd(const char *verb, char *noun) {
    char buf[MAX_LINE_LEN + 1];
    u8 i;
    u8 pos;

    for (i = 0; i < MAX_LINE_LEN; i++) buf[i] = ' ';
    buf[MAX_LINE_LEN] = 0;
    buf[0] = '>';
    buf[1] = ' ';
    pos = 2;
    i = 0;
    {
        u8 vi2;
        vi2 = 0;
        while (verb[vi2] && pos < MAX_LINE_LEN) buf[pos++] = verb[vi2++];
    }
    if (noun && noun[0] && pos < MAX_LINE_LEN - 1) {
        buf[pos++] = ' ';
        i = 0;
        while (noun[i] && pos < MAX_LINE_LEN) {
            buf[pos++] = noun[i++];
        }
    }
    /* Cursor underscore */
    if (pos < MAX_LINE_LEN) buf[pos] = '_';
    /* Use PutTile to guarantee all 20 chars are written */
    {
        u8 tx;
        for (tx = 0; tx < MAX_LINE_LEN; tx++) {
            PutTile(SCR_1_PLANE, PAL_CMD, tx, ROW_CMD, buf[tx]);
        }
    }
}

/* ---- Status bar ---- */
void text_draw_status(void) {
    char buf[MAX_LINE_LEN + 1];
    u8 i;
    u16 s;
    u16 m;

    /* "SCORE:0000 MOVES:0000" - 21 chars, trim to 20 */
    for (i = 0; i < MAX_LINE_LEN; i++) buf[i] = ' ';
    buf[MAX_LINE_LEN] = 0;

    buf[0] = 'S'; buf[1] = ':';
    s = g_score;
    buf[5] = '0' + s % 10; s = s / 10;
    buf[4] = '0' + s % 10; s = s / 10;
    buf[3] = '0' + s % 10; s = s / 10;
    buf[2] = '0' + s % 10;

    buf[7] = 'M'; buf[8] = ':';
    m = g_moves;
    buf[12] = '0' + m % 10; m = m / 10;
    buf[11] = '0' + m % 10; m = m / 10;
    buf[10] = '0' + m % 10; m = m / 10;
    buf[9]  = '0' + m % 10;

    /* Lamp indicator */
    if (g_obj_flags[OBJ_LAMP] & OBJ_LIT) {
        buf[14] = 'L'; buf[15] = 'A'; buf[16] = 'M'; buf[17] = 'P';
    }

    PrintString(SCR_1_PLANE, PAL_STATUS, 0, ROW_STATUS, buf);
}
