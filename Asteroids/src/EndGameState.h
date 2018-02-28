#ifndef __ENDGAMESTATE_H
#define __ENDGAMESTATE_H

#include "graphics.h"
#include "Bitmap.h"

/** @defgroup EndGameState EndGameState
* @{
*/

typedef struct {
  int done;

  Rectangle* exitButton;

  Bitmap* background;
} EndGameState;

/**
* @brief allocates all memory needed for the state and initializes struct values
*
* @return returns a EndGameState struct pointer
*/
EndGameState* newEndGameState();

/**
* @brief cheks if the exit button is being pressed, if so sets the done flag form the state struct to 1
*/
void updateEndGameState();

/**
* @brief frees all memory allocated on newEndGameState function
*/
void deleteEndGameState();

/**
* @brief starts the state calling the needed fucntions and calling the interrupt handlers
*
* @return     returns 0 if ended properly, -1 if and error occurs
*/
int startEndGameState();

/**
* @brief draws the state's background
*/
void drawEndGameBackground();

/**
* @brief draws the button outline if the mouse is hovering it
*/
void drawEndGameState();

/**
* @brief draws a number centered on the value of x and y
*
* @param number         number to print
* @param x              pixel on the x axis to start the bitmap draw
* @param y              pixel on the y axis to start the bitmap draw
*/
void drawScore(int number, int x, int y);

/**
* @brief loads all number bitmaps on to the array
*/
void loadNumbers();

/**
* @brief frees al memory from the array used to store all number bitmaps
*/
void deleteNumbers();

 /** @} end of EndGameState */

#endif
