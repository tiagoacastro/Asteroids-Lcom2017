#ifndef _SPACESHIP_H_
#define _SPACESHIP_H_

#include "Bitmap.h"

/** @defgroup Spaceship Spaceship
* @{
*/

typedef struct {
    Bitmap* map;
    int xpos,ypos;
    int xspeed,yspeed;

} Spaceship;


typedef struct {
    Bitmap* map;
    int xpos,ypos;
    int xspeed,yspeed;
    int indx;

} Bullet;
/**
* @brief allocates all memory needed for the spaceship and initializes struct values
*
* @return returns a Spaceship struct pointer
*/
Spaceship* createSpaceship();
/**
* @brief Frees all memory allocated on createSpaceship() function
*/
void deleteSpaceship();
/**
* @brief Allocates all memory needed for the Bullet and adds it to the bullet array
*
*/
void createBullet();
/**
* @brief Updates the spaceship position on every keyboard interrupt
*
*/
void updateSpaceship();
/**
* @brief Draws the spaceship with the correct direction and position
*
*/
void drawSpaceship();
/**
* @brief By calling createBullet() it will spawn a bullet with the correct speed so that it points to the mouse cursor
* @param x The x coordinate to spawn the bullet
* @param y The y coordinate to spawn the bullet
*
*/
void shootBullet(int x, int y);
/**
* @brief Update the bullets positions by adding its speed to the postion, if it leaves the screen than they are deleted
*
*/
void updateBullets();
/**
* @brief Draws all the bullets on the bullet array
*
*/
void drawBullets();
// Algorithm adpatded from Java Implementation seen on VIDEO URL: https://www.youtube.com/watch?v=EnC1WKnN-H4
/**
* @brief Calculates the new rotated bitmap considering the degree between the spaceship and the mouse cursor
*
* @param map The pointer to the original bitmap
* @return returns a Bitmap pointer, pointing to the new rotated bitmap
*/
Bitmap* updateRotation(Bitmap* map);
/**
* @brief An auxiliar function to the updateRotation() that calculates the rotation on x axis
*
* @param degree The degree that we are rotating by
* @param x 1 if we want the value for the x coordinate after rotation 0 if we dont
* @param y 1 if we want the value for the y coordinate after rotation 0 if we dont
* @return The value after the rotation to use in the algorithm
*/
double rotx(double degree,double x,double y);
/**
* @brief An auxiliar function to the updateRotation() that calculates the rotation on y axis
*
* @param degree The degree that we are rotating by
* @param x 1 if we want the value for the x coordinate after rotation 0 if we dont
* @param y 1 if we want the value for the y coordinate after rotation 0 if we dont
* @return The value after the rotation to use in the algorithm
*/
double roty(double degree,double x,double y);
/**
* @brief Frees all memory allocated on createBullet() function
* @param The bullet we want to remove
*/
void destroyBullet(Bullet* bull);

/** @} end of Spaceship */
#endif
