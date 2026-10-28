# Undertow — developer notes

Code in the sketch, content in data. Chapters, dialogue, units and items are
data files compiled onto the FX flash chip; adding a chapter costs no program flash.

MIT licensed (see `LICENSE`): use it, change it, ship it.

## Building it yourself (and checking the release)
1. **Data** (Python 3 with Pillow, plus Mr.Blinky's
   [Arduboy-Python-Utilities](https://github.com/MrBlinky/Arduboy-Python-Utilities) for `fxdata-build.py`):
   `APU=/path/to/Arduboy-Python-Utilities sh tools/build_data.sh`. This regenerates `story.h`, `sfx.h`
   and everything in `fxdata/` from `chapters/`, `tools/chapters/game_data.py` and the art/sound scripts.
2. **Sketch** (Arduino IDE): install the **Arduboy Homemade** board package (Mr.Blinky), pick board
   **Arduboy FX-C** (or **Arduboy FX** for the original's flash pin) with the **Arduboy optimized core**.
   Libraries: Arduboy2 6.0.0, ArduboyFX 1.0.11, ArduboyTones (bundled with the package).
   Add this line to the board package's `platform.local.txt` or it won't fit:
   `compiler.c.elf.extra_flags=-mcall-prologues -mrelax -fno-move-loop-invariants`
3. **Check**: built with avr-gcc 7.3.0 (the package's toolchain), the .hex files and `fxdata-data.bin`
   come out byte-identical to `release/` -- compare against `release/SHA256SUMS.txt`
   (`sha256sum -c SHA256SUMS.txt`). A different compiler version gives a different (but equivalent) hex.

What the game touches on the device: the screen, buttons, speaker and LEDs; the FX flash chip
(read-only: art, text, chapters); and EEPROM bytes 80-~500 for the save and the supply wagon, written
with avr-libc `eeprom_update_*` (only changed bytes are written) at each chapter start and when the
wagon changes. It's built without the USB stack, so it never talks to a computer: to upload a new
.hex, hold UP + DOWN while it runs to enter the bootloader.

## Layout
| Path | What |
|---|---|
| `Undertow.ino` | the engine |
| `chapters/NN_*.txt` | **one file per chapter** (keywords in `chapters/README.txt`) |
| `tools/chapters/game_data.py` | names, classes (and promotions), items, heroes' base stats and growth rates |
| `tools/chapters/make_chapters.py` | compiles the above → `story.h` + `fxdata/chapters.bin` + `fxdata/strings.bin` (validates everything) |
| `tools/woodcut/` | art pipeline → FX images: map tiles, unit sprites, font, figures, backdrops, shields, portraits, cards |
| `tools/audio/make_sfx.py` | sound → `sfx.h` + `.wav` previews |
| `tools/build_data.sh` | rebuild all generated data in order |
| `tools/sim/` | PC campaign harness (plays whole campaigns, checks invariants) + scenario tests (`test_chapters/`) |
| `release/` | prebuilt v0.4: `Undertow-FX.hex` (original FX flash pin), `Undertow-FXC.hex` (FX-C / Mini pin), `Undertow-data.bin` (FX data for a cart image), `Undertow-dev.bin` (padded, for Ardens or `fxdata-upload.py`), `Undertow.arduboy`, and `SHA256SUMS.txt` |
| `tools/emu/` | headless Ardens driver: runs the real .hex, presses buttons, saves screenshots |

## Chapters
| # | Title | Goal | Notes |
|---|---|---|---|
| 1 | Drowned Ford | seize the keep | Osk |
| 2 | The Quiet Mill | defeat Varga | captive Maren (gone after turn 8), two villages |
| 3 | Red Palisade | defeat Kell | the gate wakes the garrison; a gap in the south fence; Kell's ledger in the strongroom chest |
| 3X | The Grey Road | defeat Voss | only with the ledger; the Drowned rise on turn 3 |
| 4 | Border Trouble | defeat the captain | Dain riders return every 4 turns until he falls; Brannock can talk Oren round |
| 5 | The Pass | seize the pass | the gap wakes the ridge archers; riders close the road on turn 5; kill the courier for 5X |
| 5X | The Lodge | defeat Corvane | only with the courier's orders; Corvane talks, and his key matters in Ch 7 |
| 6 | The Dain Army | defeat Gerault | reserves join on turn 4; Hale can talk Cass round; Gerault covers what 5X would have told you |
| 7 | Dainholt | seize the gate | towers hold archers; the gate garrison wakes as you reach it; Corvane's key (flag 9) opens the sally port; free the prisoners in the river-gate cells (flag 3) for 7X |
| 7X | Drowned Vaults | seize the altar | only with the prisoners freed; rows of sleeping Drowned, a bell-rope tripwire; the register (flag 10) names Lir |
| 8 | Dain Castle | seize the throne | King Aldric on the throne; when he falls the Hollow King pours into the well and Drowned climb out |
| 9 | Black Water | survive 10 turns | Drowned climb out of the canals in alternating waves; Aelis (mage) trapped in the chapel until turn 6 |
| 10 | The Hideout | defeat Mother Silt | sea caves; tidal pools are portals between chambers; Silt raises the Drowned every 4 turns; the sea chart (flag 4) opens 10X |
| 10X | The Sea Stack | seize the tower | only with the chart; a climb past bellwraiths; the last ward bell (flag 11) |
| 11 | Saltmarch | seize the hall | the port; a shop; Rook fights from his chained ship until Wren (or Brannock) talks him round; the hall's guards wake at its door |
| 12 | Black Sails | survive 8 turns | two ships and two boarding planks; pirates cross every 3 turns until Black Marl falls; sea horrors climb aboard every 3 turns unless the 10X ward bell is on the mast |
| 13 | Strange Shores | seize the fort | Vael; Drowned in small groups, each with its own tripwire; a hiding islander to TALK to (flag 5, for 13X) |
| 13X | Whisperings | survive 7 turns | only after talking to the islander; hold the rebels' meeting house against the watch, its bowmen and the Drowned; Brigid's trust (flag 12) opens a way in Ch 15; a hare's foot in the watchtower |
| 14 | Cliff Village | survive 8 turns | Drowned climb three cliff paths in waves; Saoirse harries you on her sea-eagle until Rook talks her down; Thane locked in the chapel until turn 6 |
| 15 | The Rebellion | seize the bell | street groups wake as you cross; with flag 12 the fish-market cellar comes up in the square; Vey holds the tower; the winged boots in the counting house |
| 16 | Desert Silence | rout | open salt flats, the Horned in force; Hesk holds his ground until Thane talks him round; a hare's foot in a cairn |
| 17 | The Demon Gate | defeat the Gatekeeper | one ford across the chasm and two portals; the gate spills demons every 3 turns until the Gatekeeper falls (he drops an ox horn); Maren can talk to the still Drowned (flag 6, for 17X) |
| 17X | Secrets Below | seize the stone | only via the still one; Lir joins; galleries that wake hall by hall; the calling stone (flag 13); iron bark in the old chest |
| 18 | The Coast Road | survive 10 turns | the cult's host from west, east and north in waves; a horned sentry in the tollhouse |

## Adding a chapter
1. Copy `chapters/01_drowned_ford.txt` to `chapters/05_whatever.txt` (files load in name order).
2. Edit the map (24x16), hero starts, enemies, objective (`seize X Y`, `rout`, `boss`, `survive N`), scenes.
3. New enemy type? Add a name to `NAMES` in `game_data.py` (and a battle figure if it's a new look).
4. `APU=/path/to/Arduboy-Python-Utilities sh tools/build_data.sh`, then rebuild the sketch.
The compiler stops with a file:line message for anything wrong: unknown names or items, units on
water or walls, two units on one tile, a class carrying no usable weapon, dialogue lines over
17 characters or 5 lines, a `boss` objective with no boss, etc.

## Systems
- **Campaign**: chapters play in order. Heroes carry levels, EXP, stats and items forward; HP refills
  between chapters. Fallen heroes stay fallen (permadeath); losing Brannock ends the chapter.
- **Side chapters**: a chapter with `requires: flag N` is only played if a chest or village set that
  story flag earlier; otherwise the campaign skips it. Story flags are saved with the army.
  Chapter 3X (the Grey Road) needs Kell's ledger from the strongroom chest in chapter 3.
- **Events** (up to 8 per chapter; full syntax in `chapters/README.txt`): *when* a hero walks over a
  tile, a turn starts (once or every K turns), a named enemy falls, or a hero talks to a unit — *do*
  wake or spawn a group, warp the hero (portals), recruit the talked-to enemy, set a flag, win, play a
  scene, switch on the demon motif. `until N` switches an event off once flag N is set; `needs N` /
  `needs !N` only lets it fire while flag N is (isn't) set -- Corvane's key and the barred sally port
  are the same tile. A repeating event (portal, wave) plays its scene only the first time.
  Enemies belong to groups (`reserveG`, `ambushG`, `gG`); a spawn brings fallen members back fresh at
  their posts (reinforcements, demon gates). The Red Palisade's gate and 3X's Drowned are events.
- **Recruiting**: captives (TALK frees them), enemies that turn (`talk HERO to NAME ; join`: the enemy
  joins with the stats it fought with), and `joins: NAME` (joins as a chapter opens).
- **Boosters** (8 in the whole game, never sold): OX HORN ×2 (+2 STR), HAWK FEATHER (+2 SKL), HARE'S
  FOOT ×2 (+2 SPD), IRON BARK (+2 DEF), HEART ROOT (+5 HP), WINGED BOOTS (+1 MOV, the only pair). ITEM ->
  A uses one for good (it takes the turn) and shows the gain on the level-up board. They're in the
  side chapters' hardest chests (7X, 10X, 13X, 17X), on two bosses (Black Marl, the Gatekeeper) and in
  two out-of-the-way chests (Ch 15, 16). The boots are a hero flag (F_BOOTS = F_BOSS on a hero).
- **Crits** triple the damage, but never do less than STR + weapon might: a crit finds the gap in any
  armour, so a Killer Bow matters against a 13-DEF boss. (It works for enemies too.)
- **Promotion**: a HERO CREST used at level 10+ turns a class into its promoted class (game_data
  `CLASSES`: LORD → HIGH LORD, LANCER → HUSSAR, FIGHTER → REAVER, ARCHER → RANGER, KNIGHT → WARDEN,
  CLERIC → BISHOP — placeholder names). Level resets to 1, the class bonuses are added (shown on
  the level-up screen as a class change), and promoted classes wield two weapon kinds, move further,
  and count as 10 levels higher for EXP. Promoted units get a pennant on their map sprite and a
  second rim on their shield.
- **Flags**: 1–16 are story flags saved with the army; 17–24 last one chapter.
- **Conditional lines**: any dialogue line can depend on a flag or on a hero being in the army
  (`[9] GERAULT: ...`, `[!9] ...`, `[CASS] CASS: ...`). Endings and side-chapter fallbacks use them.
- **Saves** (EEPROM): written at the start of every chapter. Title offers CONTINUE / NEW GAME;
  a defeat offers RETRY from that chapter's start.
- **Items**: 4 slots per unit. Common weapons and staves (iron, steel, Fire, the heal staff) never wear
  out. Fine ones -- silver, the Killer Bow, Thunder, the Mend Staff (and legendary ones later) -- have
  uses (20 or so); each swing, hit or miss, costs one, and the last breaks it (BROKE! in the battle;
  its wielder stops striking). Potions and crests are used up. Anything that runs out shows a count.
- **Supply wagon**: 24 more slots, kept in EEPROM (no RAM). In the trade table it's six extra
  "heroes" (WAGON 1..6) after the army. A find goes to the finder, else the wagon, else anyone with
  room. The wagon is saved with the chapter, so a restart puts it back too.
- **Gold and shops**: gold comes from villages (some give a purse instead of an item) and a reward
  for each chapter cleared (paid on ONWARD). Town chapters (`shop:` -- Ch 4, 7, 11) open a SHOP row
  on the prep screen: up to six goods, bought into the wagon. Prices are in game_data.PRICES; silver,
  crests and other finds are never sold. No selling (yet). The first weapon the class can wield is equipped; ITEM -> A on a weapon
  equips it (free), A on a healing item uses it (ends the turn). Javelins and hand axes reach 1-2
  squares. Enemies can carry drops (`*` in the chapter file) that go to the hero who fells them.
- **Game menu** (A on an empty tile): END TURN, SOUND, RESTART (this chapter, from its start) and QUIT
  (to the title). RESTART and QUIT ask "SURE?" first: press A again. Every list wraps: UP from the
  top lands on the bottom.
- **Map overview**: hold B on the map for the whole battlefield at 4 px a square -- heroes as solid
  blocks (hollow once they've acted), enemies flashing (a steady dot while asleep), the screen's view
  as a frame, and the turn and head counts. A quick tap of B still jumps to the next hero.
- **Last words**: when a hero falls, the battle pauses for their portrait and a line (game_data
  `HEROES[...]['death']`), so an enemy-phase crit can't slip by unnoticed.
- **Chapter clear**: the end screen names who fell (FELL: HALE +1) and offers ONWARD or RESTART. The
  chapter's save is only replaced by going ONWARD, so a costly win can be replayed.
- **Deployment**: before every chapter after the first, tick who fights (up to the chapter's
  `deploy:`, at most 8); the lord always goes. Benched heroes keep their levels and items. The list
  scrolls (the roster holds up to 15 heroes).
- **Trade** (ITEMS on the deployment screen): two heroes side by side; left/right switches column.
  The name row: A cycles that column's hero. On an item, A picks it up; A on any slot then puts it
  there, swapping with what's there (an empty slot = a plain give; the same column = reorder).
  THROW AWAY (under the slots) discards the item in hand: A, then A again. B puts it back down,
  then B again returns to the roster.
- **Healing** (staff or vulnerary): two dotted rings squeeze in on the healed unit, then the +HP tag.
- **Battle backdrops** follow the defender's ground: field, forest, hills, river (water/ford),
  stronghold (walls, keep, palisade, gate, chest), village, vaulted hall (floor, portal).
- **Villages** (`V` tiles) and **chests** (`C`): stop a hero on one and pick VISIT / OPEN to hear the
  villagers (or read what's inside) and take the gift. Gifts and battle drops go to the finder, or to the first hero with a free slot.
- **Captives**: a not-yet-joined hero waiting on the map (blinking, in a dotted cage). Any hero
  next to them picks TALK and they join. With `leaves T` they're carted off after turn T's enemy
  phase (warning scene on turn T), and lost for good.
- **Clerics** carry staves: HEAL (10 HP) and MEND (20 HP, 15 uses) restore an adjacent ally's HP and
  earn EXP. The unit menu names the strongest staff carried (HEAL or MEND); the healer then uses the
  weakest staff that covers the whole wound, so Mend's uses go where they count. Staves never attack or
  counter. A staff is never "the weapon in hand": a unit fights with its first weapon and heals with
  its first staff, whatever order they're carried in (so a Sage can do either from one square).
- **Magic**: tomes (FIRE, THUNDER, the cult's MIRE) strike at range 1-2 and count only **half** the
  target's DEF (terrain still counts in full). There's no separate magic stat: a caster's STR is its
  power. Mages (Aelis, the Undertow's cultists) promote to Sage (tomes + staves). In battle a caster
  never dashes in: a bolt flies, and rings spread from the target on a hit, with its own sound.
- **Indoors**: floor tiles (`_`) for halls, vaults and caves, with a vaulted-hall battle backdrop
  (portals use it too).
- Walking, x2 badges, EXP/level-ups, sound, portraits as before.

## FX vs FX-C (important)
The FX and the FX-C wire the flash chip to different pins. A build for the wrong one freezes or
stays black as soon as it touches FX data. In the Arduino IDE pick **Arduboy FX-C** for your
device; Ardens emulates the original FX unless you add `fxport=e2`.

## Budget (Arduino IDE: Arduboy Homemade package, board "Arduboy FX-C", Arduboy optimized core)
Flash 29,398 of 29,696 B (298 free) · RAM 2,321 B static (13 heroes) · FX data 147 KB of 16 MB.
**Build flags**: the release hex is linked with `-mcall-prologues -mrelax -fno-move-loop-invariants`
(about 1.1 KB + 0.7 KB smaller). In the Arduino IDE put
`compiler.c.elf.extra_flags=-mcall-prologues -mrelax -fno-move-loop-invariants` in the board
package's `platform.local.txt`, or the sketch won't fit. The boot-time flashlight / sound combos are left out
(sound is toggled in game; uploads use UP + DOWN while running).
All art, every name (heroes, classes, items, terrain), the UI text, the heroes' starting stats and
growths, and each name's portrait/voice/figure live on the FX chip (`strings.bin`, `tables.bin`);
program flash holds the code, each item's kind and each class's weapons and move (read constantly
by the AI; the rest of those records is on FX) and the sound. Music notes slur into each other (only a
repeated pitch gets a gap), which halved the sound data. If the FX data is missing (or the build is for the wrong
flash pin) the game shows a big X and lights the red LED.
**RAM** is the tight one as the cast grows: every hero is 21 bytes and stays in memory. Measured in
Ardens, the deepest stack (a tone interrupt landing on the enemy AI) is about 190 bytes. With 13
heroes static RAM is 2,321 B and the deepest measured stack leaves ~50 B; at 15 heroes ~2,363 B,
leaving ~10 B to spare -- so the compiler stops at 15 heroes, and
chapters at 10 enemies (reinforcements reuse fallen enemies' slots).
The sketch is built **without the USB stack** (`#define NO_USB_BUILD`): chapter 3's systems needed the
2.7 KB it frees. Ardens doesn't care. On the device, the IDE can't reset it for an upload any more:
hold **UP + DOWN** for a moment while the game runs (the Arduboy core's bootloader combo), then upload.

## Harness
```
cd tools/sim
g++ -std=gnu++17 -O1 -fsanitize=address,undefined -I. -I../.. -x c++ sim.cpp -o sim
./sim 60                       # campaigns: clears, carry-over, retry/save, items, routes,
                               # deployment, trades, villages, captives, heals, alarms,
                               # ledger runs into 3X
CAREFUL=1 ./sim 50             # a player who reloads when a hero dies (difficulty check)
START=10 LV=11 FLAGS=0xffff CAREFUL=1 ./sim 10
                               # one chapter (index 10 = Ch 8) with a levelled army: balance
                               # a late chapter without playing up to it
START=19 LV=16 FLAGS=0x1fff CAREFUL=1 ONLY=1 ./sim 12
                               # ONLY=1: stop once that chapter is cleared (fast per-chapter tuning)
SHOTS=1 ./sim 1 && python3 sheet.py screens.png
python3 make_test_fx.py && TESTS=1 FXDATA=test_fxdata.bin ./sim
                               # scenario tests: portals, tripwires, talk-to-recruit, spawns,
                               # repeats/until/falls, joins, side-chapter branching, promotion,
                               # needs/needs!, magic (range, half DEF, no dash), sages, clerics,
                               # crit floor, smart staves, boosters + boots (111 checks)
```
