#ifndef __NEWHIGHSCORESTATE_H
#define __NEWHIGHSCORESTATE_H

#include "graphics.h"
#include "Bitmap.h"

/** @defgroup NewHighscoreState NewHighscoreState
* @{
*/

typedef struct {
  Bitmap* background;
} NewHighscoreState;

/**
* @brief allocates all memory needed for the state and initializes struct values
*
* @return returns a NewHighscoreState struct pointer
*/
NewHighscoreState* newNewHighscoreState();

/**
* @brief frees all memory allocated on newNewHighscoreState function
*/
void deleteNewHighscoreState();

/**
* @brief starts the state calling the needed fucntions and calling the interrupt handlers
*
* @return     returns 0 if ended properly, -1 if and error occurs
*/
int startNewHighscoreState();

/**
* @brief draws the state's background
*/
void drawNewHighscoreStateBackground();

/**
* @brief adds the pressed key to the string, if it's back space it removes the last character from the string
*
* @return returns hte index of the letter in the array
*/
int keyHandler();

/**
* @brief prints the string to show the user the name he is writing centered in the x value
*
* @param x              pixel on the x axis to start the bitmap draw
* @param y              pixel on the y axis to start the bitmap draw
*/
void printName(int x, int y);

/**
* @brief loads all alphabet bitmaps
*/
void loadAlphabet();

/**
* @brief frees al memory from the array used to store all alphabet bitmaps
*/
void deleteAlphabet();

/** @} end of NewHighscoreState */

#endif
