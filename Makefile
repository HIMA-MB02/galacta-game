CC = cc
game: galacta.c
	cc galacta.c -w -lSDL2 -lSDL2_ttf -lSDL2_mixer -lSDL2_image -Wall -I/opt/homebrew/include -L/opt/homebrew/lib -o galacta