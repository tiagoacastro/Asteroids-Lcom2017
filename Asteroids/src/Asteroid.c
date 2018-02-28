#include "video_gr.h"
#include <stdlib.h>
#include "Bitmap.h"
#include "game.h"
#include <stdio.h>
extern Game* game;
static Bitmap* BigAsteroid;
static Bitmap* MediumAsteroid;
static Bitmap* SmallAsteroid;

void loadAsteroidBitmaps() {
	BigAsteroid= loadBitmap("/home/lcom/Asteroids/res/BIG1.bmp");
	MediumAsteroid= loadBitmap("/home/lcom/Asteroids/res/MED1.bmp");
	SmallAsteroid = loadBitmap("/home/lcom/Asteroids/res/SMALL1rec.bmp");
}

void deleteAsteroidBitmaps() {
	deleteBitmap(BigAsteroid);
	deleteBitmap(MediumAsteroid);
	deleteBitmap(SmallAsteroid);
}

void createAsteroid(unsigned int size, unsigned int x,
		unsigned int y, int xspeed, int yspeed) {
	Asteroid *ast = (Asteroid *) malloc(sizeof(Asteroid));
	if (ast == NULL)
		return ;

	ast->size = size;

	if (size == 3)
		ast->map = BigAsteroid;
		//ast->map = loadBitmap("/home/lcom/Asteroids/res/BIG1.bmp");

	if (size == 2)
		ast->map = MediumAsteroid;
		//ast->map = loadBitmap("/home/lcom/Asteroids/res/MED1.bmp");

	if (size == 1)
		ast->map = SmallAsteroid;
		//ast->map = loadBitmap("/home/lcom/Asteroids/res/SMALL1.bmp");

	ast->xspeed = xspeed;
	ast->yspeed = yspeed;
	ast->xpos = x;
	ast->ypos = y;
	int add = 0;
	while (game->asteroids[add] != NULL)
			add++;
	game->asteroids[add] = ast;

	ast->indx = add;

	return ;
}

void update_Asteroids() {
	int i;
	for (i = 0; i < 100; i++) {
		if (game->asteroids[i] != NULL) {
			game->asteroids[i]->xpos += game->asteroids[i]->xspeed;
			game->asteroids[i]->ypos += game->asteroids[i]->yspeed;


			if (game->asteroids[i]->xpos < 0) {
				game->asteroids[i]->xpos = 1000;

			}
			if (game->asteroids[i]->xpos >= 1024 ) {
				game->asteroids[i]->xpos = 10;
			}

			if (game->asteroids[i]->ypos >= 768)  {
				game->asteroids[i]->ypos = 10;
			}
			if (game->asteroids[i]->ypos < 0 )  {
				game->asteroids[i]->ypos = 767;
			}

		}
	}
}

void destroyAsteroid(Asteroid* ast) {
	int remove = ast->indx;
		//deleteBitmap(ast->map);
		free(game->asteroids[remove]);
		game->asteroids[remove] = NULL;
		ast = NULL;
}

void drawAsteroids()
{
	int i;
	for (i = 0; i < 100; i++) {
		if (game->asteroids[i] != NULL)
		{
			if(game->asteroids[i]->size == 1)
				if((game->asteroids[i]->xpos + 24 > 1024 && game->asteroids[i]->ypos + 24 > 768)
					|| (game->asteroids[i]->xpos - 24 < 0 && game->asteroids[i]->ypos + 24 > 768)
					 || (game->asteroids[i]->xpos - 24 < 0 && game->asteroids[i]->ypos - 24 < 0)
					 || (game->asteroids[i]->xpos + 24 < 1024 && game->asteroids[i]->ypos - 24 < 0))
					continue;
			drawBitmap(game->asteroids[i]->map, game->asteroids[i]->xpos,
					game->asteroids[i]->ypos, ALIGN_LEFT);

		}
	}
}

void randomAsteroid()
{
	int xs = rand() % 1;
	int ys;
	if (xs == 0)
		ys = 1;
	else
		ys = 0;
	int xspeed = (rand() % 4) - 2;
	int yspeed = (rand() % 4) - 2;
	if (xspeed == 0)
		xspeed = 1;
	if(yspeed == 0)
		yspeed =1;

	createAsteroid(3, (rand() % 1000) * xs, (rand() % 700) * ys,
			xspeed, yspeed);


}
