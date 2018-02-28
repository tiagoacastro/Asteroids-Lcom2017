#ifndef __KEYBOARD_H
#define __KEYBOARD_H

/** @defgroup Keyboard Keyboard
* @{
*/

/**
* @brief subscribes keyboard interrupts
*
* @return Returns bit order in interrupt mask if successfull and -1 if an error occured
*/
int kbd_subscribe_int(void);

/**
* @brief unsubscribes keyboard interrupts
*
* @return returns 0 if successfull and -1 if an error occured
*/
int kbd_unsubscribe_int();

/**
* @brief handles keyboard interrupts, reading the out buffer and saving the pressed key in the key global
*/
void kbd_read_buffer_asm();

/** @} end of Keyboard */

unsigned long key;

#endif
