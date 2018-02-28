#ifndef __COLLISION_H
#define __COLLISION_H

#include "Bitmap.h"
#include "spaceship.h"
#include "game.h"
/** @defgroup Collision Collision
 * @{
 * Functions for collisions
 */
/**
 * @brief Checks AABB collisions between 2 bitmaps
 *
 * @param first The first bitmap that we will check if it collides with the second
 * @param fx The x coordinate of the first bitmap
 * @param fy The y coordinate of the first bitmap
 * @param second The second bitmap
 * @param sx The x coordinate of the second bitmap
 * @param sy The y coordinate of the second bitmap
 * @return Non NULL pointer to the image buffer
 */
int checkCollision(Bitmap* first,int fx,int fy ,Bitmap* second,int sx, int sy);
/**
* @brief Checks if any bullet hit any of the asteroids, if it did it will delete both the bullet and the asteroid
*/
void checkBulletAsteroidCollision();
/**
 * @brief Checks ifs any asteroid collided with the spaceship
 *
 * @return	1 on collision 0 on non collision
 */
int checkAsteroidShipCollision();
/** @} end of Collision */
#endif
