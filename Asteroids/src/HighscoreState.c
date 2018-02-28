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
#include "HighscoreState.h"
#include "mouse.h"
#include <unistd.h>
#include "NewHighscoreState.h"

extern HighscoreState* highscore;
extern int irq_set_mouse,irq_set_keyboard;
extern HighestScore* highestscore;
extern HighestScore* highestscore1;
extern HighestScore* highestscore2;
static Bitmap* letters[13];
static Bitmap* rectangle;
extern Date* currentDate;
extern Bitmap* alphabet[26];
int newHighscoreFlag = 0;

HighscoreState* newHighscoreState() {
    HighscoreState* state;
    state = (HighscoreState*) malloc(sizeof(HighscoreState));

    state->background = loadBitmap("/home/lcom/Asteroids/res/highscore.bmp");

    rectangle = loadBitmap("/home/lcom/Asteroids/res/evenBiggerRectangle.bmp");

    int x1 = 320;
    int y1 = 580;
    int x2 = 680;
    int y2 = 630;
    Rectangle *r = newRectangle(x1,y1,x2,y2);
    state->exitButton = r;

    state->done = 0;

    loadLetters();

    loadAlphabet();

    return state;
}

void updateHighscoreState() {
    // if mouse is inside the exit button rectangle (boundaries)
    if (mouseInsideRect(highscore->exitButton))
      if(getMouse()->leftButtonReleased)
        highscore->done = 1;
}

void loadLetters()
{
	letters[0] = loadBitmap("/home/lcom/Asteroids/res/0big.bmp");
	letters[1] = loadBitmap("/home/lcom/Asteroids/res/1big.bmp");
	letters[2] = loadBitmap("/home/lcom/Asteroids/res/2big.bmp");
	letters[3] = loadBitmap("/home/lcom/Asteroids/res/3big.bmp");
	letters[4] = loadBitmap("/home/lcom/Asteroids/res/4big.bmp");
	letters[5] = loadBitmap("/home/lcom/Asteroids/res/5big.bmp");
	letters[6] = loadBitmap("/home/lcom/Asteroids/res/6big.bmp");
	letters[7] = loadBitmap("/home/lcom/Asteroids/res/7big.bmp");
	letters[8] = loadBitmap("/home/lcom/Asteroids/res/8big.bmp");
	letters[9] = loadBitmap("/home/lcom/Asteroids/res/9big.bmp");
	letters[10] = loadBitmap("/home/lcom/Asteroids/res/traco.bmp");
	letters[11] = loadBitmap("/home/lcom/Asteroids/res/backslashbig.bmp");
  letters[12] = loadBitmap("/home/lcom/Asteroids/res/colonbig.bmp");
}

void deleteLetters(){
	deleteBitmap(letters[0]);
	deleteBitmap(letters[1]);
	deleteBitmap(letters[2]);
	deleteBitmap(letters[3]);
	deleteBitmap(letters[4]);
	deleteBitmap(letters[5]);
	deleteBitmap(letters[6]);
	deleteBitmap(letters[7]);
	deleteBitmap(letters[8]);
	deleteBitmap(letters[9]);
	deleteBitmap(letters[10]);
	deleteBitmap(letters[11]);
  deleteBitmap(letters[12]);
}

int drawNumber(int number, int x, int y){
  while (number >= 0)
  {
    x -= 18;
    int print = number % 10;
    switch(print)
    {
    case 0:
      drawBitmap(letters[0], x, y, ALIGN_LEFT);
      break;
    case 1:
      drawBitmap(letters[1], x, y, ALIGN_LEFT);
      break;
    case 2:
      drawBitmap(letters[2], x, y, ALIGN_LEFT);
      break;
    case 3:
      drawBitmap(letters[3], x, y, ALIGN_LEFT);
      break;
    case 4:
      drawBitmap(letters[4], x, y, ALIGN_LEFT);
      break;
    case 5:
      drawBitmap(letters[5], x, y, ALIGN_LEFT);
      break;
    case 6:
      drawBitmap(letters[6], x, y, ALIGN_LEFT);
      break;
    case 7:
      drawBitmap(letters[7], x, y, ALIGN_LEFT);
      break;
    case 8:
      drawBitmap(letters[8], x, y, ALIGN_LEFT);
      break;
    case 9:
      drawBitmap(letters[9], x, y, ALIGN_LEFT);
      break;
    }
    if(number < 10)
      break;
    else
      number = number / 10;
  }
  return x;
}

int drawDate(int number, int x, int y){
  int c = 0;
  while (number >= 0)
  {
    x -= 18;
    int print = number % 10;
    c++;
    switch(print)
    {
    case 0:
      drawBitmap(letters[0], x, y, ALIGN_LEFT);
      break;
    case 1:
      drawBitmap(letters[1], x, y, ALIGN_LEFT);
      break;
    case 2:
      drawBitmap(letters[2], x, y, ALIGN_LEFT);
      break;
    case 3:
      drawBitmap(letters[3], x, y, ALIGN_LEFT);
      break;
    case 4:
      drawBitmap(letters[4], x, y, ALIGN_LEFT);
      break;
    case 5:
      drawBitmap(letters[5], x, y, ALIGN_LEFT);
      break;
    case 6:
      drawBitmap(letters[6], x, y, ALIGN_LEFT);
      break;
    case 7:
      drawBitmap(letters[7], x, y, ALIGN_LEFT);
      break;
    case 8:
      drawBitmap(letters[8], x, y, ALIGN_LEFT);
      break;
    case 9:
      drawBitmap(letters[9], x, y, ALIGN_LEFT);
      break;
    }
    if(number < 10)
      break;
    else
      number = number / 10;
  }
  if(c<2){
    x -= 18;
    drawBitmap(letters[0], x, y, ALIGN_LEFT);
  }
  return x;
}

void drawHighscore(HighestScore* hs, int y) {
    int xPos = 924;
    if(hs->score != -1){
      int score = hs->score;
      int minute = (int) hs->date->minute;
      int hour = (int) hs->date->hour;
      int day = (int) hs->date->day;
      int month = (int) hs->date->month;
      int year = (int) hs->date->year;
      xPos = drawNumber(score, xPos, y);
      xPos = 624;
      xPos = drawDate(minute, xPos, y);
      xPos -= 18;
      drawBitmap(letters[12], xPos, y, ALIGN_LEFT);
      xPos = drawDate(hour, xPos, y);
      xPos -= 18;
      drawBitmap(letters[10], xPos, y, ALIGN_LEFT);
      xPos = drawDate(year, xPos, y);
      xPos -= 18;
      drawBitmap(letters[11], xPos, y, ALIGN_LEFT);
      xPos = drawDate(month, xPos, y);
      xPos -= 18;
      drawBitmap(letters[11], xPos, y, ALIGN_LEFT);
      xPos = drawDate(day, xPos, y);
    }
    if (mouseInsideRect(highscore->exitButton))
        drawBitmap(rectangle, highscore->exitButton->x1 - 15, highscore->exitButton->y1 - 12, ALIGN_LEFT);
    printHighscoreName(hs, 50, y);
}

void printHighscoreName(HighestScore* hs, int x, int y){
  int c = 0;
  while (hs->name[c] != '0')
  {
    c++;
    if(c == 10)
      break;
  }
  x += 18 * c;
  c--;
  while (c >= 0)
  {
    x -= 18;
    switch(hs->name[c])
    {
    case 'A':
      drawBitmap(alphabet[0], x, y, ALIGN_LEFT);
      break;
    case 'B':
      drawBitmap(alphabet[1], x, y, ALIGN_LEFT);
      break;
    case 'C':
      drawBitmap(alphabet[2], x, y, ALIGN_LEFT);
      break;
    case 'D':
      drawBitmap(alphabet[3], x, y, ALIGN_LEFT);
      break;
    case 'E':
      drawBitmap(alphabet[4], x, y, ALIGN_LEFT);
      break;
    case 'F':
      drawBitmap(alphabet[5], x, y, ALIGN_LEFT);
      break;
    case 'G':
      drawBitmap(alphabet[6], x, y, ALIGN_LEFT);
      break;
    case 'H':
      drawBitmap(alphabet[7], x, y, ALIGN_LEFT);
      break;
    case 'I':
      drawBitmap(alphabet[8], x, y, ALIGN_LEFT);
      break;
    case 'J':
      drawBitmap(alphabet[9], x, y, ALIGN_LEFT);
      break;
    case 'K':
      drawBitmap(alphabet[10], x, y, ALIGN_LEFT);
      break;
    case 'L':
      drawBitmap(alphabet[11], x, y, ALIGN_LEFT);
      break;
    case 'M':
      drawBitmap(alphabet[12], x, y, ALIGN_LEFT);
      break;
    case 'N':
      drawBitmap(alphabet[13], x, y, ALIGN_LEFT);
      break;
    case 'O':
      drawBitmap(alphabet[14], x, y, ALIGN_LEFT);
      break;
    case 'P':
      drawBitmap(alphabet[15], x, y, ALIGN_LEFT);
      break;
    case 'Q':
      drawBitmap(alphabet[16], x, y, ALIGN_LEFT);
      break;
    case 'R':
      drawBitmap(alphabet[17], x, y, ALIGN_LEFT);
      break;
    case 'S':
      drawBitmap(alphabet[18], x, y, ALIGN_LEFT);
      break;
    case 'T':
      drawBitmap(alphabet[19], x, y, ALIGN_LEFT);
      break;
    case 'U':
      drawBitmap(alphabet[20], x, y, ALIGN_LEFT);
      break;
    case 'V':
      drawBitmap(alphabet[21], x, y, ALIGN_LEFT);
      break;
    case 'W':
      drawBitmap(alphabet[22], x, y, ALIGN_LEFT);
      break;
    case 'X':
      drawBitmap(alphabet[23], x, y, ALIGN_LEFT);
      break;
    case 'Y':
      drawBitmap(alphabet[24], x, y, ALIGN_LEFT);
      break;
    case 'Z':
      drawBitmap(alphabet[25], x, y, ALIGN_LEFT);
      break;
    }
    c--;
  }
}

void drawHighscoreBackground(){
    drawBitmap(highscore->background, 0, 0, ALIGN_LEFT);
}

void deleteHighscoreState() {
    deleteRectangle(highscore->exitButton);

    deleteBitmap(highscore->background);

    deleteBitmap(rectangle);

    deleteLetters();

    deleteAlphabet();

    free(highscore);
}

char* newString(){
  char* string;
  string = (char*) malloc(11);
  char name[11] = {'0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '\0'};
  strcpy(string, name);
  return string;
}

HighestScore* newHighscore(){
    HighestScore* hs;
    hs = (HighestScore*) malloc(sizeof(HighestScore));
    hs->date = newDate();
    hs->score = -1;
    hs->name = newString();
    return hs;
}

void deleteString(char* string){
  free(string);
}

void deleteHighscore(HighestScore* hs){
  deleteDate(hs->date);
  deleteString(hs->name);
  free(hs);
}

int checkHighscore(int score){
    if(score > highestscore->score){
      highestscore2->date->year = highestscore1->date->year;
      highestscore2->date->month = highestscore1->date->month;
      highestscore2->date->day = highestscore1->date->day;
      highestscore2->date->hour = highestscore1->date->hour;
      highestscore2->date->minute = highestscore1->date->minute;
      highestscore2->date->second = highestscore1->date->second;
      highestscore2->score = highestscore1->score;
      strcpy(highestscore2->name, highestscore1->name);

      highestscore1->date->year = highestscore->date->year;
      highestscore1->date->month = highestscore->date->month;
      highestscore1->date->day = highestscore->date->day;
      highestscore1->date->hour = highestscore->date->hour;
      highestscore1->date->minute = highestscore->date->minute;
      highestscore1->date->second = highestscore->date->second;
      highestscore1->score = highestscore->score;
      strcpy(highestscore1->name, highestscore->name);

      highestscore->date->year = currentDate->year;
      highestscore->date->month = currentDate->month;
      highestscore->date->day = currentDate->day;
      highestscore->date->hour = currentDate->hour;
      highestscore->date->minute = currentDate->minute;
      highestscore->date->second = currentDate->second;
      highestscore->score = score;

      newHighscoreFlag = 1;

      return 1;
    } else {
      if(score > highestscore1->score){
        highestscore2->date->year = highestscore1->date->year;
        highestscore2->date->month = highestscore1->date->month;
        highestscore2->date->day = highestscore1->date->day;
        highestscore2->date->hour = highestscore1->date->hour;
        highestscore2->date->minute = highestscore1->date->minute;
        highestscore2->date->second = highestscore1->date->second;
        highestscore2->score = highestscore1->score;
        strcpy(highestscore2->name, highestscore1->name);

        highestscore1->date->year = currentDate->year;
        highestscore1->date->month = currentDate->month;
        highestscore1->date->day = currentDate->day;
        highestscore1->date->hour = currentDate->hour;
        highestscore1->date->minute = currentDate->minute;
        highestscore1->date->second = currentDate->second;
        highestscore1->score = score;

        newHighscoreFlag = 2;

        return 1;
      } else {
        if(score > highestscore2->score){
          highestscore2->date->year = currentDate->year;
          highestscore2->date->month = currentDate->month;
          highestscore2->date->day = currentDate->day;
          highestscore2->date->hour = currentDate->hour;
          highestscore2->date->minute = currentDate->minute;
          highestscore2->date->second = currentDate->second;
          highestscore2->score = score;

          newHighscoreFlag = 3;

          return 1;
        }
      }
    }
  return 0;
}

int startHighscoreState() {
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

	while (highscore->done == 0) {
		if ((r = driver_receive(ANY, &msg, &ipc_status)) != 0) {
			printf("driver_receive failed with: %d", r);
			continue;
		}

		if (is_ipc_notify(ipc_status)) {
			switch (_ENDPOINT_P(msg.m_source)) {
			case HARDWARE:
				if (msg.NOTIFY_ARG & irq_set_mouse) {
					updateMouse();
					updateHighscoreState();
				}
				if (msg.NOTIFY_ARG & irq_set_keyboard) {
					kbd_read_buffer_asm();

				}
				if (msg.NOTIFY_ARG & irq_set_timer) {

					if (counter % freq == 0) {
						drawHighscoreBackground();
						drawMouseMenu();
						drawHighscore(highestscore, 284);
						drawHighscore(highestscore1, 334);
						drawHighscore(highestscore2, 384);
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

void readScoresFile()
{
      FILE *fp;
      int tscore,tyear,tmonth,tday,thour,tmin,tsec;

      if((fp = fopen("/home/lcom/Asteroids/res/test.txt", "r")))
      {
      fscanf(fp, "%d %d %d %d %d %d %d %d", &tscore,&tday,&tmonth,&tyear,&thour,&tmin,&tsec);
      highestscore->score = tscore;
      highestscore->date->day = tday;
      highestscore->date->month = tmonth;
      highestscore->date->year = tyear;
      highestscore->date->hour = thour;
      highestscore->date->minute = tmin;
      highestscore->date->second = tsec;
      fgets(highestscore->name,11,fp);
      fscanf(fp, "%d %d %d %d %d %d %d %d", &tscore,&tday,&tmonth,&tyear,&thour,&tmin,&tsec);
      highestscore1->score = tscore;
      highestscore1->date->day = tday;
      highestscore1->date->month = tmonth;
      highestscore1->date->year = tyear;
      highestscore1->date->hour = thour;
      highestscore1->date->minute = tmin;
      highestscore1->date->second = tsec;
      fgets(highestscore1->name,11,fp);
      fscanf(fp, "%d %d %d %d %d %d %d %d", &tscore,&tday,&tmonth,&tyear,&thour,&tmin,&tsec);
      highestscore2->score = tscore;
      highestscore2->date->day = tday;
      highestscore2->date->month = tmonth;
      highestscore2->date->year = tyear;
      highestscore2->date->hour = thour;
      highestscore2->date->minute = tmin;
      highestscore2->date->second = tsec;
      fgets(highestscore2->name,11,fp);
      }

      return;
}

void writeScoresFile()
{
      FILE *fp;
      fp = fopen("/home/lcom/Asteroids/res/test.txt", "w+");
      fprintf(fp, "%d %d %d %d %d %d %d %s \n", highestscore->score,  highestscore->date->day,
              highestscore->date->month,  highestscore->date->year,  highestscore->date->hour ,
               highestscore->date->minute, highestscore->date->second, highestscore->name);
               fprintf(fp, "%d %d %d %d %d %d %d %s \n", highestscore1->score,  highestscore1->date->day,
                       highestscore1->date->month,  highestscore1->date->year,  highestscore1->date->hour ,
                        highestscore1->date->minute, highestscore1->date->second, highestscore1->name);
                        fprintf(fp, "%d %d %d %d %d %d %d %s \n", highestscore2->score,  highestscore2->date->day,
                                highestscore2->date->month,  highestscore2->date->year,  highestscore2->date->hour ,
                                 highestscore2->date->minute, highestscore2->date->second, highestscore2->name);


      fclose(fp);
      return;
}
