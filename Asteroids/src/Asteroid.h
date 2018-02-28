#include "Bitmap.h"

/** @defgroup Asteroids Asteroids
* @{
*/
typedef struct {
    unsigned int size; // specifies the size of Asteroid
    Bitmap* map;
    int xpos,ypos;
    int xspeed,yspeed;
    int indx;

} Asteroid;

/**
* @brief Allocates the required memory for an asteroids and adds it to the game's asteroid array
*
* @param size	The size of the asteroid (1 2 or 3)
* @param x		The position on x
* @param y		The postion on y
* @param xspeed	The speed on x
* @param yspeed	The speed on y
*/
void createAsteroid(unsigned int size,unsigned int x,unsigned int y,int xspeed,int yspeed);
/**
* @brief Updates the state of all asteroids, by adding their speed to their position, if they leave the screen they appear on the other side
*/
void update_Asteroids();
/**
* @brief frees all memory allocated on the createAsteroid() function
*
* @param ast The asteroid we want to delete
*/
void destroyAsteroid(Asteroid* ast);
/**
* @brief Loads all the bitmaps for the asteroids.
*/
void loadAsteroidBitmaps();
/**
* @brief Frees all memory allocated on the loadAsteroidBitmaps() function
*/
void deleteAsteroidBitmaps();
/**
* @brief Spawns a random asteroid on the limits of the screen
*/
void randomAsteroid();
/**
* @brief Draws all the asteroids that exist on the screen
*/
void drawAsteroids();
/** @} end of Asteroid */
