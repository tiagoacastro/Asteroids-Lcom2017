#include "collision.h"
#include <stdio.h>
#include <stdlib.h>

extern Game* game;


int checkCollision(Bitmap* first,int fx,int fy, Bitmap* second,int sx, int sy)
{
	return!((fx + first->bitmapInfoHeader.width) < sx ||
			(sx + second->bitmapInfoHeader.width) < fx ||
			(fy +first->bitmapInfoHeader.height) < sy ||
			(sy + second->bitmapInfoHeader.height) < fy);
}


void checkBulletAsteroidCollision()
{
    int i, j;
    for (i = 0; i < 100; i++) {
        if (game->bullets[i] == NULL)
            continue;
        for (j = 0; j < 100; j++) {
            if (game->asteroids[j] == NULL)
                continue;
            if (checkCollision(game->bullets[i]->map, game->bullets[i]->xpos,
                    game->bullets[i]->ypos, game->asteroids[j]->map,
                    game->asteroids[j]->xpos, game->asteroids[j]->ypos)) {
                destroyBullet(game->bullets[i]);
                if (game->asteroids[j]->size == 3) {
                    game->score += 30;
                    createAsteroid(2, game->asteroids[j]->xpos,
                            game->asteroids[j]->ypos,
                            game->asteroids[j]->xspeed +(game->asteroids[j]->xspeed / abs(game->asteroids[j]->xspeed)) ,
                            game->asteroids[j]->yspeed);
                    createAsteroid(2, game->asteroids[j]->xpos,
                            game->asteroids[j]->ypos,
                            game->asteroids[j]->xspeed,
                            game->asteroids[j]->yspeed + (game->asteroids[j]->yspeed / abs(game->asteroids[j]->yspeed)));
                } else if (game->asteroids[j]->size == 2) {
                    game->score += 10;
                    createAsteroid(1, game->asteroids[j]->xpos,
                            game->asteroids[j]->ypos,
                            game->asteroids[j]->xspeed + (game->asteroids[j]->xspeed / abs(game->asteroids[j]->xspeed)),
                            game->asteroids[j]->yspeed);
                    createAsteroid(1, game->asteroids[j]->xpos,
                            game->asteroids[j]->ypos,
                            game->asteroids[j]->xspeed,
                            game->asteroids[j]->yspeed + (game->asteroids[j]->yspeed / abs(game->asteroids[j]->yspeed)));
                }
                game->score +=10;
                destroyAsteroid(game->asteroids[j]);

                j = 100;

            }
        }
    }
   return;
}

int checkAsteroidShipCollision()
{
	int j;
	for(j=0;j<100;j++)
			{
				if(game->asteroids[j] == NULL)
					continue;
				if(checkCollision(game->ship->map,game->ship->xpos,
						game->ship->ypos,game->asteroids[j]->map,game->asteroids[j]->xpos,
						game->asteroids[j]->ypos))
				{
					return 1;
				}
			}
	return 0;
}
