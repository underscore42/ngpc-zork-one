/* zork_sound.c - Zork Neo: UI click sounds
 *
 * Four short PSG blips for menu feedback. Uses the library SFX driver.
 *
 * NGPC constraints obeyed (hard-won):
 *   - NO const on SOUNDEFFECT arrays (cc900 breaks)
 *   - 2x WaitVsync() before InstallSounds() or real hardware is silent
 *
 * SOUNDEFFECT fields:
 *   Channel, Length, Repeat,
 *   InitialTone, ToneStep, ToneSpeed, ToneOWB, ToneLowerLimit, ToneUpperLimit,
 *   InitialVol, VolStep, VolSpeed, VolOWB, VolLowerLimit, VolUpperLimit
 */
#include "zork_sound.h"

static SOUNDEFFECT ui_sounds[] = {
    /* 0 SND_NAV: tiny mid blip, fast decay */
    { 1, 2, 0, 0x0100, 0x0000, 1, 0, 0x0080, 0x0200, 10, 5, 1, 0, 0, 15 },
    /* 1 SND_CONFIRM: brighter, slightly longer */
    { 1, 3, 0, 0x00C0, 0x0010, 1, 0, 0x0080, 0x0200, 12, 4, 1, 0, 0, 15 },
    /* 2 SND_CANCEL: lower blip */
    { 1, 3, 0, 0x0180, 0x0020, 1, 0, 0x0100, 0x0300, 11, 4, 1, 0, 0, 15 },
    /* 3 SND_TICK: softest, shortest, second channel */
    { 2, 2, 0, 0x0100, 0x0000, 1, 0, 0x0080, 0x0200,  8, 6, 1, 0, 0, 15 },
};

void zork_sound_init(void) {
    InstallSoundDriver();
    /* Z80 driver needs settle time before sound data upload */
    WaitVsync();
    WaitVsync();
    InstallSounds(ui_sounds, 4);
}
