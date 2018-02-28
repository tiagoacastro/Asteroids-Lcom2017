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
#include "game.h"
#include "mouse.h"
#include "spaceship.h"
#include "collision.h"
#include "HighscoreState.h"

extern Game* game;
static Bitmap* background;
static Bitmap* hud;
static Bitmap* alphabet[17];
extern int irq_set_mouse,irq_set_keyboard;
extern Date* currentDate;

Game* newGame() {
	Game* game;
	game = (Game *) malloc(sizeof(Game));
	int i;
	for (i = 0; i < 100; i++) {
		game->asteroids[i] = NULL;
	}
	game->ship = createSpaceship();
	for (i = 0; i < 100; i++) {
			game->bullets[i] = NULL;
		}
	game->score = 0;
	loadAsteroidBitmaps();
	loadBackgroundandHud();
	return game;
}

void deleteGame(){
	int i;
	for (i = 0; i < 100; i++) {
		if(game->bullets[i] != NULL)
			destroyBullet(game->bullets[i]);
	}
	deleteSpaceship();
	for (i = 0; i < 100; i++) {
		if(game->asteroids[i] != NULL)
			destroyAsteroid(game->asteroids[i]);
	}
	free(game);
	deleteBackgroundandHud();
	deleteAsteroidBitmaps();
}

void loadBackgroundandHud()
{
	background = loadBitmap("/home/lcom/Asteroids/res/back.bmp");
	hud = loadBitmap("/home/lcom/Asteroids/res/Hud.bmp");
	alphabet[0] = loadBitmap("/home/lcom/Asteroids/res/0.bmp");
	alphabet[1] = loadBitmap("/home/lcom/Asteroids/res/1.bmp");
	alphabet[2] = loadBitmap("/home/lcom/Asteroids/res/2.bmp");
	alphabet[3] = loadBitmap("/home/lcom/Asteroids/res/3.bmp");
	alphabet[4] = loadBitmap("/home/lcom/Asteroids/res/4.bmp");
	alphabet[5] = loadBitmap("/home/lcom/Asteroids/res/5.bmp");
	alphabet[6] = loadBitmap("/home/lcom/Asteroids/res/6.bmp");
	alphabet[7] = loadBitmap("/home/lcom/Asteroids/res/7.bmp");
	alphabet[8] = loadBitmap("/home/lcom/Asteroids/res/8.bmp");
	alphabet[9] = loadBitmap("/home/lcom/Asteroids/res/9.bmp");
	alphabet[10] = loadBitmap("/home/lcom/Asteroids/res/s.bmp");
	alphabet[11] = loadBitmap("/home/lcom/Asteroids/res/c.bmp");
	alphabet[12] = loadBitmap("/home/lcom/Asteroids/res/o.bmp");
	alphabet[13] = loadBitmap("/home/lcom/Asteroids/res/r.bmp");
	alphabet[14] = loadBitmap("/home/lcom/Asteroids/res/e.bmp");
	alphabet[15] = loadBitmap("/home/lcom/Asteroids/res/colon.bmp");
	alphabet[16] = loadBitmap("/home/lcom/Asteroids/res/backslash.bmp");

}

void deleteBackgroundandHud(){
	deleteBitmap(background);
	deleteBitmap(hud);
	deleteBitmap(alphabet[0]);
	deleteBitmap(alphabet[1]);
	deleteBitmap(alphabet[2]);
	deleteBitmap(alphabet[3]);
	deleteBitmap(alphabet[4]);
	deleteBitmap(alphabet[5]);
	deleteBitmap(alphabet[6]);
	deleteBitmap(alphabet[7]);
	deleteBitmap(alphabet[8]);
	deleteBitmap(alphabet[9]);
	deleteBitmap(alphabet[10]);
	deleteBitmap(alphabet[11]);
	deleteBitmap(alphabet[12]);
	deleteBitmap(alphabet[13]);
	deleteBitmap(alphabet[14]);
	deleteBitmap(alphabet[15]);
	deleteBitmap(alphabet[16]);
}

void drawBuff() {

	drawBack(background, 0, 0, ALIGN_LEFT);
	drawAsteroids();

}
void drawHudScore()
{
	drawBitmap(hud,15,710,ALIGN_LEFT);
	drawBitmap(alphabet[10],65,733,ALIGN_LEFT);
	drawBitmap(alphabet[11],80,733,ALIGN_LEFT);
	drawBitmap(alphabet[12],100,734,ALIGN_LEFT);
	drawBitmap(alphabet[13],120,733,ALIGN_LEFT);
	drawBitmap(alphabet[14],135,733,ALIGN_LEFT);
	drawBitmap(alphabet[15],150,730,ALIGN_LEFT);

	int tscore = game->score;
	int xPos = 350;
	while (tscore >= 0)
	{
		xPos -= 20;
		int print = tscore % 10;
		switch(print)
		{
		case 0:
			drawBitmap(alphabet[0], xPos, 730, ALIGN_LEFT);
			break;
		case 1:
			drawBitmap(alphabet[1], xPos, 730, ALIGN_LEFT);
			break;
		case 2:
			drawBitmap(alphabet[2], xPos, 730, ALIGN_LEFT);
			break;
		case 3:
			drawBitmap(alphabet[3], xPos, 730, ALIGN_LEFT);
			break;
		case 4:
			drawBitmap(alphabet[4], xPos, 730, ALIGN_LEFT);
			break;
		case 5:
			drawBitmap(alphabet[5], xPos, 730, ALIGN_LEFT);
			break;
		case 6:
			drawBitmap(alphabet[6], xPos, 730, ALIGN_LEFT);
			break;
		case 7:
			drawBitmap(alphabet[7], xPos, 730, ALIGN_LEFT);
			break;
		case 8:
			drawBitmap(alphabet[8], xPos, 730, ALIGN_LEFT);
			break;
		case 9:
			drawBitmap(alphabet[9], xPos, 730, ALIGN_LEFT);
			break;

		}
		if(tscore < 10)
			break;
		else
			tscore = tscore / 10;
	}

}

int startGame()
{
	srand(time(NULL));
	key = 0x00;
	//int freq = 2;
	int counter = 0;
	//createAsteroid(1,400,300,-4,5);
	//createAsteroid(1,920,654,1,1);
	int timePassed = 0;
	int ipc_status, r;
	int endGame=0;
	message msg;


	int irq_set_timer = timer_subscribe_int();

	if (irq_set_timer == ERROR) {
		printf("Error subscribing to timer \n");
		return ERROR;
	}



	int irq_set_rtc = rtc_subscribe_int();

	if (irq_set_rtc == ERROR) {
		printf("Error subscribing to rtc \n");
		return ERROR;
	}

	if (rtc_enable() == ERROR) {
		printf("Error enabling rtc \n");
		return ERROR;
	}

	while (endGame ==0) {
		if ((r = driver_receive(ANY, &msg, &ipc_status)) != 0) {
			printf("driver_receive failed with: %d", r);
			continue;
		}

		if (is_ipc_notify(ipc_status)) {
			switch (_ENDPOINT_P(msg.m_source)) {
			case HARDWARE:
				if (msg.NOTIFY_ARG & irq_set_keyboard) {
					kbd_read_buffer_asm();
					updateSpaceship();
					if(key == ESC_KEY)
						endGame = 1;
				}
				if (msg.NOTIFY_ARG & irq_set_mouse) {
					updateMouse();
				}
				if (msg.NOTIFY_ARG & irq_set_rtc) {
					updateDate(currentDate);
				}
				if (msg.NOTIFY_ARG & irq_set_timer) {
					timePassed++;
					//if (counter % freq == 0) {
						if (game->score < 1000) {
							if (timePassed % 100 == 0) {
								randomAsteroid();
								game->score += 10;
							}
						} else if (game->score < 3000) {
							if (timePassed % 80 == 0) {
								randomAsteroid();
								game->score += 10;
							}
						} else if (game->score < 5000) {
							if (timePassed % 60 == 0) {
								randomAsteroid();
								game->score += 10;
							}
						} else
						{
							if (timePassed % 40 == 0)
							{
								randomAsteroid();
								game->score +=10;
							}
						}


						drawBuff();
						drawMouse();
						drawBullets();
						drawSpaceship();
						updateBullets();
						update_Asteroids();
						checkBulletAsteroidCollision();
						drawHudScore();
						swapBuffer();
						if (checkAsteroidShipCollision())
							endGame = 1;
						counter++;
					}

				//}
			default:
				break;
			}

		}
	}


	if (timer_unsubscribe_int() == ERROR) {
		printf("Error unsubscribing timer \n");
		return ERROR;
	}

	if (rtc_disable() == ERROR) {
		printf("Error disabling rtc \n");
		return ERROR;
	}

	if (rtc_unsubscribe_int() == ERROR) {
		printf("Error unsubscribing rtc \n");
		return ERROR;
	}



	return game->score;
}
