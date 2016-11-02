//Using Standard I/O Library
#include <stdio.h>
#include<time.h>
//SDL 2 Libaries
#include<SDL2/SDL.h>
#include <SDL2/SDL_mixer.h>
#include<SDL/SDL_image.h>
#include"SDL/SDL_ttf.h"
//Library used to create and assign bool values and variables
#include <stdbool.h>
//Libary that contains the atoi() function
#include <stdlib.h>
//library that contains the usleep()
#include <unistd.h>

/*******************************************************************************
    Copyright (C) 2015 Himanshu Ganapavarapu

    This program is free software: you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation, either version 3 of the License, or
    (at your option) any later version.

    This program is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License
    along with this program.  If not, see <http://www.gnu.org/licenses/>.
    *************************************************************************/

#define MAX_BULLETS 1000


//Screen dimension definitions
#define SCREEN_WIDTH 640
#define SCREEN_HEIGHT 480

/***********Global Variables***********/
//Score of the Player
int player_score = 0 , width, flag = 0, score_flag = 0, MAX_ENEMIES;
char score[10], life[5], text[20];
//The window we'll be rendering to
SDL_Window* gWindow = NULL;
//The render we will be using for all the objects
SDL_Renderer *gRenderer;
//The surface contained by the window
SDL_Surface* ScreenSurface = NULL;
//Textue used to display text
SDL_Texture* Message;
SDL_Texture *shipTexture = NULL;
SDL_Texture *bulletTexture = NULL;
SDL_Texture *enemyTexture = NULL;
SDL_Texture *backgroundTexture = NULL;
SDL_Texture *logoTexture = NULL;

//The music that will be played
Mix_Music *gMusic = NULL;
Mix_Music *gOver = NULL;
//Sound Effects for Bullet
Mix_Chunk *gShoot = NULL;
//Sound Effect for Enemy Destruction
Mix_Chunk *gEnemyDestroy = NULL;
//Sound Effect for Ship Destruction
Mix_Chunk *gShipExplode = NULL;
//Sound Effect for Bullet Hitting Enemy
Mix_Chunk *bCollision = NULL;
//Fonts
TTF_Font *font;


/*********Function Prototypes**********/
void Quit();
void mediaInit();
void doRender();
void checkCollisions();
void addBullet(float , float , float );
void removeBullet(int);
void addEnemy(int, int, int);
void removeEnemy(int);
bool collision(int , int );
bool collisionShip(int);
bool ttfDisplay(TTF_Font* f, char *str, int x, int y, int w, int h, int r, int g, int b);
int menu();
int diffDisplay();
void displayScore();

/****Structures involved in the game****/
typedef struct Ship{

	SDL_Rect destination;
	int velocityX;
	int velocityY;
	int life;

}Ship;
Ship ship;

typedef struct enemy {

	float x, y;
	int life;

}Enemy;
Enemy *enemies[6] = {NULL};

typedef struct {

	int x, y;
	int dy;

} Bullet;

Bullet *bullets[MAX_BULLETS] = { NULL };

void checkCollisions() {

						
	//Checking collision of the bullet and the Enemy
	for(int i = 0; i < MAX_BULLETS ; i++ ) if(bullets[i]) {					
		for(int j = 0; j < MAX_ENEMIES ; j++) if(enemies[j]) {

			if (collision(i, j) == true) {

				Mix_PlayChannel( -1,bCollision , 0 );
				enemies[j]->life -= 25;
				removeBullet(i);

				if(enemies[j]->life == 0) {
					
					removeEnemy(j);
					Mix_PlayChannel( -1, gEnemyDestroy, 0 );
					player_score += 25;
					score_flag = 1;
				}

				break;
			}

		}

	}

	//Checking collision of the Ship and the Enemy
	for(int i = 0; i < MAX_ENEMIES ; i++) if(enemies[i]) {

		if(collisionShip(i) == true) {

			Mix_PlayChannel( -1, bCollision, 0 );
			removeEnemy(i);
			ship.life -= 10;

		}

	}

}

void displayScore() {
		
	SDL_RenderClear(gRenderer);
	//Rendering the Background on the Screen
	SDL_RenderCopy(gRenderer, backgroundTexture, NULL, NULL);
	ttfDisplay( font , "Your Score", 100, (SCREEN_HEIGHT / 2) - 30, 400, 30, 255, 255, 255);
	ttfDisplay(font, score ,250, (SCREEN_HEIGHT / 2) + 30, 100, 30, 255, 255, 255);
	//Loadinf the Theme Music
	if( Mix_PlayMusic( gOver, -1 ) == -1 ) {
		printf("Theme Music Could not be loaded!");
	}
	SDL_RenderPresent(gRenderer);
	SDL_Delay(5000);

}

void mediaInit() {

	//Initialize SDL_mixer
	if( Mix_OpenAudio( 44100, MIX_DEFAULT_FORMAT, 2, 2048 ) < 0 ) {

		printf( "SDL_mixer could not initialize! SDL_mixer Error: %s\n", Mix_GetError() );
	}

	//Clearing the screen to black			
	SDL_SetRenderDrawColor(gRenderer, 0, 0, 0, 255);//Draw In Black
	SDL_RenderClear(gRenderer);//Clear the Whole window
	SDL_RenderPresent(gRenderer);//Put cleared window on screen
			
	//Making it think it's a 640*480 window, but rendering to system resolution without delay 
	SDL_SetHint(SDL_HINT_RENDER_SCALE_QUALITY, "linear");  // make the scaled rendering look smoother.
	SDL_RenderSetLogicalSize(gRenderer, 640, 480);

	//Load music
	gMusic = Mix_LoadMUS( "Resources/SoundR/beat.mp3" );

	if( gMusic == NULL ) {

		printf( "Failed to load beat music! SDL_mixer Error: %s\n", Mix_GetError() );
	}
	gOver = Mix_LoadMUS( "Resources/SoundR/GameOver.wav" );

	if( gOver == NULL ) {

		printf( "Failed to load beat music! SDL_mixer Error: %s\n", Mix_GetError() );
	}
	
	//Loading the SpaceShip
	SDL_Surface* objSurface = SDL_LoadBMP("Resources/ship.bmp");
	SDL_SetColorKey( objSurface, SDL_TRUE, SDL_MapRGB(objSurface->format, 255, 255, 255) );
	shipTexture = SDL_CreateTextureFromSurface(gRenderer, objSurface);
	SDL_FreeSurface(objSurface);
		
	if( shipTexture == NULL ) {

		printf( "Error_Resources : Cannot Find Ship!\n");
	}

	//Load Sound Effect for ShipExplode
	gShipExplode = Mix_LoadWAV( "Resources/SoundR/ShipExplode.wav" );

	if( gShipExplode == NULL ) {

		printf( "Failed to load high sound effect! SDL_mixer Error: %s\n", Mix_GetError() );
	}

	//load the bullet  
 	SDL_Surface *bullet = SDL_LoadBMP("Resources/bullet.bmp");
	bulletTexture = SDL_CreateTextureFromSurface(gRenderer, bullet);
 	SDL_FreeSurface(bullet);

  	if(!bullet) {

   		printf("Error_Resources : Cannot find bullet\n");
 	}

	//Load Sound Effect for Bullet
	gShoot = Mix_LoadWAV( "Resources/SoundR/shoot.wav" );

	if( gShoot == NULL ) {

		printf( "Failed to load high sound effect! SDL_mixer Error: %s\n", Mix_GetError() );
	}

	//Load the enemy
	SDL_Surface *enemy = SDL_LoadBMP("Resources/enemy.bmp");
	enemyTexture = SDL_CreateTextureFromSurface(gRenderer, enemy);
	SDL_FreeSurface(enemy);
			
	if(!enemy) {

		printf("Error_Resources : Enemy not Found\n");
	}

	//Load Sound Effect for EnemyDestroy
	gEnemyDestroy = Mix_LoadWAV( "Resources/SoundR/EnemyDestroy.wav" );

	if( gEnemyDestroy == NULL ) {

		printf( "Failed to load high sound effect! SDL_mixer Error: %s\n", Mix_GetError() );
	}

	//load the bg  
  	SDL_Surface *bg = SDL_LoadBMP("Resources/bg.bmp");
  	backgroundTexture = SDL_CreateTextureFromSurface(gRenderer, bg);
  	SDL_FreeSurface(bg);

	if(!bg) {

    		printf("Cannot find background\n");
  	}

	//Other Sound Effects
	bCollision = Mix_LoadWAV( "Resources/SoundR/bCollision.wav" );

	if( bCollision == NULL ) {

		printf( "Failed to load high sound effect! SDL_mixer Error: %s\n", Mix_GetError() );
	}
			
	font = TTF_OpenFont("Resources/fonts/times.ttf", 80);

	if(!font) {

		printf("Font Style not found!\n");
	}
	
	//Loadinf the Theme Music
	if( Mix_PlayMusic( gMusic, -1 ) == -1 ) {
		printf("Theme Music Could not be loaded!");
	}


}

void doRender() {

	SDL_RenderClear(gRenderer);

	//Rendering the Background on the Screen
	SDL_RenderCopy(gRenderer, backgroundTexture, NULL, NULL);

	//Rendering the Score on the Screen
	sprintf(score,"%d", player_score);

	if(player_score < 100)
		width = 32;
	else if(player_score >= 100 && player_score < 1000)
		width = 50;
	else if(player_score >= 1000)
		width = 67;
	ttfDisplay(font, score , 0, 10, width, 55, 255, 255, 255);

	//Rendering life points on the screen
	sprintf(life,"%d", ship.life);
	ttfDisplay(font, life, 0, 75, 50, 55, 255, 0, 0);
				
	//Rendering the Ship on the Screen
	SDL_RenderCopy(gRenderer, shipTexture, NULL, &ship.destination);

	//Rendering The Enemies on the Screen
	for(int i = 0; i < MAX_ENEMIES; i++) if(enemies[i]) {

    		SDL_Rect enemy = { enemies[i]->x, enemies[i]->y, 50, 60 };  
    		SDL_RenderCopy(gRenderer, enemyTexture, NULL, &enemy);
  	}			

	//Rendering the bullets on the Screen
    	for(int i = 0; i < MAX_BULLETS; i++) if(bullets[i]) {

    		SDL_Rect rect = { bullets[i]->x, bullets[i]->y, 8, 8 };  
    		SDL_RenderCopy(gRenderer, bulletTexture, NULL, &rect);
				
  	}

	SDL_RenderPresent(gRenderer);

}

int diffDisplay() {

	int x, y;

	const int MAX_OPTIONS = 3 ;

	char* options[MAX_OPTIONS] ;
	options[0] = "Easy" ;
	options[1] = "Medium" ;
	options[2] = "Hard" ;

	int level[MAX_OPTIONS];
	level[0] = 4;
	level[1] = 5;
	level[2] = 6;
	
	//The Destinations of the individual Options
	SDL_Rect optionsDest[MAX_OPTIONS];
	optionsDest[0].x= (SCREEN_HEIGHT/2) + 30;
	optionsDest[0].y = 200;
	optionsDest[0].w = 70;
	optionsDest[0].h = 50;

	optionsDest[1].x = (SCREEN_HEIGHT/2) + 15 ;
	optionsDest[1].y = 250;
	optionsDest[1].w = 100;
	optionsDest[1].h = 50;

	optionsDest[2].x = (SCREEN_HEIGHT/2) + 30;
	optionsDest[2].y = 300;
	optionsDest[2].w = 70;
	optionsDest[2].h = 50;
	
	//Surface and Texture that we will be using for rendering this screen
	SDL_Surface* levelSurface[MAX_OPTIONS];
	SDL_Texture* levelTexture[MAX_OPTIONS];

	bool selected[MAX_OPTIONS];
	selected[0] = false;
	selected[1] = false;
	selected[2] = false;  
	
	//The two different colors of the Difficulty Level Menu 
	SDL_Color white = { 255, 255, 255, 0 }; 
	SDL_Color yellow = { 255, 255, 0, 0 }; 
 	
	//Font that will be used
	TTF_Font *font;
	font = TTF_OpenFont("Resources/fonts/Oswald-Medium.ttf", 80);
	if( font == NULL ){ 
		printf("Failed to load font for Level Menu. SDL_TTF Error : %s\n", TTF_GetError());
	}
	
	//Loading the background image
	SDL_Surface* bgSurface;
	SDL_Texture* bgTexture;

	bgSurface = SDL_LoadBMP("Resources/bg.bmp");
  	bgTexture = SDL_CreateTextureFromSurface(gRenderer, bgSurface);
	SDL_FreeSurface(bgSurface);

	if(bgSurface == NULL)
		printf("Cannot load background image for Level Menu. Error: %s\n", SDL_GetError());
	if(bgTexture == NULL)
		printf("Can't Load Texture for Menu BG. Error: %s\n", SDL_GetError());

	//The Easy Option
	levelSurface[0] = TTF_RenderText_Blended(font, options[0], white);
	levelTexture[0] = SDL_CreateTextureFromSurface(gRenderer, levelSurface[0]);
	if( levelSurface [0] == NULL){
		printf("Easy Option Can't be Loaded. Error in Rendering Text : %s\n", TTF_GetError());
	}
	
	//The Medium Option
	levelSurface[1] = TTF_RenderText_Blended(font, options[1], white);
	levelTexture[1] = SDL_CreateTextureFromSurface(gRenderer, levelSurface[1]);

	if( levelSurface [1] == NULL){
		printf("Medium Option Can't be Loaded. Error in Rendering Text : %s\n", TTF_GetError());
	}
	
	//The Hard Option
	levelSurface[2] = TTF_RenderText_Blended(font, options[2], white);
	levelTexture[2] = SDL_CreateTextureFromSurface(gRenderer, levelSurface[2]);
	if( levelSurface [2] == NULL){
		printf("Hard Option Can't be Loaded. Error in Rendering Text : %s\n", TTF_GetError());
	}
	
	//Rendering the items on the screen
	SDL_SetRenderDrawColor(gRenderer, 0, 0, 0, 255);
	SDL_RenderClear(gRenderer); 
	SDL_RenderPresent(gRenderer); 
		
	SDL_SetHint(SDL_HINT_RENDER_SCALE_QUALITY, "linear"); 
	SDL_RenderSetLogicalSize(gRenderer, 640, 480);

	SDL_RenderClear(gRenderer);
	SDL_RenderCopy(gRenderer, bgTexture, NULL, NULL);
	SDL_RenderCopy(gRenderer, levelTexture[0], NULL, &optionsDest[0]);
	SDL_RenderCopy(gRenderer, levelTexture[1], NULL, &optionsDest[1]);
	SDL_RenderCopy(gRenderer, levelTexture[2], NULL, &optionsDest[2]);
				
	SDL_RenderPresent(gRenderer);

	//Screen loop
	SDL_Event event;
	while(1){

		
		while(SDL_PollEvent(&event)){
			switch(event.type){

				case SDL_QUIT:
					return 1;
					break;

				case SDL_MOUSEMOTION:
					x = event.motion.x;
					y = event.motion.y;
					for(int  i = 0; i < MAX_OPTIONS; i++){
						if(x >= optionsDest[i].x && x <= optionsDest[i].x + optionsDest[i].w && y >= optionsDest[i].y && y <= optionsDest[i].y + optionsDest[i].h){
							if(!selected[i]){
								selected[i] = 1;
								levelSurface[i] = TTF_RenderText_Solid(font, options[i], yellow);
								levelTexture[i] = SDL_CreateTextureFromSurface(gRenderer, levelSurface[i]);
								SDL_FreeSurface(levelSurface[i]);
							}
						}
						else{
							if(selected[i]){
								selected[i] = 0;
								levelSurface[i] = TTF_RenderText_Solid(font, options[i], white);
								levelTexture[i] = SDL_CreateTextureFromSurface(gRenderer, levelSurface[i]);
								SDL_FreeSurface(levelSurface[i]);
							}
 						}
					}
					break;

				case SDL_MOUSEBUTTONDOWN:
					x = event.button.x;
					y = event.button.y;
					for(int i = 0; i < MAX_OPTIONS; i++){
   						if(x >= optionsDest[i].x && x <= optionsDest[i].x + optionsDest[i].w && y >= optionsDest[i].y && y <= optionsDest[i].y + optionsDest[i].h){
							return level[i];
						}
					}					
					break;

				case SDL_KEYDOWN:
					if(event.key.keysym.sym == SDLK_ESCAPE){
						return 1;
					}
					break;

			}

		}
	
		//Rendering the items on the screen
		SDL_RenderClear(gRenderer);	
		SDL_RenderCopy(gRenderer, bgTexture, NULL, NULL);
		SDL_RenderCopy(gRenderer, levelTexture[0], NULL, &optionsDest[0]);
		SDL_RenderCopy(gRenderer, levelTexture[1], NULL, &optionsDest[1]);
		SDL_RenderCopy(gRenderer, levelTexture[2], NULL, &optionsDest[2]);
		SDL_RenderPresent(gRenderer);
	}

	return 0;


	
	


}


int menu() {

	int x, y;
	const int MAX_OPTIONS = 2;
	char* menuOptions[MAX_OPTIONS] ;
	menuOptions[0] = "Play" ;
	menuOptions[1] = "Quit" ;
	
	//Destinations of the items on the screen
	SDL_Rect menuDest[MAX_OPTIONS];
	menuDest[0].x= (SCREEN_HEIGHT/2) + 30;
	menuDest[0].y = 200;
	menuDest[0].w = 70;
	menuDest[0].h = 50;

	menuDest[1].x = (SCREEN_HEIGHT/2) + 30;
	menuDest[1].y = 250;
	menuDest[1].w = 70;
	menuDest[1].h = 50;
	
	//Texture and Surface used in this screen
	SDL_Surface* menuSurface[MAX_OPTIONS];
	SDL_Texture* menuTexture[MAX_OPTIONS];

	bool selected[MAX_OPTIONS];
	selected[0] = false;
	selected[1] = false;  
	
	//The two different colors used for the text
	SDL_Color white = { 255, 255, 255, 0 }; 
	SDL_Color yellow = { 255, 255, 0, 0 }; 
 	
	//Font used for the text
	TTF_Font *font;
	font = TTF_OpenFont("Resources/fonts/Oswald-Medium.ttf", 200);
	if( font == NULL ){ 
		printf("Failed to load font for Menu. SDL_TTF Error : %s\n", TTF_GetError());
	}
	
	//Load the background Image
	SDL_Surface* bgSurface;
	SDL_Texture* bgTexture;

	bgSurface = SDL_LoadBMP("Resources/bg.bmp");
  	bgTexture = SDL_CreateTextureFromSurface(gRenderer, bgSurface);
	SDL_FreeSurface(bgSurface);

	if(bgSurface == NULL)
		printf("Cannot load background image for Menu. Error: %s\n", SDL_GetError());
	if(bgTexture == NULL)
		printf("Can't Load Texture for Menu BG. Error: %s\n", SDL_GetError());


	//The Play Option
	menuSurface[0] = TTF_RenderText_Blended(font, menuOptions[0], white);
	menuTexture[0] = SDL_CreateTextureFromSurface(gRenderer, menuSurface[0]);

	if(menuTexture[0] == NULL){
		printf("Couldn't Create Texture from Menu Surface, Play Option. ERROR : %s\n", SDL_GetError());
	}
	if( menuSurface [0] == NULL){
		printf("Play Option Can't be Loaded. Error in Rendering Text : %s\n", TTF_GetError());
	}
	
	//The Quit Option
	menuSurface[1] = TTF_RenderText_Blended(font, menuOptions[1], white);
	menuTexture[1] = SDL_CreateTextureFromSurface(gRenderer, menuSurface[1]);

	if(menuTexture[1] == NULL){
		printf("Couldn't Create Texture from Menu Surface for Option Quit. ERROR : %s\n", SDL_GetError());
	}
	if( menuSurface [1] == NULL){
		printf("Quit Option Can't be Loaded. Error in Rendering Text : %s\n", TTF_GetError());
	}
	
	//Rendering items onto the screen
	SDL_SetRenderDrawColor(gRenderer, 0, 0, 0, 255);
	SDL_RenderClear(gRenderer); 
	SDL_RenderPresent(gRenderer); 
		
	SDL_SetHint(SDL_HINT_RENDER_SCALE_QUALITY, "linear"); 
	SDL_RenderSetLogicalSize(gRenderer, 640, 480);

	SDL_RenderClear(gRenderer);
	SDL_RenderCopy(gRenderer, bgTexture, NULL, NULL); 
    	ttfDisplay( font, "Galacta", ((SCREEN_WIDTH/2) - 170),  20, 300, 150, 225, 225, 0);
	SDL_RenderCopy(gRenderer, menuTexture[0], NULL, &menuDest[0]);

	SDL_RenderPresent(gRenderer);
	
	//Menu Loop
	SDL_Event event;
	while(1){

		
		while(SDL_PollEvent(&event)){
			switch(event.type){

				case SDL_QUIT:
					return 1;
					break;

				case SDL_MOUSEMOTION:
					x = event.motion.x;
					y = event.motion.y;
					for(int  i = 0; i < MAX_OPTIONS; i++){
						if(x >= menuDest[i].x && x <= menuDest[i].x + menuDest[i].w && y >= menuDest[i].y && y <= menuDest[i].y + menuDest[i].h){
							if(!selected[i]){
								selected[i] = 1;
								menuSurface[i] = TTF_RenderText_Solid(font, menuOptions[i], yellow);
								menuTexture[i] = SDL_CreateTextureFromSurface(gRenderer, menuSurface[i] );
								SDL_FreeSurface(menuSurface[i]);
							}
						}
						else{
							if(selected[i]){
								selected[i] = 0;
								menuSurface[i] = TTF_RenderText_Solid(font, menuOptions[i], white);
								menuTexture[i] = SDL_CreateTextureFromSurface(gRenderer, menuSurface[i] );
								SDL_FreeSurface(menuSurface[i]);
							}
 						}
					}
					break;

				case SDL_MOUSEBUTTONDOWN:
					x = event.button.x;
					y = event.button.y;
					for(int i = 0; i < MAX_OPTIONS; i++){
   						if(x >= menuDest[i].x && x <= menuDest[i].x + menuDest[i].w && y >= menuDest[i].y && y <= menuDest[i].y + menuDest[i].h){
							return i;
						}
					}					
					break;

				case SDL_KEYDOWN:
					if(event.key.keysym.sym == SDLK_ESCAPE){
						return 1;
					}
					break;

			}
	
		}

		//Rendering the items onto the screen
		SDL_RenderClear(gRenderer);	
		SDL_RenderCopy(gRenderer, bgTexture, NULL, NULL);
		ttfDisplay( font, "Galacta", ((SCREEN_WIDTH/2) - 170),  20, 300, 150, 225, 225, 0);
		SDL_RenderCopy(gRenderer, menuTexture[0], NULL, &menuDest[0]);
		SDL_RenderCopy(gRenderer, menuTexture[1], NULL, &menuDest[1]);
		SDL_RenderPresent(gRenderer);
	}

	return -1;


}

void Quit() {

	//Loop to Free the Malloced Memory of the Bullets
	for(int i = 0; i < MAX_BULLETS; i++)
    		removeBullet(i);
	
	//Loop to Free the Malloced Memory of the Bullets
	for(int i = 0; i < MAX_ENEMIES; i++)
    		removeEnemy(i);

	//Free the Sound Effects
	Mix_FreeChunk( gShoot );
	gShoot = NULL;

	Mix_FreeChunk( gShipExplode );
	gShipExplode = NULL;

	Mix_FreeChunk( gEnemyDestroy );
	gEnemyDestroy = NULL;

	Mix_FreeChunk( bCollision );
	bCollision = NULL;
	
	//Close the font variable
	TTF_CloseFont(font);

	SDL_FreeSurface(ScreenSurface);
	
	TTF_Quit();		
	SDL_Quit();

}


//Function to Display Text on the Screen
bool ttfDisplay(TTF_Font* f, char *str, int x, int y, int w, int h, int r, int g, int b) {

 	SDL_Color textcolor = {r, g, b};
	SDL_Surface* surfaceMessage = TTF_RenderText_Solid( f , str , textcolor);
	Message = SDL_CreateTextureFromSurface(gRenderer, surfaceMessage); 
	SDL_FreeSurface(surfaceMessage);
        SDL_Rect textlocation = {x, y, w, h};
	SDL_RenderCopy(gRenderer, Message, NULL, &textlocation);


	return true;
}

//Function that checks if the Players Ship has collided with the incoming Enemie's Ship
bool collisionShip(int i) {
	
	int enemy_x = enemies[i]->x;
        int enemy_y = enemies[i]->y; 
        int enemy_w = 50;
        int enemy_h = 60;
	
	if ( (ship.destination.y >= enemy_y) && (ship.destination.y <= enemy_y + enemy_h) ) {

		if ( (ship.destination.x <= enemy_x + enemy_w) && (ship.destination.x + ship.destination.w >= enemy_x) ) {

                	return true;
		}
        }

	return false;
}

//Function that checks of the bullet has hit an enemy
bool collision(int i, int j) {

	int bullet_x = bullets[i]->x;
        int bullet_y = bullets[i]->y;
        int bullet_s = bullets[i]->dy;
	int bullet_w = 8;

        int enemy_x = enemies[j]->x;
        int enemy_y = enemies[j]->y; 
        int enemy_w = 50;
        int enemy_h = 60;

	if ( (bullet_s < 0) && (bullet_y >= enemy_y) && (bullet_y <= enemy_y + enemy_h) ) {

		if ( (bullet_x <= enemy_x + enemy_w) && (bullet_x + bullet_w >= enemy_x) ) {

                	return true;
		}
        }

	return false;
}


//Function malloc's the size of an enemy when the game first starts and when an enemy dies
void addEnemy (int x, int y, int life) {

	int found = -1;

	for(int i = 0; i < MAX_ENEMIES; i++) {

    		if(enemies[i] == NULL) {

     		 	found = i;
     		 	break;

    		}
  	}
    
  	if(found >= 0) {

   		enemies[found] = malloc(sizeof(Enemy));
   		enemies[found]->x = x;
    		enemies[found]->y = y;
    		enemies[found]->life = life;

  	}
	
}


//Function that frees the malloced space of an enemy once enemies[i]->life hits 0
void removeEnemy(int i) {

	if(enemies[i]) {

    		free(enemies[i]);
    		enemies[i] = NULL;

  	}
}

//Function mallocs the size of a bullet and this is callled whenever space is pressed
void addBullet(float x, float y, float dy) {

	int found = -1;

  	for(int i = 0; i < MAX_BULLETS; i++) {

    		if(bullets[i] == NULL) {

      			found = i;
      			break;

    		}
  	}
    
  	if(found >= 0) {

    		bullets[found] = malloc(sizeof(Bullet));
    		bullets[found]->x = x;
    		bullets[found]->y = y;
    		bullets[found]->dy = dy;
    
  	}

}

//Function that frees the malloced space of a bullet
void removeBullet(int i) {

	if(bullets[i]) {

  		free(bullets[i]);
    		bullets[i] = NULL;
  	}
}

int main( int argc, char* args[] ) {

	srand(time(NULL));

	//The SpaceShip's initial position
	ship.destination.x = 290;
   	ship.destination.y = 430;
   	ship.destination.w = 70;
   	ship.destination.h = 60;
	ship.life = 100;

	
	//Initialize TTF
	if( TTF_Init() < 0){
		printf( "Error Initialising Text:\n ");
	}

	//Initialize SDL
	if( SDL_Init( SDL_INIT_VIDEO |  SDL_INIT_AUDIO  ) < 0 ) {
		printf( "SDL could not initialize! SDL_Error: %s\n", SDL_GetError() );
	}

	else {

		//Create window
		SDL_Window *gWindow = SDL_CreateWindow("Galacta",
                             SDL_WINDOWPOS_UNDEFINED,
                             SDL_WINDOWPOS_UNDEFINED,
                             640, 480,
                             SDL_WINDOW_FULLSCREEN_DESKTOP);

		gRenderer = SDL_CreateRenderer(gWindow, -1, SDL_RENDERER_ACCELERATED);
		SDL_RenderSetLogicalSize(gRenderer, 320, 240);		

		if( gWindow == NULL ) {
			printf( "Window could not be created! SDL_Error: %s\n", SDL_GetError() );
		}
		else {
			int i = menu();

			if(i != 1) {
				
				MAX_ENEMIES = diffDisplay();

				mediaInit();

				bool quit = false;
				float  speed = 0;
	
				while(!quit){

					SDL_Event event;

					while(SDL_PollEvent(&event)) {
						if(event.type == SDL_QUIT){
							quit = true;
							break;
						}
						SDL_Keycode code = event.key.keysym.sym;
						
						switch(code) {
							case SDLK_ESCAPE:
								quit = true;
								break;
							case SDLK_LEFT:
								if(!(ship.destination.x < 0 + 2))
									ship.destination.x -= 17;
								break;
							case SDLK_RIGHT :
								if(!((ship.destination.x + ship.destination.w) >= SCREEN_WIDTH - 2))
									ship.destination.x += 17;
								break;
							case SDLK_SPACE : 
								Mix_PlayChannel( -1, gShoot, 0 );
								addBullet((ship.destination.x + 28 ), (ship.destination.y - 15), -3);
								break;
							default : 
								break; 
	
						}
					}

									
					//To check if the player has won the game
					if(player_score >= 10000) {
	
						player_score = 10000;
						break;
					}
	
					//To check if the Ship has no life points left
					if(ship.life == 0) {
						
						usleep(10000);
						Mix_PlayChannel( -1, gShipExplode, 0 );
						break;
					}
					
					//This is not working
					for(int i = 0; (i < MAX_ENEMIES) ; i++) {
						if(flag == 0) {
							addEnemy((i*85) ,-20,100);
							flag++;
						}
						else {
							addEnemy((rand() % 551) + 50 ,-20,100);
						}
					}

					
					if((player_score % 5 == 0) && (player_score > 0) && (score_flag == 1)) {
						speed = speed + 0.02;
						score_flag = 0;
					}
	
					//Enemies going down
					for(int i = 0; i < MAX_ENEMIES ; i++) if(enemies[i]) {
						enemies[i]->y += 0.4 + speed;
	    
	    					if(enemies[i]->y >= SCREEN_HEIGHT) {
	
	     						removeEnemy(i);
	
							if(player_score != 0)
								player_score -= 10;
	
						}
					}
	
					for(int i = 0; i < MAX_BULLETS; i++) if(bullets[i]) {
					
						bullets[i]->y += bullets[i]->dy;
   	 
    						if(bullets[i]->y < 0)
     							removeBullet(i);
						}

					checkCollisions();

					doRender();
	
					//SO that the CPU doesn't burn up
					SDL_Delay(10);
					
				}

				displayScore();
	
			}

			Quit();

		}

	}
	
	return 0;
}

