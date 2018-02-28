#ifndef __RTC_H
#define __RTC_H

#include "i8042.h"

/** @defgroup Rtc Rtc
* @{
*/

typedef struct {
  unsigned long year;
  unsigned long month;
  unsigned long day;
  unsigned long hour;
  unsigned long minute;
  unsigned long second;
} Date;

/**
* @brief subscribes rtc interrupts
*
* @return Returns bit order in interrupt mask if successfull and -1 if an error occured
*/
int rtc_subscribe_int(void);

/**
* @brief unsubscribes keyboard interrupts
*
* @return returns 0 if successfull and -1 if an error occured
*/
int rtc_unsubscribe_int();

/**
* @brief enables the update interrupts
*
* @return returns 0 if successfull and -1 if an error occured
*/
int rtc_enable();

/**
* @brief disables the update interrupts
*
* @return returns 0 if successfull and -1 if an error occured
*/
int rtc_disable();

/**
* @brief allocates the memory need for an object of the Date struct
*
* @return returns a pointer to the object of date type
*/
Date* newDate();

/**
* @brief frees the memory previously allocated for the Date object
*
* @param date               object of type date wich the memory will be freed
*/
void deleteDate(Date* date);

/**
* @brief handles the interrupts, loading the data (minute, hour, day, moneth, year) onto the date object received as parameter
*
* @param date               object of type date that will be updated
*/
void updateDate(Date* date);

/**
* @brief converts from bcd to int
*
* @param x                  number that will be converted
* @return returns number in int
*/
int bcd_to_int(unsigned long x);

/** @} end of Rtc */

#endif
