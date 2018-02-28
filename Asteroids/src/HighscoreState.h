#ifndef __HIGHSCORESTATE_H
#define __HIGHSCORESTATE_H

#include "graphics.h"
#include <string.h>
#include "rtc.h"
#include "Bitmap.h"

/** @defgroup EndGameState EndGameState
* @{
*/

typedef struct {
  int done;

  Rectangle* exitButton;

  Bitmap* background;
} HighscoreState;

typedef struct {
  int score;

  Date* date;

  char* name;
} HighestScore;

/**
* @brief allocates all memory needed for the state and initializes struct values
*
* @return returns a HighscoreState struct pointer
*/
HighscoreState* newHighscoreState();

/**
* @brief cheks if the exit button is being pressed, if so sets the done flag from the state struct to 1
*/
void updateHighscoreState();

/**
* @brief draws an highscore (name, date, score), doesnt need x parameter because it doesnt change from highscore to highscore
*
* @param hs             pointer to the highscore to print
* @param y              pixel on the y axis to start the highscore draw
*/
void drawHighscore(HighestScore* hs, int y);

/**
* @brief frees all memory allocated on newHighscoreState function
*/
void deleteHighscoreState();

/**
* @brief starts the state calling the needed fucntions and calling the interrupt handlers
*
* @return     returns 0 if ended properly, -1 if and error occurs
*/
int startHighscoreState();

/**
* @brief  checks if the score obtained is an highscore, sets the newHighscoreFlag to 1 if it is the best highscore, 2 if it is the second best, 3 if it is the third best
*
* @param score            score of the last game to be analyzed
* @return     returns 1 if it's and highscore, 0 if not
*/
int checkHighscore(int score);

/**
* @brief loads all bitmaps needed for the state on to the array
*/
void loadLetters();

/**
* @brief frees al memory from the array used to store all bitmaps needed for the state
*/
void deleteLetters();

/**
* @brief draws the state's background
*/
void drawHighscoreBackground();

/**
* @brief allocates all memory needed to create a new highscore and initializes struct values
*
* @return returns a pointer to an highscore
*/
HighestScore* newHighscore();

/**
* @brief frees all memory allocated for the highscore
*
* @param hs             pointer for the highscore wich memory will be freed
*/
void deleteHighscore(HighestScore* hs);

/**
* @brief draws a number starting on the value of x and y
*
* @param number         number to print
* @param x              pixel on the x axis to start the bitmap draw
* @param y              pixel on the y axis to start the bitmap draw
* @return               returns the x where the last number's bitmap was written
*/
int drawNumber(int number, int x, int y);

/**
* @brief draws a date starting on the value of x and y (if number has only one digit it draws a 0 on the left)
*
* @param number         number to print
* @param x              pixel on the x axis to start the bitmap draw
* @param y              pixel on the y axis to start the bitmap draw
* @return               returns the x where the last number's bitmap was written
*/
int drawDate(int number, int x, int y);

/**
* @brief draws the highscorer's name
*
* @param hs             pointer to the highscore
* @param x              pixel on the x axis to start the bitmap draw
* @param y              pixel on the y axis to start the bitmap draw
*/
void printHighscoreName(HighestScore* hs, int x, int y);

/**
* @brief reads the highscores from the file where they are stored
*/
void readScoresFile();

/**
* @brief prints the highscores to a file
*/
void writeScoresFile();

/**
* @brief allocates all memory needed to create a new string
*
* @return returns a pointer to the string
*/
char* newString();

/**
* @brief frees all memory allocated for the string
*
* @param string           pointer for the string wich memory will be freed
*/
void deleteString(char* string);

/** @} end of EndGameState */

#endif
