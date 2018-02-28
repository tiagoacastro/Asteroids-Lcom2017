#ifndef __MAINMENUSTATE_H
#define __MAINMENUSTATE_H

#include "graphics.h"
#include "Bitmap.h"

#define H_RES             	1024
#define V_RES		 					  768

/** @defgroup MainMenuState MainMenuState
* @{
*/

typedef struct {
  int done;

  Rectangle* playButton;
  Rectangle* exitButton;
  Rectangle* highscoreButton;

  int next;

  Bitmap* background;
} MainMenuState;

/**
* @brief allocates all memory needed for the state and initializes struct values
*
* @return returns a MainMenuState struct pointer
*/
MainMenuState* newMainMenuState();

/**
* @brief cheks if a button is being pressed, if so sets the 'next' flag from the state struct to different values depending on wich button was pressed
*/
void updateMainMenuState();

/**
* @brief draws the buttons' outlines it the mouse is hovering them
*/
void drawMainMenuState();

/**
* @brief frees all memory allocated on newMainMenuState function
*/
void deleteMainMenuState();

/**
* @brief starts the state calling the needed fucntions and calling the interrupt handlers
*
* @return     returns 0 if ended properly, -1 if and error occurs
*/
int startMainMenuState();

/** @} end of MainMenuState */

#endif
