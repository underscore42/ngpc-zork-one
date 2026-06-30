# Zork Neo

**Zork I: The Great Underground Empire** for the Neo Geo Pocket Color.

A port of the classic 1981 Infocom text adventure, adapted for the NGPC's
20×19 character display with a button-driven verb/noun parser — no keyboard required.

![Zork Neo title screen](screenshots/title.png)

## Controls

| Button | Action |
|--------|--------|
| Left/Right | Cycle verb (when verb row active) or noun (when noun row active) |
| A | Confirm verb → move to noun selection; confirm noun → execute command |
| B | Cancel noun selection, return to verb |
| Up/Down | Scroll text output history |
| Option | Quick inventory |

The **amber** row is the active selector. Press A to confirm your verb, then
the row turns **green** for noun selection.

## Gameplay

You are an adventurer exploring the Great Underground Empire. Collect
treasures and deposit them in the trophy case in the Living Room to score
points. Watch your lamp — it has limited battery life.

Start at West of House. Open the mailbox. Read the leaflet. Find your way inside.

## Build

Requires the NGPC devkit (cc900/tulink/tuconv/s242ngp) and the ameliandev
ngpc-project-template framework.

```bash
./bootstrap.sh          # copy common files from devkit
make                    # build
make run                # run in Mednafen
```

## Credits

Game content derived from **Zork I: The Great Underground Empire**
© 1981, 1982, 1983 Infocom, Inc.
Released as open source under the MIT License by Microsoft/Activision, November 2025.

NGPC port by **underscore42** / Studio So Not Kansai, 2025.

Engine, parser, text system, and UI written from scratch for the TLCS-900H.

## License

The NGPC port code (engine, parser, text system, UI) is released under the MIT License.
See [LICENSE](LICENSE) for details.

Zork I game content is used under the MIT License granted by Microsoft/Activision (November 2025).
See [LICENSE-ZORK](LICENSE-ZORK) for the original Zork license.
