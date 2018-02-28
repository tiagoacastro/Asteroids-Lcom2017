#include "spaceship.h"
#include "video_gr.h"
#include <stdlib.h>
#include "Bitmap.h"
#include "game.h"
#include "keyboard.h"
#include "i8042.h"
#include <math.h>
#include "mouse.h"

extern Game* game;
extern Mouse* mouse;
#define BY1_IGN 0x1f
#define BY2_IGN 0xfffffff8
#define IGNORE_COL 0xf81f
#define PI 3.141592653
//static int toAdd = 0;

#define H_RES             	1024
#define V_RES		 					  768
#define SPACESHIP_BMP_X     32
#define SPACESHIP_BMP_Y			37
#define CROSSHAIR_BMP_X     30
#define CROSSHAIR_BMP_Y     28
#define SPEED								5

Spaceship* createSpaceship()
{
	Spaceship *ship = (Spaceship *) malloc(sizeof(Spaceship));
		if (ship == NULL)
			return NULL;
	ship->xpos = H_RES / 2;
	ship->ypos = V_RES / 2;
	ship->xspeed = 0;
	ship->yspeed = 0;
	ship->map = loadBitmap("/home/lcom/Asteroids/res/spaceship.bmp");

	return ship;
}

void deleteSpaceship(){
	deleteBitmap(game->ship->map);
	free(game->ship);
}

void updateSpaceship(){
	if(key == W_MAKE){
		game->ship->yspeed = -SPEED;
	}

	if(key == W_BREAK){
		if(game->ship->yspeed == -SPEED)
			game->ship->yspeed = 0;
	}

	if(key == S_MAKE){
		game->ship->yspeed = SPEED;
	}

	if(key == S_BREAK){
		if(game->ship->yspeed == SPEED)
			game->ship->yspeed = 0;
	}

	if(key == D_MAKE){
		game->ship->xspeed = SPEED;
	}

	if(key == D_BREAK){
		if(game->ship->xspeed == SPEED)
			game->ship->xspeed = 0;
	}

	if(key == A_MAKE){
		game->ship->xspeed = -SPEED;
	}


	if(key == A_BREAK){
		if(game->ship->xspeed == -SPEED)
			game->ship->xspeed = 0;
	}
}



Bitmap* updateRotation(Bitmap* map) {
	Bitmap* temp = (Bitmap*) malloc(sizeof(Bitmap));
	BitmapInfoHeader bitmapInfoHeader;
	bitmapInfoHeader = map->bitmapInfoHeader;
	short* image = (short*) malloc(map->bitmapInfoHeader.imageSize);
	temp->bitmapData = image;
	temp->bitmapInfoHeader = bitmapInfoHeader;

	double degree;
	double xstart = mouse->x - (game->ship->xpos) + H_RES / 2;
	double ystart = mouse->y - (game->ship->ypos) + V_RES / 2;
	degree = atan2(ystart, xstart);


	double new_xx = rotx(degree,1.0,0.0);
	double new_xy = roty(degree,1.0,0.0);
	double new_yx = rotx(degree,0.0,1.0);
	double new_yy = roty(degree,0.0,1.0);

	int width = map->bitmapInfoHeader.width;
	int height = map->bitmapInfoHeader.height;

	double x0 = rotx(degree, -width/2.0,-height/2.0) + width /2.0;
	double y0 = roty(degree, -width/2.0,-height/2.0) + height /2.0;

	int y,x;

	for(y=0;y<height; y++ )
	{
		double xi = x0;
		double yi=y0;
		for(x=0; x<width;x++)
		{
			int xf = (int) xi;
			int yf = (int) yi;
			if(xf<0 || xf>=width || yf < 0 || yf >= height)
			{
				temp->bitmapData[(int)(y*width + x)] = IGNORE_COL;
			}
			else
			{
			temp->bitmapData[(y*width + x)] = map->bitmapData[(yf*width + xf)];
			}
			xi += new_xx;
			yi += new_xy;

		}
		x0 += new_yx;
		y0 += new_yy;
	}

	return temp;

}
double rotx(double degree,double x,double y)
{
	double cose = cos(degree + PI/2);
	double sine = sin(degree+PI/2);
	return x * cose + y * -sine;
}

double roty(double degree,double x,double y)
{
	double cose = cos(degree +PI/2);
	double sine = sin(degree +PI/2);
	return x * sine + y * cose;
}

void drawSpaceship()
{
	game->ship->xpos = game->ship->xpos + game->ship->xspeed;
	game->ship->ypos = game->ship->ypos + game->ship->yspeed;


	if(game->ship->xpos > H_RES - SPACESHIP_BMP_X)
    game->ship->xpos = H_RES - SPACESHIP_BMP_X;
  if(game->ship->xpos < 0)
    game->ship->xpos = 0;

	if(game->ship->ypos > V_RES - SPACESHIP_BMP_Y -50)
		game->ship->ypos = V_RES - SPACESHIP_BMP_Y - 50;
	if(game->ship->ypos < 0)
		game->ship->ypos = 0;

	drawBitmap(updateRotation(game->ship->map), game->ship->xpos,game->ship->ypos, ALIGN_LEFT);
}

void createBullet(int x, int y , int xspeed, int yspeed)
{
	Bullet* bull = (Bullet*) malloc(sizeof(Bullet));
	if(bull == NULL)
		return ;
	bull->xpos = game->ship->xpos + SPACESHIP_BMP_X/4;
	bull->ypos = game->ship->ypos - SPACESHIP_BMP_Y/4;
	bull->xspeed = xspeed;
	bull->yspeed = yspeed;
	bull->map = loadBitmap("/home/lcom/Asteroids/res/bullet2.bmp");
	int add = 0;
	while (game->bullets[add] != NULL)
		add++;
	game->bullets[add] = bull;

	bull->indx = add;

}

void shootBullet(int x, int y)
{
	float degree;
	float xstart = mouse->x + CROSSHAIR_BMP_X/2 - (game->ship->xpos) + H_RES / 2;
	float ystart = mouse->y + CROSSHAIR_BMP_Y/2 - (game->ship->ypos) + V_RES / 2;
	degree = atan2(ystart,xstart);

	createBullet(0,0,cos(degree)*6,sin(degree)*6);


}
void destroyBullet(Bullet *bull) {
	int remove = bull->indx;
	deleteBitmap(bull->map);
	free(game->bullets[remove]);
	game->bullets[remove] = NULL;
	bull = NULL;
}

void updateBullets() {
	int i;
	for (i = 0; i < 100; i++) {
		if (game->bullets[i] != NULL) {
			game->bullets[i]->xpos += game->bullets[i]->xspeed;
			game->bullets[i]->ypos += game->bullets[i]->yspeed;

			if (game->bullets[i]->xpos < 0) {
				destroyBullet(game->bullets[i]);
				continue;
			}
			if (game->bullets[i]->xpos > 1000) {
				destroyBullet(game->bullets[i]);
				continue;
			}
			if (game->bullets[i]->ypos > 760) {
				destroyBullet(game->bullets[i]);
				continue;
			}
			if (game->bullets[i]->ypos < 0) {
				destroyBullet(game->bullets[i]);
			}
		}
	}
}

void drawBullets()
{
	int i;
	for (i = 0; i < 100; i++) {
		if (game->bullets[i] != NULL)
			drawBitmap(game->bullets[i]->map, game->bullets[i]->xpos +game->ship->map->bitmapInfoHeader.width/4,
					game->bullets[i]->ypos + game->ship->map->bitmapInfoHeader.height/2, ALIGN_CENTER);
	}
}
