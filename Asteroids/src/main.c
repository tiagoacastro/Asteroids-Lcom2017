#include <string.h>
#include <minix/syslib.h>
#include <minix/drivers.h>
#include <minix/com.h>
#include <sys/mman.h>
#include <sys/types.h>
#include <stdint.h>

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
#include "Bitmap.h"
#include "game.h"
#include "mouse.h"
#include "MainMenuState.h"
#include "HighscoreState.h"
#include "EndGameState.h"
#include "NewHighscoreState.h"

Game* game;
MainMenuState* menu;
HighscoreState* highscore;
EndGameState* endGame;
NewHighscoreState* newHighscoreS;
Date* currentDate;
int lastScore;

HighestScore* highestscore;
HighestScore* highestscore1;
HighestScore* highestscore2;
int irq_set_mouse,irq_set_keyboard;

typedef enum {S_EXIT, S_MENU, S_GAME, S_HIGHSCORE, S_END_GAME, S_NEW_HIGHSCORE} states;

int main() {
	srand(time(NULL));

	sef_startup();

	highestscore = newHighscore();
  highestscore1 = newHighscore();
	highestscore2 = newHighscore();
	currentDate = newDate();
	states s = S_MENU;

	readScoresFile();

	irq_set_keyboard = kbd_subscribe_int();

		if (irq_set_keyboard == ERROR) {
			printf("Error subscribing to keyboard \n");
			return ERROR;
		}
	irq_set_mouse = mouse_subscribe_int();

	if(irq_set_mouse == ERROR){
		printf("Error subscribing to mouse \n");
		return ERROR;
	}

	if(enable_packets() == ERROR){
		printf("Error enabling packets \n");
		return ERROR;
	}

	if(vg_init(V1024X768RGB) == NULL)
		return ERROR;

	sys_enable_iop(SELF);

	while(s != S_EXIT){
		switch(s){
			case S_MENU:
				resetMouseFlags();
				menu = newMainMenuState();
				startMainMenuState();
				switch (menu->next){
					case 1:
						s = S_GAME;
						break;
					case 2:
						s = S_HIGHSCORE;
						break;
					default:
						s = S_EXIT;
						break;
				}
				deleteMainMenuState();
				break;
			case S_GAME:
				resetMouseFlags();
				game = newGame();
				lastScore = startGame();
				if(checkHighscore(game->score)){
					s = S_NEW_HIGHSCORE;
				}
				else{
					s = S_END_GAME;
				}
				deleteGame();
				break;
			case S_END_GAME:
				resetMouseFlags();
				endGame = newEndGameState();
				startEndGameState();
				s = S_MENU;
				deleteEndGameState();
				break;
			case S_NEW_HIGHSCORE:
				resetMouseFlags();
				newHighscoreS = newNewHighscoreState();
				startNewHighscoreState();
				s = S_MENU;
				deleteNewHighscoreState();
				break;
			case S_HIGHSCORE:
				resetMouseFlags();
				highscore = newHighscoreState();
				startHighscoreState();
				s = S_MENU;
				deleteHighscoreState();
				break;
			case S_EXIT:
				break;
		}
	}

	if(disable_data_reporting() == ERROR){
		printf("Error disabling data reporting \n");
		return ERROR;
	}

	if (mouse_unsubscribe_int() == ERROR){
		printf("Error unsubscribing to mouse\n");
		return ERROR;
	}

	vg_exit();

	writeScoresFile();

	deleteMouse();
	deleteHighscore(highestscore);
	deleteHighscore(highestscore1);
	deleteHighscore(highestscore2);
	deleteDate(currentDate);
	if (kbd_unsubscribe_int() == ERROR) {
			printf("Error unsubscribing keyboard \n");
			return ERROR;
		}
	if (empty_OUTBUFF() == ERROR){
		printf("Error emptying OUTBUF\n");
		return ERROR;
	}

	return 0;
}
