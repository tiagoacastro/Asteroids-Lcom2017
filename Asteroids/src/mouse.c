#include "mouse.h"
#include <minix/com.h>
#include <minix/drivers.h>
#include <minix/syslib.h>
#include <minix/driver.h>
#include <minix/sysutil.h>
#include "video_gr.h"
#include "spaceship.h"

#define H_RES             	1024
#define V_RES		 					  768
#define CROSSHAIR_BMP_X     30
#define CROSSHAIR_BMP_Y     28

//global variables
int hook_id_mouse = 12;
int byte_index = 0;
int packet[3];
int c = 4;

Mouse* mouse = NULL;

//extern unsigned long asmByte;
//unsigned long read_OUTBUF_asm();

// functions

int mouse_subscribe_int(void){
  int hook_id_copy = hook_id_mouse;
	if (sys_irqsetpolicy(MOUSE_IRQ, IRQ_REENABLE | IRQ_EXCLUSIVE, &hook_id_mouse) == OK)
		if (sys_irqenable(&hook_id_mouse) == OK)
			return BIT(hook_id_copy);
	return ERROR;
}

int mouse_unsubscribe_int(){
  if (sys_irqdisable(&hook_id_mouse) == OK)
		if (sys_irqrmpolicy(&hook_id_mouse) == OK)
			return 0;
	return ERROR;
}

int enable_stream_mode(){
  unsigned long ret;
  if(writeToMouse(EN_STREAM) == ERROR){
		return ERROR;
  }
	if(sys_inb(OUT_BUF, &ret) != OK){
    return ERROR;
  }
  if (ret == NACK){
    enable_stream_mode();
  }
  else
    if(ret != ACK)
      return ERROR;
  return 0;
}

int enable_packets(){
	if (disable_data_reporting() == ERROR){
		return ERROR;
  }
  if (enable_data_reporting() == ERROR){
    return ERROR;
  }
  if (enable_stream_mode() == ERROR){
		return ERROR;
  }
  return 0;
}

int disable_data_reporting(){
  unsigned long ret;
	if(writeToMouse(DIS_DATA_REPORTING) == ERROR){
		return ERROR;
  }
  if(sys_inb(OUT_BUF, &ret) != OK){
    return ERROR;
  }
  if (ret == NACK){
    disable_data_reporting();
  }
  else
    if(ret != ACK)
      return ERROR;
    return 0;
}

int enable_data_reporting(){
  unsigned long ret;
  if (writeToMouse(EN_DATA_REPORTING) == ERROR){
		return ERROR;
  }
  if(sys_inb(OUT_BUF, &ret) != OK){
    return ERROR;
  }
  if (ret == NACK){
    enable_data_reporting();
  }
  else
    if(ret != ACK)
      return ERROR;
  return 0;
}

unsigned long read_OUTBUF() {
	int try = 0;
	unsigned long stat, ret;
	while (try < 3) {

		if (sys_inb(STAT_REG, &stat) != OK)
			return ERROR;

		if (stat & OBF)
		{
			if(sys_inb(OUT_BUF, &ret) != OK)
        return ERROR;

			if ((stat & (PAR_ERR | TO_ERR)) == 0)
				return ret;
			else
				return ERROR;
		}
		tickdelay(micros_to_ticks(DELAY_US));
		try++;
	}
	return ERROR;
}

int writeToMouse(unsigned long cmd){
  if(writeToKBC(STAT_REG, WRITE_TO_MOUSE) == ERROR)
	 return ERROR;

  if(writeToKBC(OUT_BUF, cmd) == ERROR)
	 return ERROR;
return 0;
}

int writeToKBC(unsigned long reg, unsigned long cmd){
  unsigned long stat;
  int try = 0;

  while(try < 3)
  {
	  if(sys_inb(STAT_REG, &stat) != OK)
	 	  return ERROR;

	  if((stat & IBF) == 0)
	  {
		  if(sys_outb(reg, cmd) != OK)
			  return ERROR;
      else
		  return 0;
	  }
    tickdelay(micros_to_ticks(DELAY_US));
    try++;
  }
  return ERROR;
}

int packet_handler(){
  //read_OUTBUF_asm();
  //unsigned long byte = asmByte;
  unsigned long byte = read_OUTBUF();
	if (byte == ERROR){
		return ERROR;
	}
	if (byte_index == 0){
		if (byte & BIT(3))
			 packet[byte_index] = byte;
		byte_index++;
    return 0;
	}
  if (byte_index == 1){
    packet[byte_index] = byte;
    byte_index++;
		return 0;
  }
	if (byte_index == 2){
    packet[byte_index] = byte;
    byte_index = 0;
	  return 1;
	}
  return ERROR;
}

int empty_OUTBUFF() {
	unsigned long stat;
  if (sys_inb(STAT_REG, &stat) != OK)
    return ERROR;
	while ((stat & OBF) != 0) {
		if (sys_inb(OUT_BUF, &stat) != OK)
			return ERROR;
		tickdelay(micros_to_ticks(DELAY_US));
    if (sys_inb(STAT_REG, &stat) != OK)
      return ERROR;
	}
  return 0;
}

int mouseInsideRect(Rectangle* rect){
  return (rect->x1 - H_RES/2) <= getMouse()->x && getMouse()->x <= (rect->x2 - H_RES/2) && (rect->y1 - V_RES/2) <= getMouse()->y && getMouse()->y <= (rect->y2 - V_RES/2);
}

Mouse* newMouse(){
  Mouse* mouse = (Mouse*)malloc(sizeof(Mouse));

  mouse->x = 0;
  mouse->y = 0;
  mouse->xSign = 0;
  mouse->ySign = 0;
  mouse->deltaX = 0;
  mouse->deltaY = 0;
  mouse->speedMultiplier = 1.0;

  mouse->leftButton = 0;
  mouse->leftButtonReleased = 0;
  mouse->middleButton = 0;
  mouse->rightButton = 0;

  mouse->bmp = loadBitmap("/home/lcom/Asteroids/res/crosshair.bmp");

  return mouse;
}

Mouse* getMouse(){
  if(!mouse){
    mouse = newMouse();
  }
  return mouse;
}

void updateMouse(){
  Mouse* mouse = getMouse();
  if(packet_handler()){
    if (packet[0] & LB)
  		mouse->leftButton = 1;
  	else
    if(mouse->leftButton){
      mouse->leftButton = 0;
      mouse->leftButtonReleased = 1;
    }
    else{
      mouse->leftButton = 0;
      mouse->leftButtonReleased = 0;
    }

    if (packet[0] & MB)
    	mouse->middleButton = 1;
    else
    	mouse->middleButton = 0;

  	if (packet[0] & RB)
    mouse->rightButton = 1;
    else
    mouse->rightButton = 0;

  	if (packet[0] & XSIGN)
    mouse->xSign = 1;
    else
    mouse->xSign = 0;

    if (packet[0] & YSIGN)
    mouse->ySign = 1;
    else
    mouse->ySign = 0;

    int byte2 = packet[1];
    int byte3 = packet[2];

    //passing from 2's complement if needed
  	if (mouse->xSign) {
      byte2 = byte2 ^ 0xFF;
      byte2++;
      byte2 = -byte2;
  	}
    mouse->deltaX = mouse->deltaX + byte2;

    //passing from 2's complement if needed
  	if (mouse->ySign) {
      byte3 = byte3 ^ 0xFF;
      byte3++;
      byte3 = -byte3;
  	}
    mouse->deltaY = mouse->deltaY + byte3;
  }
}

void drawMouse(){
  Mouse* mouse = getMouse();
  mouse->x = mouse->x + mouse->deltaX;
  if(mouse->x > H_RES/2 - CROSSHAIR_BMP_X)
    mouse->x = H_RES/2 - CROSSHAIR_BMP_X;
  if(mouse->x < -H_RES/2)
    mouse->x = -H_RES/2;
  mouse->deltaX = 0;
  mouse->y = mouse->y - mouse->deltaY;
  if(mouse->y > V_RES/2 - CROSSHAIR_BMP_Y)
    mouse->y = V_RES/2 - CROSSHAIR_BMP_Y;
  if(mouse->y < -V_RES/2)
    mouse->y = -V_RES/2;
  mouse->deltaY = 0;
  if(mouse->leftButton == 1){
    if(c%8 == 0)
    shootBullet(mouse->x, mouse->y);
    c++;
  }else
  c=8;
  drawBitmap(mouse->bmp, H_RES/2 + mouse->x, V_RES/2 + mouse->y, ALIGN_LEFT);
}

void drawMouseMenu(){
  Mouse* mouse = getMouse();
  mouse->x = mouse->x + mouse->deltaX;
  if(mouse->x > H_RES/2 - CROSSHAIR_BMP_X)
    mouse->x = H_RES/2 - CROSSHAIR_BMP_X;
  if(mouse->x < -H_RES/2)
    mouse->x = -H_RES/2;
  mouse->deltaX = 0;
  mouse->y = mouse->y - mouse->deltaY;
  if(mouse->y > V_RES/2 - CROSSHAIR_BMP_Y)
    mouse->y = V_RES/2 - CROSSHAIR_BMP_Y;
  if(mouse->y < -V_RES/2)
    mouse->y = -V_RES/2;
  mouse->deltaY = 0;
  drawBitmap(mouse->bmp, H_RES/2 + mouse->x, V_RES/2 + mouse->y, ALIGN_LEFT);
}

void deleteMouse(){
  free(getMouse());
}

void resetMouseFlags(){
  Mouse* mouse = getMouse();
  mouse->leftButton = 0;
  mouse->leftButtonReleased = 0;
  mouse->middleButton = 0;
  mouse->rightButton = 0;
}
