#!/usr/bin/env python3
"""read_transcript.py - Extract Zork Neo transcript from Mednafen .flash file
Usage: python3 tools/read_transcript.py [flash_file]
       Saves to flash_file.transcript.txt and prints to stdout
       Then paste transcript here for Claude to analyse.
"""
import sys, os, glob, struct

TRANSCRIPT_MAGIC = 0xBEEF
FLASH_TRANSCRIPT = 0x1FA200   # byte offset - slot 2 (slot 1 = game save)

def find_flash():
    for pat in [
        os.path.expanduser("~/.mednafen/sav/zork*.flash"),
        os.path.expanduser("~/.mednafen/zork*.flash"),
        "bin/zork-neo.ngp.flash",
        os.path.expanduser("~/.mednafen/sav/*.flash"),
    ]:
        hits = glob.glob(pat)
        if hits:
            zork = [h for h in hits if 'zork' in h.lower()]
            return zork[0] if zork else hits[0]
    return None

def extract(path):
    data = open(path, 'rb').read()
    if len(data) <= FLASH_TRANSCRIPT + 256:
        return None, f"Flash file too small ({len(data)} bytes)"
    words = struct.unpack_from('>128H', data, FLASH_TRANSCRIPT)
    if words[0] != TRANSCRIPT_MAGIC:
        return None, f"No transcript magic (got {words[0]:04X}, want {TRANSCRIPT_MAGIC:04X})"
    length = min(words[1], 252)
    chars = []
    for i in range(length):
        word = words[(i >> 1) + 2]
        if (i & 1) == 0:
            chars.append(chr((word >> 8) & 0xFF))
        else:
            chars.append(chr(word & 0xFF))
    return ''.join(chars), None

if __name__ == '__main__':
    path = sys.argv[1] if len(sys.argv) > 1 else find_flash()
    if not path:
        print("Cannot find .flash file. Run mednafen first, then:")
        print("  python3 tools/read_transcript.py ~/.mednafen/sav/zork-neo.ngp.flash")
        sys.exit(1)
    print(f"Flash: {path}")
    text, err = extract(path)
    if err:
        print(f"Error: {err}")
        sys.exit(1)
    print(f"Length: {len(text)} chars\n")
    print("=== ZORK NEO TRANSCRIPT ===")
    print(text)
    print("=== END ===")
    out = path + '.transcript.txt'
    open(out, 'w').write(text)
    print(f"\nSaved: {out}")
