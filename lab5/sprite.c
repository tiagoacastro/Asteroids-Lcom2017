
#include "video_gr.h"
#include "sprite.h"
#include "read_xpm.h"
#include <stdlib.h>

Sprite *create_sprite(char *pic[], int x, int y, int xspeed, int yspeed) {
//allocate space for the "object"
	Sprite *sp = (Sprite *) malloc(sizeof(Sprite));
	if (sp == NULL)
		return NULL;
// read the sprite pixmap
	sp->map = read_xpm(pic, &(sp->width), &(sp->height));
	if (sp->map == NULL) {
		free(sp);
		return NULL;
	}
	sp->x = x;
	sp->y = y;
	sp->xspeed = xspeed;
	sp->yspeed = yspeed;
	return sp;
}

void destroy_sprite(Sprite *sp) {
	if (sp == NULL)
		return;
	if (sp->map)
		free(sp->map);
	free(sp);
	sp = NULL; // XXX: pointer is passed by value
// should do this @ the caller
}

void move_xpm(Sprite* sp) {
	vg_eraseSprite(sp);

	if((sp->xspeed) != 0)
	{
		sp->x += sp->xspeed;
	}
	else
	{
		sp->y += sp->yspeed;
	}

	vg_drawSprite(sp);
}
