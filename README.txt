Product Name : GALACTA GAME

Name : Himanshu Ganapavarapu
MIS ID : 111508029

Description:

Game developed using SDL 2. Each Screen is rendered in such a way to fit the current systems resolution.

Three Data Structures have been used in this game. One for the bullets, one for the Enemies and one for 
the player's Ship.

//About the Ship
It is a structure created with an SDL_rect and the s and y coordinates.
It is positioned at the bottom of the screen and can move in two directions. Left and Right.
When Space is pressed, a bullet is malloced and is shot towards the top of the screen.

//About the bulllet
Each time the bullet is called, its memmory is malloced and then it is displayed on the screen. 
Bullets is a globally defined array. Whenever a bullet leaves the screen, its memory is freed.

//About the Enemy
Each Enemy has a life of 100. And each bullet decreases the life by 25 points. 
When the enemie's life hits 0, the image is removed by freeing the malloced space for the enemy.

//About the score points and the life points
Everytime an enemy is destroyed the score of the player increases by 25 points.
And if the player misses the enemy and the enemy goes off the screen, the player point is decreased by 10
points and the enemy is again malloced to a random 'x' location from the top of the screen.
If the player collides with an enemy, the player life is decreased by 10 points, which was originally set 
to 100 points.

//About the difficutly levels
The game has 3 set difficulty levels. Easy, Medium and Hard.
Each difficulty level has different number of enemies spawning throughout the game.
Easy has 4 enemies, Medium has 5 and Hard has 6.

//About the Speed of the enemies
The speed of the enemies is a variable that changes as one progresses through the game.
As the score of the player increases, the speed increases proportionately.

This game has been made as modular as possible, creating various functions for various tasks
and any open source contribution is appretiated.

Libraries Required : 

*SDL 2 Library : Linked in the terminal with -lSDL2
*SDL 2 Mixer Library : Library used for the sound effects and the theme music, linked with -lSDL2_mixer
*SDL 2 True Type Font Library : Library used for the fonts in the game, linked with -lSDL2_ttf


