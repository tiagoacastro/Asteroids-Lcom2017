#ifndef TEST_H__
#define TEST_H__

#define BLACK                   0x000000
#define WHITE                   0xFFFFFF

/** @defgroup Graphics Graphics
* @{
*/

typedef struct {
  int x1;
  int y1;
  int x2;
  int y2;
} Rectangle;

/**
* @brief allocates all memory needed to create a new rectangle and initializes struct values
*
* @return returns a rectangle struct pointer
*/
Rectangle* newRectangle(int x1, int y1, int x2, int y2);

/**
* @brief frees all memory allocated for the rectangle
*
* @param rect               pointer to the rectangle wich memory will be freed
*/
void deleteRectangle(Rectangle* rect);

/** @} end of Graphics */

#endif /* TEST_H__*/
