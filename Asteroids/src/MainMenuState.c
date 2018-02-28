#include <string.h>
#include <minix/syslib.h>
#include <minix/drivers.h>
#include <minix/com.h>
#include <sys/mman.h>
#include <sys/types.h>
#include <stdint.h>
#include <minix/syslib.h>
#include <minix/drivers.h>
#include <minix/driver.h>
#include "graphics.h"
#include "time.h"
#include "vbe.h"
#include "lmlib.h"
#include "timer.h"
#include "i8042.h"
#include "i8254.h"
#include "keyboard.h"
#include "lmlib.h"
#include "video_gr.h"
#include <machine/int86.h>
#include "Bitmap.h"
#include "MainMenuState.h"
#include "mouse.h"

extern MainMenuState* menu;
extern int irq_set_mouse,irq_set_keyboard;
static Bitmap* rectangles[2];

MainMenuState* newMainMenuState() {
    MainMenuState* state;
    state = (MainMenuState*) malloc(sizeof(MainMenuState));

    state->background = loadBitmap("/home/lcom/Asteroids/res/menu.bmp");

    rectangles[0] = loadBitmap("/home/lcom/Asteroids/res/smallRectangle.bmp");
    rectangles[1] = loadBitmap("/home/lcom/Asteroids/res/bigRectangle.bmp");

    int x1 = 460;
    int y1 = 300;
    int x2 = 540;
    int y2 = 340;
    Rectangle *r = newRectangle(x1,y1,x2,y2);
    state->playButton = r;

    y1 = 500;
    y2 = 540;
    r = newRectangle(x1,y1,x2,y2);
    state->exitButton = r;

    x1 = 390;
    y1 = 390;
    x2 = 610;
    y2 = 440;
    r = newRectangle(x1,y1,x2,y2);
    state->highscoreButton = r;

    state->done = 0;

    state->next = 0;

    return state;
}

void updateMainMenuState() {

    // if mouse is inside the play button rectangle (boundaries)
    if (mouseInsideRect(menu->playButton))
        menu->next = 1;
    else
    // if mouse is inside the highscore button rectangle (boundaries)
    if (mouseInsideRect(menu->highscoreButton))
        menu->next = 2;
    else
    // if mouse is inside the exit button rectangle (boundaries)
    if (mouseInsideRect(menu->exitButton))
        menu->next = 3;
    else
        menu->next = 0;

    if(getMouse()->leftButtonReleased && menu->next != 0)
        menu->done = 1;
}


void drawMainMenuState() {
    drawBitmap(menu->background, 0, 0, ALIGN_LEFT);
    int next = menu->next;
    switch(next){
      case 0:
        break;
      case 1:
        drawBitmap(rectangles[0], menu->playButton->x1 - 17, menu->playButton->y1 - 5, ALIGN_LEFT);
        break;
      case 2:
        drawBitmap(rectangles[1], menu->highscoreButton->x1 - 15, menu->highscoreButton->y1 + 5, ALIGN_LEFT);
        break;
      case 3:
        drawBitmap(rectangles[0], menu->exitButton->x1 - 20, menu->exitButton->y1 - 7, ALIGN_LEFT);
        break;
    }
}


void deleteMainMenuState() {
    deleteRectangle(menu->playButton);
    deleteRectangle(menu->exitButton);
    deleteRectangle(menu->highscoreButton);

    deleteBitmap(menu->background);
    deleteBitmap(rectangles[0]);
    deleteBitmap(rectangles[1]);

    free(menu);
}

int startMainMenuState() {
	srand(time(NULL));
	int freq = 2;
	int counter = 0;
	int ipc_status, r;
	message msg;

	int irq_set_timer = timer_subscribe_int();

	if (irq_set_timer == ERROR) {
		printf("Error subscribing to timer \n");
		return ERROR;
	}

	while (menu->done == 0) {
		if ((r = driver_receive(ANY, &msg, &ipc_status)) != 0) {
			printf("driver_receive failed with: %d", r);
			continue;
		}

		if (is_ipc_notify(ipc_status)) {
			switch (_ENDPOINT_P(msg.m_source)) {
			case HARDWARE:
				if (msg.NOTIFY_ARG & irq_set_mouse) {
					updateMouse();
					updateMainMenuState();
				}
				if (msg.NOTIFY_ARG & irq_set_keyboard) {
					kbd_read_buffer_asm();

				}
				if (msg.NOTIFY_ARG & irq_set_timer) {

					if (counter % freq == 0) {
						drawMainMenuState();
						drawMouseMenu();
						swapBuffer();
					}
					counter++;
				}
			default:
				break;
			}

		}
	}

	if (timer_unsubscribe_int() == ERROR) {
		printf("Error subscribing to timer \n");
		return ERROR;
	}

	return 0;
}
