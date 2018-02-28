#ifndef __MOUSE_H
#define __MOUSE_H

#include "i8042.h"
#include "Bitmap.h"
#include "graphics.h"

/** @defgroup Mouse Mouse
* @{
*/

typedef struct {
    int x, y;
    int xSign, ySign;
    int deltaX, deltaY;
    double speedMultiplier;

    int leftButton;
    int leftButtonReleased;
    int middleButton;
    int rightButton;

    Bitmap* bmp;
} Mouse;

/**
* @brief subscribes mouse interrupts
*
* @return Returns bit order in interrupt mask if successfull and -1 if an error occured
*/
int mouse_subscribe_int(void);

/**
* @brief unsubscribes mouse interrupts
*
* @return returns 0 if successfull and -1 if an error occured
*/
int mouse_unsubscribe_int();

/**
* @brief enables mouse stream mode
*
* @return returns 0 if successfull and -1 if an error occured
*/
int enable_stream_mode();

/**
* @brief enables mouse data reporting
*
* @return returns 0 if successfull and -1 if an error occured
*/
int enable_data_reporting();

/**
* @brief disables mouse data reporting
*
* @return returns 0 if successfull and -1 if an error occured
*/
int disable_data_reporting();

/**
* @brief enables mouse data reporting and stream mode
*
* @return returns 0 if successfull and -1 if an error occured
*/
int enable_packets();

/**
* @brief reads the oubuff wich gives us 1 of 3 packet bytes
*
* @return returns a bye from the packet in question
*/
unsigned long read_OUTBUF();

/**
* @brief writes a command to the mouse
*
* @param cmd              the command that will be written
* @return returns 0 if successfull and -1 if an error occured
*/
int writeToMouse(unsigned long cmd);

/**
* @brief writes a command to the KBD
*
* @param reg              the register to wich the command will be written
* @param cmd              the command that will be written
* @return returns 0 if successfull and -1 if an error occured
*/
int writeToKBC(unsigned long reg, unsigned long cmd);

/**
* @brief handles the bytes in order to check if a full packet has been achieved
*
* @return returns 0 if successfull but a full packet hasn't been achieved, 1 if a full packet has been achieved and -1 if an error occured
*/
int packet_handler();

/**
* @brief clears the out buffer
*
* @return returns 0 if successfull and -1 if an error occured
*/
int empty_OUTBUFF();

/**
* @brief checks if the mouse is inside the rectangle
*
* @param                rectangle wich will be checked to see if the mouse is inside
* @return returns 0 if successfull and -1 if an error occured
*/
int mouseInsideRect(Rectangle* rect);

/**
* @brief allocates all memory needed for a newMouse and initializes struct values
*
* @return returns the mouse struct pointer
*/
Mouse* newMouse();

/**
* @brief gets the mouse pointer, if it is NULL (hasnt been created yet), calls newMouse function
*
* @return returns the mouse
*/
Mouse* getMouse();

/**
* @brief if a full packet has been achieved (packet_handler function returns 1), this function handles the packet updating the mouse flags and values
*/
void updateMouse();

/**
* @brief draws the mouse and shoots bullets if the Left Mouse is pressed
*/
void drawMouse();

/**
* @brief draws the mouse
*/
void drawMouseMenu();

/**
* @brief frees all memory previously allocated for the mouse
*/
void deleteMouse();

/**
* @brief sets all mouse flags to 0
*/
void resetMouseFlags();

/** @} end of Mouse */

#endif
