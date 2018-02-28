#include <minix/syslib.h>
#include <minix/drivers.h>
#include <machine/int86.h>
#include <sys/mman.h>
#include <sys/types.h>
#include "i8042.h"
#include "i8254.h"
#include "vbe.h"
#include "video_gr.h"
#include "graphics.h"

Rectangle* newRectangle(int x1, int y1, int x2, int y2){
  Rectangle* rect = (Rectangle*) malloc(sizeof(Rectangle));

  rect->x1 = x1;
  rect->x2 = x2;
  rect->y1 = y1;
  rect->y2 = y2;

  return rect;
}

void deleteRectangle(Rectangle* rect){
  free(rect);
}
