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
#include "mouse.h"
#include <unistd.h>
#include "EndGameState.h"
#include "HighscoreState.h"

extern EndGameState* endGame;
extern int irq_set_mouse,irq_set_keyboard;
static Bitmap* rectangle;
extern int lastScore;
Bitmap* numbers[10];

EndGameState* newEndGameState() {
    EndGameState* state;
    state = (EndGameState*) malloc(sizeof(EndGameState));

    state->background = loadBitmap("/home/lcom/Asteroids/res/EndGame.bmp");

    rectangle = loadBitmap("/home/lcom/Asteroids/res/evenBiggerRectangle.bmp");

    int x1 = 320;
    int y1 = 580;
    int x2 = 680;
    int y2 = 630;
    Rectangle *r = newRectangle(x1,y1,x2,y2);
    state->exitButton = r;

    state->done = 0;

    loadNumbers();

    return state;
}

void updateEndGameState() {
    // if mouse is inside the exit button rectangle (boundaries)
    if (mouseInsideRect(endGame->exitButton))
      if(getMouse()->leftButtonReleased)
        endGame->done = 1;
}

void deleteEndGameState() {
    deleteRectangle(endGame->exitButton);

    deleteBitmap(endGame->background);

    deleteBitmap(rectangle);

    deleteNumbers();

    free(endGame);
}

void loadNumbers()
{
	numbers[0] = loadBitmap("/home/lcom/Asteroids/res/0big.bmp");
	numbers[1] = loadBitmap("/home/lcom/Asteroids/res/1big.bmp");
	numbers[2] = loadBitmap("/home/lcom/Asteroids/res/2big.bmp");
	numbers[3] = loadBitmap("/home/lcom/Asteroids/res/3big.bmp");
	numbers[4] = loadBitmap("/home/lcom/Asteroids/res/4big.bmp");
	numbers[5] = loadBitmap("/home/lcom/Asteroids/res/5big.bmp");
	numbers[6] = loadBitmap("/home/lcom/Asteroids/res/6big.bmp");
	numbers[7] = loadBitmap("/home/lcom/Asteroids/res/7big.bmp");
	numbers[8] = loadBitmap("/home/lcom/Asteroids/res/8big.bmp");
	numbers[9] = loadBitmap("/home/lcom/Asteroids/res/9big.bmp");
}

void deleteNumbers(){
	deleteBitmap(numbers[0]);
	deleteBitmap(numbers[1]);
	deleteBitmap(numbers[2]);
	deleteBitmap(numbers[3]);
	deleteBitmap(numbers[4]);
	deleteBitmap(numbers[5]);
	deleteBitmap(numbers[6]);
	deleteBitmap(numbers[7]);
	deleteBitmap(numbers[8]);
	deleteBitmap(numbers[9]);
}

void drawEndGameBackground(){
    drawBitmap(endGame->background, 0, 0, ALIGN_LEFT);
}

void drawEndGameState() {
    if (mouseInsideRect(endGame->exitButton))
        drawBitmap(rectangle, endGame->exitButton->x1 - 15, endGame->exitButton->y1 - 12, ALIGN_LEFT);
}

void drawScore(int number, int x, int y){
  int c = 0;
  int n = number;
  while (n >= 0)
  {
    c++;
    if(n < 10)
      break;
    else
      n = n / 10;
  }
  x += 9 * c;
  while (number >= 0)
  {
    x -= 18;
    int print = number % 10;
    switch(print)
    {
    case 0:
      drawBitmap(numbers[0], x, y, ALIGN_LEFT);
      break;
    case 1:
      drawBitmap(numbers[1], x, y, ALIGN_LEFT);
      break;
    case 2:
      drawBitmap(numbers[2], x, y, ALIGN_LEFT);
      break;
    case 3:
      drawBitmap(numbers[3], x, y, ALIGN_LEFT);
      break;
    case 4:
      drawBitmap(numbers[4], x, y, ALIGN_LEFT);
      break;
    case 5:
      drawBitmap(numbers[5], x, y, ALIGN_LEFT);
      break;
    case 6:
      drawBitmap(numbers[6], x, y, ALIGN_LEFT);
      break;
    case 7:
      drawBitmap(numbers[7], x, y, ALIGN_LEFT);
      break;
    case 8:
      drawBitmap(numbers[8], x, y, ALIGN_LEFT);
      break;
    case 9:
      drawBitmap(numbers[9], x, y, ALIGN_LEFT);
      break;
    }
    if(number < 10)
      break;
    else
      number = number / 10;
  }
}

int startEndGameState() {
	srand(time(NULL));
	int freq = 2;
	int counter = 0;
	int ipc_status, r;
	message msg;
	printf("1");
	int irq_set_timer = timer_subscribe_int();

	if (irq_set_timer == ERROR) {
		printf("Error subscribing to timer \n");
		return ERROR;
	}
	printf("2");
	while (endGame->done == 0) {
		if ((r = driver_receive(ANY, &msg, &ipc_status)) != 0) {
			printf("driver_receive failed with: %d", r);
			continue;
		}
		printf("3");
		if (is_ipc_notify(ipc_status)) {
			switch (_ENDPOINT_P(msg.m_source)) {
			case HARDWARE:
				if (msg.NOTIFY_ARG & irq_set_keyboard) {
					kbd_read_buffer_asm();

				}
				if (msg.NOTIFY_ARG & irq_set_mouse) {
					printf("4");
					updateMouse();
					printf("5");
					updateEndGameState();
					printf("6");
				}
				if (msg.NOTIFY_ARG & irq_set_timer) {
					if (counter % freq == 0) {
						printf("7");
						drawEndGameBackground();
						printf("8");
						drawMouseMenu();
						printf("9");
						drawEndGameState();
						printf("10");
						drawScore(lastScore, 500, 410);
						printf("11");
						swapBuffer();
						printf("12");
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
