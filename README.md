# Undertow — a woodcut tactics game for the Arduboy FX

![Undertow title screen](screenshots/01_title.png)

A border lord chases bandits across a river, and every trail ends in a neighbouring kingdom's silver.
**Undertow** is a Fire Emblem–style tactics game drawn in 1-bit woodcut, for the Arduboy FX and FX-C.

This is a **demo (v0.1)**: chapters 1–6 plus two hidden side chapters, the first act and a half of a
planned 22-chapter campaign.

| | |
|---|---|
| ![map](screenshots/04_map.png) | ![forecast](screenshots/05_forecast.png) |
| ![battle](screenshots/06_battle.png) | ![dialogue](screenshots/03_dialogue.png) |
| ![deploy](screenshots/07_deploy.png) | ![forest battle](screenshots/09_forest_battle.png) |

## What's in it
- Turn-based tactics on 24×16 maps, with terrain, a weapon triangle (sword > axe > lance > sword), bows at range and double attacks
- Permadeath: fallen heroes stay fallen; lose Brannock and you retry the chapter
- Recruits: rescue a captive before she's carted off, and talk enemy soldiers into joining (only the right hero can)
- Hidden side chapters, unlocked by what you do and find in earlier chapters
- Woodcut battle scenes with backgrounds that follow the terrain
- EXP, level-ups and class promotions (Hero Crests, from level 10)
- Deployment and item trading before each chapter
- Villages, chests, tripwires and ambushes
- Sound effects and music cues; saves at the start of every chapter

## Files (in [`demo/`](demo))

| File | What it's for |
|---|---|
| `Undertow-FX.hex` | the game, for the **original Arduboy FX** (and Ardens with default settings) |
| `Undertow-FXC.hex` | the game, for the **Arduboy FX-C** (and Ardens with `fxport=e2`) |
| `Undertow-data.bin` | the game's FX data, for building a flashcart image (pair it with the matching .hex) |
| `Undertow-dev.bin` | the same data padded for development: load it in Ardens with a .hex, or write it to the end of the flash chip with `fxdata-upload.py` |

**The FX and FX-C wire the flash chip differently: use the .hex that matches your device.** A big X on
screen with the red LED lit means the FX data is missing or the wrong .hex is loaded.

**Try it in your browser:** open the Ardens emulator and drop `Undertow-FX.hex` and `Undertow-dev.bin`
onto it together.

**Uploading the .hex directly:** the game is built without USB support (to fit), so the computer can't
reset it into upload mode. Hold **UP + DOWN** for a moment while the game runs to enter the
bootloader, then upload.

## Controls

| Button | Does |
|---|---|
| D-pad | move the cursor |
| A on your unit | show its move range; A again to walk there, then pick an action |
| A on an enemy | show where it can reach and strike |
| A on an empty tile | END TURN, SOUND, RESTART chapter, QUIT to title |
| B | cancel / undo a move; on the map, jump to your next ready unit |
| hold A | speed up a battle |
| B on a chapter card | sound on/off |

Each unit does one thing per turn: attack, heal, talk, visit, use an item, or wait.
Weapons never break; you find better ones as you go.

## Feedback
This is a demo: tell me what's too hard, too easy, confusing, or broken (chapters 4–6 are the newest).
