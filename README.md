# GALACTA GAME
## A Classic Arcade-Style Shooter Built with C and SDL2

**Developed by:** Himanshu Ganapavarapu  
**College Project** | MIS ID: 111508029

## Overview
GALACTA GAME is a fast-paced, retro-style space shooter developed using C and SDL2, designed to adapt dynamically to the system's resolution. The game brings back the thrill of classic arcade shooters while incorporating structured gameplay mechanics and efficient data structures.

## Gameplay Mechanics
- **Ship Controls:** Navigate left and right at the bottom of the screen, firing bullets with the spacebar.
- **Bullets:** Dynamically allocated and deallocated to optimize performance, ensuring smooth gameplay.
- **Enemies:** Each enemy starts with 100 health, decreasing by 25 with each hit. If an enemy escapes, the player loses points and the enemy respawns at a random location.
- **Scoring System:** Destroying an enemy grants +25 points, while missing an enemy deducts -10 points. Colliding with an enemy results in a -10 life penalty.
- **Difficulty Levels:** Choose from Easy (4 enemies), Medium (5 enemies), or Hard (6 enemies).

## Adaptive Difficulty
As the player's score increases, enemy speed dynamically scales, making the game progressively more challenging.

## Modular and Extensible
The game is structured with modular functions for core mechanics, making it open to improvements and contributions from the open-source community.

## Technical Stack
- SDL2 (for rendering and handling game logic)
- SDL2 Mixer (for sound effects and background music)
- SDL2 TrueType Font (for in-game fonts and UI)

## Running on macOS

1. Install the required dependencies using Homebrew:

```
brew install sdl2 sdl2_mixer sdl2_ttf sdl2_image
```

2. Create the binary
```
    make game
```

3. Execute the game
```
./galacta
```
