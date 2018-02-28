#ifndef __GAME_H
#define __GAME_H
#include "Bitmap.h"
#include "Asteroid.h"
#include "spaceship.h"

/** @defgroup Game Game
 * @{
 */
typedef struct {
 Asteroid* asteroids[100]; //All asteroids in the game
 Spaceship* ship;
 Bullet* bullets[100];	// All bullets in the game
 int score;

} Game;
/**
* @brief Allocates all memory needed for the game and initializes struct values
*
* @return returns a Game struct pointer
*/
Game* newGame();
/**
* @brief Frees all the memory allocated in the newGame() function
*
*/
void deleteGame();
/**
* @brief This function is responsable for the game state, it will update its state on every timer interrupt,it will increment the score and difficulty and end once there is a collision with an asteroid and the ship, or ESC is pressed
*
* @return returns The game score
*/
int startGame();
/**
* @brief Loads the bitmaps for the Hud and the Background
*
*/
void loadBackgroundandHud();
/**
* @brief Deletes the Hud and background bitmaps
*/
void deleteBackgroundandHud();
/**
* @brief Draws the background and all the asteroids in the game
*
*/
void drawBuff();
/**
* @brief Draws the Hud and the score
*
*/
void drawHudScore();
/** @} end of Game */
#endif


